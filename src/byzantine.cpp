//
// Created by jeffrodrigo on 18/07/16.
//
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
    int32_t myID = atoi(argv[1]);
    int nGenerals = atoi(argv[2]);
    int nTraitors = atoi(argv[3]);

    General *me = chooseGeneral(myID, nGenerals, nTraitors);
    me->run();

    free(me);

    return 0;
}
