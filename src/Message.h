//
// Created by jeffrodrigo on 18/07/16.
//

#include <string>
#include "GeneralIdentity.h"

#ifndef BYZANTINE_GENERALS_MESSAGE_H
#define BYZANTINE_GENERALS_MESSAGE_H

enum Command {
    attack, retreat, any
};

class Message {
public:
    GeneralIdentity source;
    char message;

public:
    Message(GeneralIdentity source, Command command);
    Message(char *buffer);

    void serialize(char *buffer);

    int size();
};


#endif //BYZANTINE_GENERALS_MESSAGE_H
