//
// Created by jeffrodrigo on 19/07/16.
//

#ifndef BYZANTINE_GENERALS_GENERAL_H
#define BYZANTINE_GENERALS_GENERAL_H

#include <string>
#include <vector>
#include "GeneralIdentity.h"
using namespace std;

enum Loyalty{
    loyal, traitor
};

enum Attribution {
    commanding, lieutenant
};

class General {
public:
    GeneralIdentity myID;
    Loyalty loyalty;
    Attribution attribution;
    int numberOfGenerals;
    int numberOfTraitors;

    General(int32_t id, Loyalty loyalty1, Attribution attribution1, int nGenerals, int nTraitors);

    virtual void run() {};

protected:
    vector<string> generalAddresses;
    virtual void discoverGeneralsAddresses() {};

};

struct GeneralAddress{
    GeneralIdentity id;
    int sock;
};

#endif //BYZANTINE_GENERALS_GENERAL_H
