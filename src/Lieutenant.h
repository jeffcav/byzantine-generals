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
    void discoverGeneralsAddresses();
    void discoverGenerals();

private:
    void openSocket();

    vector<Message> receiveMessages(int nMessages);
    Message receiveMessage(GeneralAddress general);
    void saveReceivedMessages(int round, vector<Message> msgs);

    void sendMessage(GeneralAddress general, Message msg);

    vector<Message> OM(int nGenerals, int nTraitors, int k);
    void actAsCommander(vector<Message> msgs);
    Command majority(int k);

    void sabotage(Message *msg);

    void setSender(Message *msg);

    Message receiveFromCommander();
};


#endif //BYZANTINE_GENERALS_LIEUTENANT_H
