#ifndef RANDOMSTRATEGY_H
#define RANDOMSTRATEGY_H

#include "Strategy.h"
#include "Move.h"

class RandomStrategy : public  Strategy
{
    public:
        RandomStrategy();
        ~RandomStrategy();
        Move chooseMove(const UltimateBoard& board);
};

#endif // RANDOMSTRATEGY_H
