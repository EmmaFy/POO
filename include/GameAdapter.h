#ifndef GAMEADAPTER_H
#define GAMEADAPTER_H

#include "UltimateBoard.h"
#include "IStrategy.h"

class GameAdapter {
private:
    UltimateBoard board;   // L'état actuel du jeu
    IStrategy* strategy;    // Pointeur vers l'IA choisie (Random, Minimax, etc.)

public:
    GameAdapter(Strategy* s);
    void onOpponentMove(Move m);
    Move computeOurMove();
    const UltimateBoard& getBoard() const { return board; }
};

#endif
