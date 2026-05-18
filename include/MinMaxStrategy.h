#ifndef MINIMAX_STRATEGY_H
#define MINIMAX_STRATEGY_H

#include "../include/Strategy.h"
#include "Cell.h"

class Evaluator;

class MinimaxStrategy : public Strategy {

private:
    int maximize(UltimateBoard& board, int depth, int alpha, int beta);
    int minimize(UltimateBoard& board, int depth, int alpha, int beta);
    int terminalScore(const UltimateBoard& board) const;

    int m_maxDepth;
    Evaluator* m_evaluator;  // injecté, pas possédé
    Cell m_rootPlayer;       // le joueur concerné

public:
    MinimaxStrategy(int maxDepth, Evaluator* evaluator);
    Move chooseMove(const UltimateBoard& board) override;
};

#endif
