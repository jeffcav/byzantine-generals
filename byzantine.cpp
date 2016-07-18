//
// Created by jeffrodrigo on 18/07/16.
//
#include <iostream>
#include "Lieutenant.h"
using namespace std;

int main(){
    int nLieutenants = 3;
    int nTraitors = 1;
    int myID = 2;

    Lieutenant me(myID, nTraitors);
    me.run();

    return 0;
}
