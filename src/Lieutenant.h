//
// Created by jeffrodrigo on 18/07/16.
//
#include "Message.h"
#include "LieutenantID.h"
#include <map>
#include <list>

using namespace std;

#ifndef BYZANTINE_GENERALS_LIEUTENANT_H
#define BYZANTINE_GENERALS_LIEUTENANT_H

enum Loyalty{
    loyal, traitor
};

enum Attribution {
    source, common
};

class Lieutenant {
public:
    LieutenantID myID;
    Loyalty loyalty;
    Attribution attribution;

private:
    int port;
    int nTraitors;
    int nLieutenants;
    string *colleagues;

    map <int, list<Message>> messages;
public:
    Lieutenant(LieutenantID id, Loyalty loyalty, Attribution attribution);

    Lieutenant(LieutenantID id, int nLieutenants, int nTraitors);

    void run();

    void runCommon();

    void runSource();

private:
    void findColleagues();

    void sendMessage(string lieutenantAddress, Message message);
};


#endif //BYZANTINE_GENERALS_LIEUTENANT_H
