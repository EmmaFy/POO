#ifndef RANDOMSTRATEGY_H
#define RANDOMSTRATEGY_H

class RandomStrategy : public IStrategy
{
    public:
        RandomStrategy();
        ~RandomStrategy();
        Move chooseMove(const UltimateBoard& board)
};

#endif // RANDOMSTRATEGY_H
