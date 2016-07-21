//
// Created by jeffrodrigo on 19/07/16.
//
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include "Commandant.h"

using namespace std;

Commandant::Commandant(int nGenerals, int nTraitors) :
        General(0, loyal, commanding, nGenerals, nTraitors)
{
    discoverGeneralsAddresses();
}

void Commandant::discoverGeneralsAddresses()
{
    for (int host = 1; host < this->numberOfGenerals; host++) {
        stringstream address;
        address << "10.0.0." << host;

        this->generalAddresses.push_back(address.str());
    }
}

void Commandant::run()
{
    for (int i = 0; i < this->numberOfGenerals - 1; i++) {
        Message message(this->myID, attack);
        sendMessage(this->generalAddresses[i], message);
    }
}

void Commandant::sendMessage(string ltAddress, Message message)
{
    char buffer[6];
    struct sockaddr_in saddr;
    socklen_t len = sizeof(struct sockaddr_in);
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    saddr.sin_port = htons(5000);
    saddr.sin_family = AF_INET;
    inet_aton(ltAddress.c_str(), &saddr.sin_addr);

    bind(sock, (struct sockaddr*) &saddr, len);
    message.serialize(buffer);
    sendto(sock, buffer, (size_t) message.size(), 0, (struct sockaddr*) &saddr, len);

    close(sock);

    cout << "Sent " << message.printCommand() << " to " << ltAddress << "\n";
}







