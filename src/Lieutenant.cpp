//
// Created by jeffrodrigo on 18/07/16.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include "Lieutenant.h"

Lieutenant::Lieutenant(int32_t id, Loyalty loyalty, int nGenerals, int nTraitors)
        : General(id, loyalty, lieutenant, nGenerals, nTraitors)
{
    discoverGeneralsAddresses();
}

Lieutenant::Lieutenant(int32_t id, int nGenerals, int nTraitors)
        : General(id,
                  id <= nTraitors? traitor : loyal,
                  lieutenant, nGenerals, nTraitors)
{
    discoverGeneralsAddresses();
}

void Lieutenant::run()
{
    openSocket();

    for (int round=0; round <= this->numberOfTraitors; round++) {
        communicate(round);
    }

    decide();
}


void Lieutenant::discoverGeneralsAddresses()
{
    this->generalAddresses = new string[this->numberOfGenerals - 2];

    int nextAddress = 0;
    for (int host = 1; nextAddress < (this->numberOfGenerals - 2); host++) {
        if (host == myID.name)
            continue;

        stringstream address;
        address << "10.0.0." << host;

        this->generalAddresses[nextAddress] = address.str();
        cout << "Adding " << generalAddresses[nextAddress] << endl;
        nextAddress++;
    }
}

Message Lieutenant::receiveMessage()
{
    char buffer[6];
    recv(this->sock, buffer, 6, 0);

    Message message(buffer);

    cout << "Received: " << message.message << " from lieutenant " << to_string(message.source.name) << endl;

    return message;
}



void Lieutenant::communicate(int round)
{
    if (round > 0) {
        for (int i = 0; i < messages[round-1].size(); i++) {
            informLieutenants(messages[round-1][i]);
        }
    }

    int nMsgs = pow(numberOfGenerals-2, round);

    for (int i = 0; i < nMsgs; i++) {
        Message msg = receiveMessage();
        saveReceivedMessage(round, msg);
    }
}

void Lieutenant::informLieutenants(Message msg) {
    msg.source = this->myID;

    for (int i = 0; i < this->numberOfGenerals - 2; i++) {
        if (this->isATraitor())
            msg = sabotage(msg);

        sendMessage(generalAddresses[i], msg);
        cout << "Sent " << msg.message << " from " << msg.source.name << " to "<< generalAddresses[i] << endl;
    }
}

void Lieutenant::openSocket() {
    struct sockaddr_in saddr;
    int len = sizeof(struct sockaddr_in);

    saddr.sin_port = htons(5000);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(this->sock, (struct sockaddr*) &saddr, (socklen_t)  len);
}

void Lieutenant::saveReceivedMessage(int round, Message msg) {
    this->messages[round].push_back(msg);
}

void Lieutenant::sendMessage(string address, Message msg) {
    char buffer[msg.size()];
    struct sockaddr_in saddr;
    int len = sizeof(struct sockaddr_in);

    saddr.sin_port = htons(5000);
    saddr.sin_family = AF_INET;
    inet_aton(address.c_str(), &saddr.sin_addr);

    msg.serialize(buffer);
    sendto(this->sock, buffer, (size_t) msg.size(), 0, (struct sockaddr*) &saddr, (socklen_t) len);
}

Lieutenant::~Lieutenant() {
    close(this->sock);
}

void Lieutenant::decide()
{
    int numAttack = 0, numRetreat = 0;

    for (int round = 0; round <= this->numberOfTraitors; round++) {
        int nMessages = this->messages[round].size();
        for (int msg = 0; msg < nMessages; msg++) {

            if (this->messages[round][msg].message == 'A')
                numAttack++;
            else
                numRetreat++;
        }
    }


    if (numAttack >= numRetreat)
        cout << "ATTACK\n";
    else
        cout << "RETREAT\n";
}

Message Lieutenant::sabotage(Message msg) {
    msg.message = 'R';
    return msg;
}

bool Lieutenant::isATraitor() {
    return this->loyalty==traitor;
}
































