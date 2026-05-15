#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "Cell.h"

class UltimateBoard;

class Evaluator {

public:
    virtual ~Evaluator() = default;
    virtual int evaluate(const UltimateBoard& board, Cell player) const = 0;
};

#endif
