#ifndef RANDOMSTRATEGY_H
#define RANDOMSTRATEGY_H
#include "Move.h"
#include "Ultimateboard.h"


class RandomStrategy
{
    public:
        RandomStrategy();
        ~RandomStrategy();
        Move chooseMove(UltimateBoard.board);
};

#endif // RANDOMSTRATEGY_H
