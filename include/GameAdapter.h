#pragma once

#ifndef GAMEADAPTER_H
#define GAMEADAPTER_H

#include "UltimateBoard.h"
#include "Strategy.h"
#include "Move.h"
#include "../main.h"

class GameAdapter {
private:
    UltimateBoard board;   // L'état actuel du jeu
    Strategy* strategy;    // Pointeur vers l'IA choisie (Random, Minimax ou Monte Carlo)

public:
    GameAdapter(Strategy* s);

    // Getter Setter
    //UltimateBoard getBoard() const;
    //void setBoard(UltimateBoard board);

    // Methods
    void onOpponentMove(GameMove m);
    GameMove computeOurMove();
    const UltimateBoard& getBoard() const;
    bool isGameOver() const;
};

#endif
