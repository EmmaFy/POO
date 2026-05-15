#include "SmallBoard.h"

SmallBoard::SmallBoard()
{
    //ctor
}

SmallBoard::~SmallBoard()
{
    //dtor
}

bool SmallBoard::play(GameMove m, Cell p) {
    return false;
}

Cell SmallBoard::getWinner() const {
    return winner;
}

bool SmallBoard::isComplete() const {
    return false;
}

std::vector<int> SmallBoard::getLegalMoves() const {
    return std::vector<int>();
}

SmallBoard SmallBoard::clone() const {
    return SmallBoard();
}
