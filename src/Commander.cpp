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

extern int BYZ_RUNLOCAL;

Commander::Commander(int nGenerals, int nTraitors) :
        General(0, loyal, commanding, nGenerals, nTraitors)
{
    discoverGenerals();
}

void Commander::discoverGenerals()
{
    socklen_t len = sizeof(struct sockaddr_in);
    string prefix = "10.0.0.";
    uint32_t myID = 0;
    string generalIP;

    for (int host = 1; host < numberOfGenerals; host++) {
        struct GeneralAddress general;
        struct sockaddr_in caddr;

        if (BYZ_RUNLOCAL == 0)
            generalIP = prefix + to_string(host);
        else
            generalIP = "127.0.0.1";

        general.id = GeneralIdentity(host);
        general.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        generals.push_back(general);

        memset(&caddr, 0 ,sizeof(struct sockaddr_in));

        int port = 5000 + host;
        caddr.sin_port = htons(port);
        caddr.sin_family = AF_INET;

        inet_aton(generalIP.c_str(), &caddr.sin_addr);

        if (connect(general.sock, (struct sockaddr*) &caddr, len) < 0)
            cout << "Error connecting to " << host << endl;

        if (send(general.sock, (char*) &myID, 4, 0) < 4)
            cout << "Error sending to " << host << endl;

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
    char buffer[5];
    struct sockaddr_in saddr;
    socklen_t len = sizeof(struct sockaddr_in);

    message.serialize(buffer);
    send(general.sock, buffer, 5, 0);

    sleep(1);

    close(general.sock);

    cout << "Sent " << message.printCommand() << " to " << general.id.name << "\n";
}







