//
// Created by jeffrodrigo on 18/07/16.
//

#include <string>
#include "LieutenantID.h"

#ifndef BYZANTINE_GENERALS_MESSAGE_H
#define BYZANTINE_GENERALS_MESSAGE_H

enum Command {
    attack, retreat, any
};

class Message {
public:
    LieutenantID source;
    char message;

public:
    Message(LieutenantID source, Command command);
    Message(char *buffer);

    void serialize(char *buffer);

    int size();
};


#endif //BYZANTINE_GENERALS_MESSAGE_H
