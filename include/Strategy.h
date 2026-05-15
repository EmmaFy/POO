#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <vector>

class UltimateBoard;
class Move;

struct Strategy {
    virtual ~Strategy() = default;
    virtual Move chooseMove(const UltimateBoard& board) = 0;
};

#endif
