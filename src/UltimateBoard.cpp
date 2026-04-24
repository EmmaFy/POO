#include "UltimateBoard.h"

UltimateBoard::UltimateBoard()
{
    //ctor
}

UltimateBoard::~UltimateBoard()
{
    //dtor
}

bool UltimateBoard::play(Move move) {
    return false;
}

std::vector<Move> UltimateBoard::getLegalMoves() const {
    return std::vector<Move>();
}

Cell UltimateBoard::getWinner() const {
    return Cell::EMPTY;
}

UltimateBoard UltimateBoard::clone() const {
    return UltimateBoard();
}

bool UltimateBoard::isFinished() const {
    return false;
}
