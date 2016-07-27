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

extern int BYZ_RUNLOCAL;

Lieutenant::Lieutenant(int32_t id, Loyalty loyalty, int nGenerals, int nTraitors)
        : General(id, loyalty, lieutenant, nGenerals, nTraitors)
{
    discoverGenerals();
}

Lieutenant::Lieutenant(int32_t id, int nGenerals, int nTraitors)
        : Lieutenant(id, id <= nTraitors? traitor : loyal, nGenerals, nTraitors)
{
}

Lieutenant::~Lieutenant()
{
    close(serverSock);

    for (int i = 0; i < generals.size(); i++)
        close (generals[i].sock);

    close(commanderSock);
}

void Lieutenant::run()
{
    OM(this->numberOfGenerals, this->numberOfTraitors, this->numberOfTraitors);
}

vector<Message> Lieutenant::OM(int nGenerals, int nTraitors, int k)
{
    vector<Message> receivedMessages = receiveMessages(k);

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
    int retval;
    size_t count;
    char buffer[MSG_MAXBUFLEN];
    uint16_t pathlen;

    fd_set fds;
    struct timeval tv;

    tv.tv_sec = 15;
    tv.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(general.sock, &fds);

    retval = select(general.sock+1, &fds, NULL, NULL, &tv);
    if (retval < 1)
        cout << "Error listening to socket\n";
    //TODO: on timeout, pretend the reception of a RETREAT

    retval = (int) recv(general.sock, &pathlen, 2, 0);
    if (retval < 2)
        cout << "Error receiving message\n";

    memcpy(&buffer[0], (char*) &pathlen, 2);

    count = (size_t) (4 * pathlen) + 1;
    if (recv(general.sock, &buffer[2], count, 0) < count)
        cout << "Error receiving message\n";

    Message message(buffer);

    cout << "Received: " << message.toString() << endl;

    return message;
}

void Lieutenant::saveReceivedMessages(int round, vector<Message> msgs)
{
    this->messages[round] = msgs;
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
        if (round == this->numberOfTraitors)
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

void Lieutenant::sendMessages(GeneralAddress general, vector<Message> msgs)
{
    Message sndMsg;
    for (int i = 0; i < msgs.size(); i++) {
        sndMsg = msgs[i];

        prepareMessage(&sndMsg);

        cout << "Sending "<< sndMsg.toString() << endl;

        sendMessage(general, sndMsg);
    }
}

void Lieutenant::prepareMessage(Message *msg)
{
    msg->appendSource(myID);

    if (this->isTraitorous())
        sabotage(msg);
}

void Lieutenant::sabotage(Message *msg)
{
    msg->command = retreat;
}

void Lieutenant::sendMessage(GeneralAddress general, Message msg)
{
    ssize_t count;
    char buffer[msg.size()];
    msg.serialize(buffer);

    count = send(general.sock, buffer, (size_t) msg.size(), 0);
    if (count < msg.size())
        cout << "Error sending message\n";
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

//TODO: close sockets on error
void Lieutenant::discoverGenerals()
{
    openServerSocket();

    if (connectToGenerals()) {
        cout << "Could not connect to generals. Exiting...\n";
        exit(1);
    }

    if (waitNewGeneralsConnections()) {
        cout << "Could not be found by generals. Exiting...\n";
        exit(1);
    }
}

void Lieutenant::openServerSocket()
{
    int option;
    socklen_t len;
    struct linger lngr;
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
        perror("setsockopt: reuseaddr");
        exit(1);
    }

    lngr.l_linger = 120;
    lngr.l_onoff = 1;
    if (setsockopt(this->serverSock,
                   SOL_SOCKET,
                   SO_LINGER,
                   &lngr,
                   sizeof(struct linger))) {
        perror("setsockopt: linger\n");
        exit(1);
    }

    int port = 15000 + myID.name;

    addr = buildSockAddr(NULL, port);
    bind(serverSock, (struct sockaddr*) &addr, len);

    listen(this->serverSock, this->numberOfGenerals);
}

int Lieutenant::connectToGenerals()
{
    int sock;
    int port;
    socklen_t len;
    string ip, prefix;
    struct sockaddr_in addr;
    struct linger lngr;

    lngr.l_linger = 120;
    lngr.l_onoff = 1;

    prefix = "10.0.0.";
    ip = "127.0.0.1";
    len = sizeof(struct sockaddr_in);

    for (int host = 1; host < myID.name; host++) {

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock <= 0) {
            cout << "Error opening socket\n";
            return -1;
        }

        if (!BYZ_RUNLOCAL)
            ip = prefix + to_string(host);
        port = 15000 + host;
        addr = buildSockAddr(&ip, port);

        if (connect(sock, (struct sockaddr*) &addr, len) < 0) {
            cout << "Error connecting\n";
            return -1;
        }

        if (setsockopt(sock,
                       SOL_SOCKET,
                       SO_LINGER,
                       &lngr,
                       sizeof(struct linger))) {
            perror("Could not set linger\n");
            exit(1);
        }

        if (send(sock, (char*) &myID.name, 4, 0) < 4) {
            cout << "Error sending my ID\n";
            return -1;
        }

        GeneralAddress newGeneral(GeneralIdentity(host), sock);
        generals.push_back(newGeneral);

        cout << "Connection to " << host << endl;
    }

    return 0;
}

int Lieutenant::waitNewGeneralsConnections()
{
    int error;
    int sock;

    socklen_t len;
    uint32_t generalID;
    struct sockaddr addr;
    struct linger lngr;

    lngr.l_linger = 120;
    lngr.l_onoff = 1;

    for (int i = myID.name + 1; i <= numberOfGenerals; i++) {
        sock = accept(serverSock, &addr, &len);

        if (sock <= 0) {
            cout << "Connection error\n";
            return -1;
        }

        if (setsockopt(sock,
                       SOL_SOCKET,
                       SO_LINGER,
                       &lngr,
                       sizeof(struct linger))) {
            perror("Could not set linger\n");
            exit(1);
        }

        generalID = receiveGeneralIdentification(sock, &error);

        if (error) {
            return -1;
        }

        if (generalID) {
            GeneralAddress newGeneral(GeneralIdentity(generalID), sock);
            generals.push_back(newGeneral);
        }
        else {
            this->commanderSock = sock;
        }

        cout << "Connection from " << generalID << endl;
    }

    return 0;
}

uint32_t Lieutenant::receiveGeneralIdentification(int sock, int *error)
{
    uint32_t id;
    fd_set fds;
    struct timeval tv;

    tv.tv_sec = 15;
    tv.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    if (select(sock+1, &fds, NULL, NULL, &tv) < 1) {
        cout << "Connection error\n";
        *error = 1;
        return 0;
    }

    if (read(sock, (char*) &id, 4) < 4) {
        cout << "Read error\n";
        *error = 1;
        return 0;
    }

    *error = 0;
    return id;
}
