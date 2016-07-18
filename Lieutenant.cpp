//
// Created by jeffrodrigo on 18/07/16.
//

#include "Lieutenant.h"

Lieutenant::Lieutenant(int id, Loyalty loyalty, Attribution attribution) {
    this->id.name = id;
    this->attribution = attribution;
    this->loyalty = loyalty;
}

Lieutenant::Lieutenant(int myID, int nTraitors) {
    //ID
    this->id.name = myID;

    //Attribution
    if (myID == 0)
        this->attribution = source;
    else
        this->attribution = common;

    //Loyalty
    if (myID < nTraitors)
        this->loyalty = traitor;
    else
        this->loyalty = loyal;
}


void Lieutenant::run() {

}

