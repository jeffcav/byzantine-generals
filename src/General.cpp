//
// Created by jeffrodrigo on 19/07/16.
//

#include <iostream>
#include "General.h"

using namespace std;

General::General(int32_t id, Loyalty loyalty1, Attribution attribution1, int nGenerals, int nTraitors)
        : myID(GeneralIdentity(id)),
          loyalty(loyalty1),
          attribution(attribution1),
          numberOfGenerals(nGenerals),
          numberOfTraitors(nTraitors)
{
    cout << "I am " <<
            (this->loyalty == loyal? "a loyal " : "a traitor ") <<
            (this->attribution == commanding? "commandant" : "lieutenant") <<
            endl;
}





