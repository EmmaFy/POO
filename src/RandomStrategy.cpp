#include "RandomStrategy.h"

RandomStrategy::RandomStrategy()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

RandomStrategy::~RandomStrategy()
{
    //dtor
}

Move RandomStrategy::RandomStrategy(board)
{
    Move move;
    move.row = std::rand() % 9;
    move.col = std::rand() % 9;
    return move;
}
