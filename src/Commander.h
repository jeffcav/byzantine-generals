//
// Created by jeffrodrigo on 19/07/16.
//

#ifndef BYZANTINE_GENERALS_COMMANDING_H
#define BYZANTINE_GENERALS_COMMANDING_H


#include "General.h"
#include "Message.h"

class Commander : public General{
public:
    vector<GeneralAddress> generals;


    Commander(int nGenerals, int nTraitors);

    void run();

protected:
    void discoverGenerals();

private:
    void sendMessage(GeneralAddress general, Message message);
};


#endif //BYZANTINE_GENERALS_COMMANDING_H
