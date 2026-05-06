#ifndef HEURISTICEVALUATOR_H
#define HEURISTICEVALUATOR_H


class HeuristicEvaluator
{

    private:
        int m_weights[];

    public:
        HeuristicEvaluator();
        ~HeuristicEvaluator();

        int getweights[]() { return m_weights[]; }
        void setweights[](int val) { m_weights[] = val; }
};

#endif // HEURISTICEVALUATOR_H
