//
// Created by jeffrodrigo on 18/07/16.
//

#include <string>
#include <vector>
#include "GeneralIdentity.h"

using namespace std;

#ifndef BYZANTINE_GENERALS_MESSAGE_H
#define BYZANTINE_GENERALS_MESSAGE_H

#define MSG_MAXBUFLEN 16387

/* Messages format
 *
 * 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
 * +-----------------------------+
 * |          path length        |
 * +-----------------------------+
 * |            ID 1             |
 * +-----------  ...  -----------+
 * |            ID N             |
 * +-------------+---------------+
 * |   command   |
 * +-------------+
 *
 */

enum Command {
    attack, retreat
};

class Message {
public:
    vector<GeneralIdentity> path;
    Command command;

public:
    Message(GeneralIdentity source, Command command);
    Message(char *buffer);
    Message();

    void serialize(char *buffer);

    int size();
    string printCommand();
    string toString();
};


#endif //BYZANTINE_GENERALS_MESSAGE_H
