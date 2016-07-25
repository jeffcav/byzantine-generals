//
// Created by jeffrodrigo on 18/07/16.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include "Lieutenant.h"

Lieutenant::Lieutenant(int32_t id, Loyalty loyalty, int nGenerals, int nTraitors)
        : General(id, loyalty, lieutenant, nGenerals, nTraitors)
{
    discoverGenerals();
}

Lieutenant::Lieutenant(int32_t id, int nGenerals, int nTraitors)
        : Lieutenant(id, id <= nTraitors? traitor : loyal, nGenerals, nTraitors)
{ }

void Lieutenant::run()
{
    OM(this->numberOfGenerals, this->numberOfTraitors, this->numberOfTraitors);
}

vector<Message> Lieutenant::OM(int nGenerals, int nTraitors, int k)
{
    vector<Message> receivedMessages = receiveMessages(k);

    sleep(1);
    cout << endl;

    if (k == 0)
        return receivedMessages;

    actAsCommander(receivedMessages);
    receivedMessages = OM(nGenerals, nTraitors, k-1);

    if (k == nTraitors) {
        Command c = majority(k);
        cout << (c==attack?"Attack!":"Retreat!") << endl;
    }

    return receivedMessages;
}

Message Lieutenant::receiveMessage(GeneralAddress general)
{
    char buffer[6];
    recv(general.sock, buffer, 6, 0);

    Message message(buffer);

    cout << "Received: " << message.printCommand()
         << " from lieutenant " << to_string(message.source.name)
         << endl;

    return message;
}

void Lieutenant::saveReceivedMessages(int round, vector<Message> msgs)
{
    this->messages[round] = msgs;
}

Lieutenant::~Lieutenant()
{
    close(this->serverSock);
}

vector<Message> Lieutenant::receiveMessages(int round)
{
    Message msg;
    int nMessages;
    vector<Message> msgs;
    GeneralAddress commander, *general;

    commander = GeneralAddress(GeneralIdentity(0),
                               commanderSock);

    nMessages = pow(this->numberOfGenerals - 2,
                    this->numberOfTraitors - round);

    for (int i = 0; i < nMessages; i++) {
        if (nMessages == 1)
            general = &commander;
        else
            general = &generals[i % (numberOfGenerals - 2)];

        msg = receiveMessage(*general);
        msgs.push_back(msg);
    }

    saveReceivedMessages(round, msgs);

    return msgs;
}

Command Lieutenant::majority(int k)
{
    int nAttack = 0, nRetreat = 0;

    for (int round = 0; round <= k; round++) {
        vector<Message> msgs = messages[round];

        for (int i = 0; i < msgs.size(); i++) {

            if (msgs[i].command == attack)
                nAttack++;
            else
                nRetreat++;
        }
    }

    if (nAttack > nRetreat)
        return attack;
    return retreat;

}

void Lieutenant::actAsCommander(vector<Message> msgs)
{
    for (int i = 0; i < this->generals.size(); i++) {
        sendMessages(generals[i], msgs);
    }
}

void Lieutenant::sendMessages(GeneralAddress general, vector<Message> msgs) {
    Message sndMsg;
    for (int i = 0; i < msgs.size(); i++) {
        sndMsg = msgs[i];

        prepareMessage(&sndMsg);
        sendMessage(general, sndMsg);
    }
}

void Lieutenant::prepareMessage(Message *msg)
{
    msg->source = this->myID;

    if (this->isTraitorous())
        sabotage(msg);
}

void Lieutenant::sabotage(Message *msg)
{
    msg->command = retreat;
}

void Lieutenant::sendMessage(GeneralAddress general, Message msg)
{
    char buffer[msg.size()];
    msg.serialize(buffer);
    send(general.sock, buffer, (size_t) msg.size(), 0);
}


static struct sockaddr_in buildSockAddr(string *ip, int port)
{
    struct sockaddr_in addr;

    memset(&addr, 0 ,sizeof(struct sockaddr_in));
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    if (ip)
        inet_aton(ip->c_str(), &addr.sin_addr);
    else
        addr.sin_addr.s_addr = INADDR_ANY;

    return addr;
}

void Lieutenant::discoverGenerals()
{
    openServerSocket();

    connectToGenerals();

    waitNewGeneralsConnections();
}

void Lieutenant::openServerSocket()
{
    int option;
    socklen_t len;
    struct sockaddr_in addr;

    option = 1;
    len = sizeof(addr);
    this->serverSock = socket(AF_INET,
                              SOCK_STREAM,
                              IPPROTO_TCP);

    if (setsockopt(this->serverSock,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &option,
                   sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    addr = buildSockAddr(NULL, 5000);
    bind(serverSock, (struct sockaddr*) &addr, len);

    listen(this->serverSock, this->numberOfGenerals);
}

void Lieutenant::connectToGenerals() {
    int sock;
    socklen_t len;
    string ip, prefix;
    struct sockaddr_in addr;

    prefix = "10.0.0.";
    len = sizeof(struct sockaddr_in);

    for (int host = 1; host < this->myID.name; host++) {
        ip = prefix + to_string(host);

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        addr = buildSockAddr(&ip, 5000);
        connect(sock, (struct sockaddr*) &addr, len);
        send(sock, (char*) &myID.name, 4, 0);

        GeneralAddress newGeneral(GeneralIdentity(host), sock);
        generals.push_back(newGeneral);

        cout << "Connection to " << ip << endl;
    }
}

void Lieutenant::waitNewGeneralsConnections() {
    int clientSock;
    string prefix;
    socklen_t len;
    uint32_t generalID;
    struct sockaddr_in addr;

    prefix = "10.0.0.";

    // Lieutenant Generals
    for (int i = this->myID.name + 1; i <= numberOfGenerals; i++) {
        clientSock = accept(serverSock, (struct sockaddr*) &addr, &len);
        read(clientSock, (char*) &generalID, 4);

        if (generalID == 0) {
            commanderSock = clientSock;
            continue;
        }

        GeneralAddress newGeneral(GeneralIdentity(generalID), clientSock);
        generals.push_back(newGeneral);

        cout << "Connection from " << prefix << generalID << endl;
    }

    // Commanding General
    //commanderSock = accept(serverSock, (struct sockaddr*) &addr, &len);
}

