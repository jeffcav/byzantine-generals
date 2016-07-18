//
// Created by jeffrodrigo on 18/07/16.
//

#include <cstring>
#include "Message.h"

void Message::serialize(char *buffer) {
    memcpy(&buffer[0], &source, sizeof(int32_t));
    memcpy(&buffer[4], &message, sizeof(char));
}

int Message::size() {
    return 6;
}

Message::Message(LieutenantID source, Command command) : source(source){

    switch(command) {
        case attack:
            this->message = 'A';
            break;
        case retreat:
            this->message = 'R';
            break;
        case any:
        default:
            this->message = '?';
    }
}

Message::Message(char *buffer) : source(0) {
    memcpy(&source, &buffer[0], sizeof(int32_t));
    memcpy(&message, &buffer[4], sizeof(char));
}







