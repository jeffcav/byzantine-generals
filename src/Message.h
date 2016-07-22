//
// Created by jeffrodrigo on 18/07/16.
//

#include <string>
#include "GeneralIdentity.h"

using namespace std;

#ifndef BYZANTINE_GENERALS_MESSAGE_H
#define BYZANTINE_GENERALS_MESSAGE_H

enum Command {
    attack, retreat
};

class Message {
public:
    GeneralIdentity source;
    Command command;

public:
    Message(GeneralIdentity source, Command command);
    Message(char *buffer);
    Message() : source(GeneralIdentity(0)){};

    void serialize(char *buffer);

    int size();
    string printCommand();
};


#endif //BYZANTINE_GENERALS_MESSAGE_H
