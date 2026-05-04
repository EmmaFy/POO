#ifndef STRATEGY_H
#define STRATEGY_H

#include "Move.h"

class UltimateBoard; // Forward declaration pour éviter les inclusions croisées

struct IStrategy {
    virtual ~IStrategy();
    virtual Move chooseMove(const UltimateBoard& board) = 0;
};

#endif
