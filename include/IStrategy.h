#ifndef ISTRATEGY_H
#define ISTRATEGY_H

#include "UltimateBoard.h"
#include "Move.h"


class IStrategy
{
    public:
        IStrategy();
        ~IStrategy();

        Move chooseMove(board);

    protected:

    private:
};

#endif // ISTRATEGY_H
