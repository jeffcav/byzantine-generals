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
    //pathLen = htons(pathLen);
    memcpy(&buffer[0], (char*) &pathLen, 2);
    //pathLen = ntohs(pathLen);

    pos = 2;
    for (uint16_t i = 0; i < pathLen; i++) {
        uint16_t id = (uint16_t) path[i].name;
        memcpy(&buffer[pos], &id, 2);

        pos += 2;
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
    return (int) (2 + (2 * this->path.size()) + 1);
}

Message::Message(GeneralIdentity source, Command command) : command(command){
    path.push_back(source);
}

Message::Message(char *buffer) {
    uint16_t id;
    char cmd;
    uint16_t pathLen;

    std::cout << "starting conversion" << endl;

    memcpy(&pathLen, &buffer[0], 2);

    std::cout << "path len gone \n";

    int pos = 2;
    for (uint16_t i = 0; i < pathLen; i++) {
        memcpy(&id, &buffer[pos], 2);
        pos += 2;

        this->path.push_back(GeneralIdentity((uint32_t)id));
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

string Message::printCommand() {
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

    for (int i = 0; i < this->path.size(); i++) {
        result += to_string(path[i].name);
        result += "->";
    }

    result += ": ";
    result += printCommand();

    return result;
}







