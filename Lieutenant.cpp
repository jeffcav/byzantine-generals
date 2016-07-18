//
// Created by jeffrodrigo on 18/07/16.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include "Lieutenant.h"

Lieutenant::Lieutenant(LieutenantID id, Loyalty loyalty, Attribution attribution) : myID(id) {
    this->attribution = attribution;
    this->loyalty = loyalty;
}

Lieutenant::Lieutenant(LieutenantID id, int nLieutenants, int nTraitors) : myID(id) {
    this->nTraitors = nTraitors;
    this->nLieutenants = nLieutenants;

    if (id.name == 0) {
        this->attribution = source;
        this->loyalty = loyal;
    }
    else {
        this->attribution = common;
        this->loyalty = id.name <= nTraitors? traitor : loyal;
    }
}


void Lieutenant::run() {
    findColleagues();
    if (this->attribution == source)
        runSource();
    else
        runCommon();
}

static Message readFromSource() {
    struct sockaddr_in saddr;
    int len = sizeof(struct sockaddr_in);
    int source = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    saddr.sin_port = htons(5000);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;


    bind(source, (struct sockaddr*) &saddr, len);

    char buffer[6];

    cout << "Waiting message\n";

    recv(source, buffer, 6, 0);
    //recvfrom(source, buffer, 6, 0, (struct sockaddr*) &saddr, (socklen_t*)&len);

    Message message(buffer);
    close(source);

    cout << "message received: " << message.message << "\n";

    return message;
}

void Lieutenant::runCommon() {
    Message m = readFromSource();
}

void Lieutenant::runSource() {
    cout << "Running source\n";

    for (int i = 1; i < nLieutenants; i++) {
        Message message(this->myID, attack);
        sendMessage(colleagues[i], message);
    }
}

void Lieutenant::sendMessage(string lieutenantAddress, Message message) {
    cout << "Send msg \n" ;
    char buffer[6];
    struct sockaddr_in saddr;
    int len = sizeof(struct sockaddr_in);
    int lieutenant = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    saddr.sin_port = htons(5000);
    saddr.sin_family = AF_INET;
    inet_aton(lieutenantAddress.c_str(), &saddr.sin_addr);

    cout << "sending " << message.message << " to " << lieutenantAddress << "\n";

    bind(lieutenant, (struct sockaddr*) &saddr, len);
    message.serialize(buffer);
    sendto(lieutenant, buffer, message.size(), 0, (struct sockaddr*) &saddr, len);

    close(lieutenant);
}

void Lieutenant::findColleagues() {
    this->colleagues = new string[nLieutenants];
    for (int i = 1; i < nLieutenants; i++) {
        stringstream address;
        address << "10.0.0." << i;
        colleagues[i] = address.str();
    }
}









