//
// Created by jeffrodrigo on 19/07/16.
//
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Commander.h"

using namespace std;

Commander::Commander(int nGenerals, int nTraitors) :
        General(0, loyal, commanding, nGenerals, nTraitors)
{
    discoverGeneralsAddresses();
}

void Commander::discoverGeneralsAddresses()
{
    socklen_t len = sizeof(struct sockaddr_in);
    string prefix = "10.0.0.";

    for (int host = 1; host < numberOfGenerals; host++) {
        struct GeneralAddress general;
        struct sockaddr_in caddr;
        string generalIP = prefix + to_string(host);

        general.id = GeneralIdentity(host);
        general.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        generals.push_back(general);

        memset(&caddr, 0 ,sizeof(struct sockaddr_in));
        caddr.sin_port = htons(5000);
        caddr.sin_family = AF_INET;

        inet_aton(generalIP.c_str(), &caddr.sin_addr);

        connect(general.sock, (struct sockaddr*) &caddr, len);

        cout << "Connected to " << generalIP << endl;
    }
    cout << endl;
}

void Commander::run()
{
    for (int i = 0; i < this->numberOfGenerals - 1; i++) {
        Message message(this->myID, attack);
        sendMessage(this->generals[i], message);
    }
}

void Commander::sendMessage(GeneralAddress general, Message message)
{
    char buffer[6];
    struct sockaddr_in saddr;
    socklen_t len = sizeof(struct sockaddr_in);

    message.serialize(buffer);
    sendto(general.sock, buffer, (size_t) message.size(), 0, (struct sockaddr*) &saddr, len);

    close(general.sock);

    cout << "Sent " << message.printCommand() << " to " << general.id.name << "\n";
}







