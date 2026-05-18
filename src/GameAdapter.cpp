#include "GameAdapter.h"
#include <stdexcept>

GameAdapter::GameAdapter(Strategy* s) : strategy(s) {
}

void GameAdapter::onOpponentMove(GameMove m) {
    Move internalMove(m.row, m.col);

    if (!board.play(internalMove)){
        throw std::invalid_argument("GameAdapter::onOpponentMove: illegal move");
    }
}

GameMove GameAdapter::computeOurMove() {
    if (board.isFinished()){
        throw std::runtime_error("GameAdapter::computeOurMove: game is already over");
    }

    // IA génère un move interne
    Move chosenMove = strategy->chooseMove(board);

    // on joue ce move sur le plateau local de l'adaptateur
    board.play(chosenMove);

    // conversion du move interne vers structure GameMove
    GameMove myMove{ chosenMove.getRow(), chosenMove.getCol() };

    return myMove;
}

const UltimateBoard& GameAdapter::getBoard() const {
    return board;
}

bool GameAdapter::isGameOver() const {
    return board.isFinished();
}
