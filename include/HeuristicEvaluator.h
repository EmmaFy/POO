#ifndef HEURISTIC_EVALUATOR_H
#define HEURISTIC_EVALUATOR_H

#include "Evaluator.h"

class SmallBoard;

class HeuristicEvaluator : public Evaluator {

private:
    int linePotential(const SmallBoard& sb, Cell player) const;

public:
    int evaluate(const UltimateBoard& board, Cell player) const override;
};

#endif
