//
// Created by jeffrodrigo on 18/07/16.
//
int BYZ_RUNLOCAL = 0;

#include <iostream>
#include "Lieutenant.h"
#include "Commander.h"

using namespace std;



static General* chooseGeneral(int32_t id, int nGenerals, int nTraitors)
{
    if (id == 0)
        return new Commander(nGenerals, nTraitors);
    return new Lieutenant(id, nGenerals, nTraitors);

}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        cout << "Expected arguments: general_ID:int numGenerals:int numTraitors:int [l]"
             << endl
             << "l means local - single machine."
             << endl;
    }


    int32_t myID = atoi(argv[1]);
    int nGenerals = atoi(argv[2]);
    int nTraitors = atoi(argv[3]);

    if (argc == 5 && argv[4][0] == 'l') {
        BYZ_RUNLOCAL = 1;
        cout << "Running local\n";
    }
    else {
        BYZ_RUNLOCAL = 0;
        cout << "Running remote\n";
    }



    General *me = chooseGeneral(myID, nGenerals, nTraitors);
    me->run();

    free(me);

    return 0;
}
