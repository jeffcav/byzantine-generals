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
    int sock;

public:
    Lieutenant(int32_t, Loyalty, int, int);
    Lieutenant(int32_t, int, int);
    void run();
    ~Lieutenant();

protected:
    void discoverGeneralsAddresses();

private:
    void communicate(int round);
    void informLieutenants(Message msg);
    void openSocket();

    Message receiveMessage();
    void saveReceivedMessage(int round, Message msg);
    void sendMessage(string address, Message msg);

};


#endif //BYZANTINE_GENERALS_LIEUTENANT_H
