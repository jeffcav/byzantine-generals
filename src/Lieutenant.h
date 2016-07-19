//
// Created by jeffrodrigo on 18/07/16.
//
#ifndef BYZANTINE_GENERALS_LIEUTENANT_H
#define BYZANTINE_GENERALS_LIEUTENANT_H


#include "Message.h"
#include "GeneralIdentity.h"
#include "General.h"
#include <map>
#include <list>

using namespace std;

class Lieutenant : public General {
private:
    map <int, list<Message>> messages;

public:
    Lieutenant(int32_t, Loyalty, int, int);

    Lieutenant(int32_t, int, int);

    void run();

protected:
    void discoverGeneralsAddresses();

private:
    void runCommon();
};


#endif //BYZANTINE_GENERALS_LIEUTENANT_H
