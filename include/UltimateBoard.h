#ifndef ULTIMATEBOARD_H
#define ULTIMATEBOARD_H

#include <vector>
#include "SmallBoard.h"
#include "Move.h"

class UltimateBoard
{
    private:
        SmallBoard m_boards;
        SmallBoard m_result; // retourne le résultat pour chaque grille ainsi que le résultat total
        int m_forcedBoards; // 0..8 = sous-grille forcée, -1 = libre
        Cell m_currentPlayer;

    public:
        UltimateBoard();
        ~UltimateBoard();

        // Getter Setter
        int getForcedBoards() { return m_forcedBoards; }
        void setForcedBoards(int val) { m_forcedBoards = val; }
        SmallBoard getBoards() { return m_boards; }
        void setBoards(SmallBoard val) { m_boards = val; }
        SmallBoard getResult() { return m_result; }
        void setResult(SmallBoard val) { m_result = val; }
        Cell getCurrentPlayer() { return m_currentPlayer; }
        void setCurrentPlayer(Cell val) { m_currentPlayer = val; }

        // Methods
        bool play(Move move);
        std::vector<Move> getLegalMoves() const;
        Cell getWinner() const;
        UltimateBoard clone() const;
        bool isFinished() const;
};

#endif // ULTIMATEBOARD_H
