//
// Created by jeffrodrigo on 19/07/16.
//

#ifndef BYZANTINE_GENERALS_COMMANDING_H
#define BYZANTINE_GENERALS_COMMANDING_H


#include "General.h"
#include "Message.h"

class Commandant : public General{
public:
    Commandant(int nGenerals, int nTraitors);

    void run();

protected:
    void discoverGeneralsAddresses();

private:
    void sendMessage(string ltAddress, Message message);
};


#endif //BYZANTINE_GENERALS_COMMANDING_H
