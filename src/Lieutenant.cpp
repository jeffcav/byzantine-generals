//
// Created by jeffrodrigo on 18/07/16.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h>
#include <iostream>
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

static Message readFromSource() {
    char buffer[6];
    struct sockaddr_in saddr;
    int len = sizeof(struct sockaddr_in);
    int source = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    saddr.sin_port = htons(5000);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    bind(source, (struct sockaddr*) &saddr, len);
    recv(source, buffer, 6, 0);

    Message message(buffer);
    close(source);

    cout << "Received: " << message.message << " from the commanding general\n";

    return message;
}

void Lieutenant::run() {
    runCommon();
}

void Lieutenant::runCommon() {
    Message m = readFromSource();
}

void Lieutenant::discoverGeneralsAddresses() {
    this->generalAddresses = new string[this->numberOfGenerals];

    for (int i = 1; i <= this->numberOfGenerals; i++) {
        if (i == myID.name)
            continue;

        stringstream address;
        address << "10.0.0." << i;

        this->generalAddresses[i - 1] = address.str();
    }
}











