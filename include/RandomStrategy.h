#ifndef RANDOMSTRATEGY_H
#define RANDOMSTRATEGY_H
#include "Move.h"


class RandomStrategy
{
    public:
        RandomStrategy();
        ~RandomStrategy();
        Move chooseMove(board);
};

#endif // RANDOMSTRATEGY_H
