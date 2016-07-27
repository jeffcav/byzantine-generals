//
// Created by jeffrodrigo on 18/07/16.
//
#ifndef BYZANTINE_GENERALS_LIEUTENANT_H
#define BYZANTINE_GENERALS_LIEUTENANT_H


#include "Message.h"
#include "GeneralIdentity.h"
#include "General.h"
#include <map>
#include <vector>

using namespace std;

class Lieutenant : public General {

private:
    map <int, vector<Message>> messages;
    vector<GeneralAddress> generals;

    int serverSock;
    int commanderSock;

public:
    Lieutenant(int32_t, Loyalty, int, int);
    Lieutenant(int32_t, int, int);

    void run();
    ~Lieutenant();

protected:
    void discoverGenerals();

private:
    void openServerSocket();
    int connectToGenerals();
    int waitNewGeneralsConnections();

    uint32_t receiveGeneralIdentification(int sock, int *status);

    vector<Message> receiveMessages(int round);
    Message receiveMessage(GeneralAddress general);
    void saveReceivedMessages(int round, vector<Message> msgs);

    void sendMessages(GeneralAddress general, vector<Message> msgs);
    void sendMessage(GeneralAddress general, Message msg);

    vector<Message> OM(int nGenerals, int nTraitors, int k);
    void actAsCommander(vector<Message> msgs);
    Command majority(int k);

    void sabotage(Message *msg);

    void prepareMessage(Message *msg);
};


#endif //BYZANTINE_GENERALS_LIEUTENANT_H
