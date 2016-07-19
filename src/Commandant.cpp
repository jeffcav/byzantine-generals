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
#include "Message.h"

using namespace std;

Commandant::Commandant(int nGenerals, int nTraitors) :
        General(0, loyal, commanding, nGenerals, nTraitors)
{
    discoverGeneralsAddresses();
}

void Commandant::discoverGeneralsAddresses()
{
    this->generalAddresses = new string[this->numberOfGenerals];

    for (int i = 1; i <= this->numberOfGenerals; i++) {
        stringstream address;
        address << "10.0.0." << i;

        this->generalAddresses[i - 1] = address.str();
    }
}

void Commandant::run()
{
    for (int i = 0; i < this->numberOfGenerals; i++) {
        Message message(this->myID, attack);
        sendMessage(this->generalAddresses[i], message);
    }
}

void Commandant::sendMessage(string ltAddress, Message message)
{
    char buffer[6];
    struct sockaddr_in saddr;
    int len = sizeof(struct sockaddr_in);
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    saddr.sin_port = htons(5000);
    saddr.sin_family = AF_INET;
    inet_aton(ltAddress.c_str(), &saddr.sin_addr);

    bind(sock, (struct sockaddr*) &saddr, len);
    message.serialize(buffer);
    sendto(sock, buffer, message.size(), 0, (struct sockaddr*) &saddr, len);

    close(sock);

    cout << "Sent " << message.message << " to " << ltAddress << "\n";
}







