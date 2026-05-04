#include "GameAdapter.h"

GameAdapter::GameAdapter(Strategy* s) : strategy(s) {
}

void GameAdapter::onOpponentMove(Move m) {
    // On met à jour notre plateau local avec le coup de l'adversaire
    // pour que l'IA sache où elle en est.
    board.play(m);
}

Move GameAdapter::computeOurMove() {
    if (strategy == nullptr) {
        return {-1, -1}; // Sécurité
    }

    Move myMove = strategy->chooseMove(board);
    board.play(myMove);

    return myMove;
}
