#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <vector>

// Forward declarations pour éviter les inclusions circulaires
class UltimateBoard;
struct Move;

struct Strategy {
    // Destructeur virtuel indispensable pour l'héritage
    virtual ~Strategy() = default;
    virtual Move chooseMove(const UltimateBoard& board) = 0;
};

#endif
