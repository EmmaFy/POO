#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "Move.h"

class UltimateBoard;

class Strategy {
    public :
        virtual ~Strategy() = default;
        virtual Move chooseMove(const UltimateBoard& board) = 0;
};

#endif
