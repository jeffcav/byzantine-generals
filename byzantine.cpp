//
// Created by jeffrodrigo on 18/07/16.
//
#include <iostream>
#include "Lieutenant.h"
using namespace std;

int main(int argc, char *argv[]){
    int nLieutenants = 3;
    int nTraitors = 1;
    int myID = atoi(argv[1]);

    Lieutenant me(LieutenantID(myID), nLieutenants, nTraitors);
    me.run();

    return 0;
}
