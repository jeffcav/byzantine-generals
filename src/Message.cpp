//
// Created by jeffrodrigo on 18/07/16.
//

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include "Message.h"

void Message::serialize(char *buffer) {
    char c;
    int pos;
    uint16_t pathLen;

    pathLen = (uint16_t) this->path.size();
    memcpy(&buffer[0], (char*) &pathLen, 2);

    pos = 2;
    for (uint16_t i = 0; i < pathLen; i++) {
        uint32_t id = path[i].name;
        memcpy(&buffer[pos], &id, 4);

        pos += 4;
    }

    switch (command) {
        case attack:
            c = 1;
            break;
        default:
            c = 0;
    }

    memcpy(&buffer[pos], &c, 1);
}

int Message::size() {
    return (int) (2 + (4 * this->path.size()) + 1);
}

Message::Message(GeneralIdentity source, Command command) : command(command){
    path.push_back(source);
}

Message::Message(char *buffer) {
    uint32_t id;
    char cmd;
    uint16_t pathLen;

    memcpy((char*) &pathLen, &buffer[0], 2);

    int pos = 2;
    for (uint16_t i = 0; i < pathLen; i++) {
        memcpy(&id, &buffer[pos], 4);
        pos += 4;

        this->path.push_back(GeneralIdentity(id));
    }

    memcpy(&cmd, &buffer[pos], 1);

    switch (cmd) {
        case 1:
            this->command = attack;
            break;
        default:
            this->command = retreat;
    }
}

string Message::commandAsString() {
    switch(this->command) {
        case attack:
            return "Attack";
        case retreat:
        default:
            return "Retreat";
    }
}

Message::Message() {

}

string Message::toString() {
    string result = "";

    for (int i = 0; i < path.size(); i++) {
        if (i != 0)
            result += "->";

        result += to_string(path[i].name);
    }

    result += ": ";
    result += commandAsString();

    return result;
}

void Message::appendSource(GeneralIdentity id) {
    this->path.push_back(id);
}







