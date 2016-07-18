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
    LieutenantID id;
    Loyalty loyalty;
    Attribution attribution;

    map <int, list<Message>> messages;
public:
    Lieutenant(int id, Loyalty loyalty, Attribution attribution);
    Lieutenant(int myID, int nTraitors);
    void run();
};


#endif //BYZANTINE_GENERALS_LIEUTENANT_H
