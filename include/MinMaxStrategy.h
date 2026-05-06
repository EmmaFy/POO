class Evaluator;  // forward declaration

class MinimaxStrategy : public Strategy {

private:

    int m_maxDepth;
    Evaluator* m_evaluator;
    int alphaBeta(UltimateBoard& board, int depth, int alpha, int beta);

public:

    MinimaxStrategy(int maxDepth, Evaluator* evaluator);
    Move chooseMove(const UltimateBoard& board) override;
};
