//
// Created by jeffrodrigo on 18/07/16.
//
#include <iostream>
#include "Lieutenant.h"
#include "Commandant.h"

using namespace std;

static General* chooseGeneral(int32_t id, int nGenerals, int nTraitors)
{
    if (id == 0)
        return new Commandant(nGenerals, nTraitors);
    return new Lieutenant(id, nGenerals, nTraitors);

}

int main(int argc, char *argv[])
{
    int nGenerals = 4;
    int nTraitors = 1;
    int32_t myID = atoi(argv[1]);

    General *me = chooseGeneral(myID, nGenerals, nTraitors);
    me->run();

    free(me);

    return 0;
}
