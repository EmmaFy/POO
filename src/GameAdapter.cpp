#include "GameAdapter.h"
#include <stdexcept>

GameAdapter::GameAdapter(Strategy* s) : strategy(s) {
}

void GameAdapter::onOpponentMove(GameMove m) {
    // On met à jour notre plateau local avec le coup de l'adversaire
    // pour que l'IA sache où elle en est.
    //board.play(m);
    if (!board.play(m)){
        throw std::invalid_argument("GameAdapter::onOpponentMove: illegal move");
    }
}

GameMove GameAdapter::computeOurMove() {
    //if (strategy == nullptr) {
    //    return {-1, -1}; // Sécurité
    //}

    if (board.isFinished()){
        throw std::runtime_error("GameAdapter::computeOurMove: game is already over");
    }

    GameMove myMove = strategy->chooseMove(board);
    board.play(myMove);

    return myMove;
}

const UltimateBoard& GameAdapter::getBoard() const {
    return board;
}

bool GameAdapter::isGameOver() const {
    return board.isFinished();
}
