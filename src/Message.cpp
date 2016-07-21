//
// Created by jeffrodrigo on 18/07/16.
//

#include <cstring>
#include "Message.h"

void Message::serialize(char *buffer) {
    memcpy(&buffer[0], &source, 4);
    memcpy(&buffer[4], &command, 4);
}

int Message::size() {
    return 6;
}

Message::Message(GeneralIdentity source, Command command) : source(source), command(command){

}

Message::Message(char *buffer) : source(0) {
    memcpy(&source, &buffer[0], 4);
    memcpy(&command, &buffer[4], 4);
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







