#ifndef ULTIMATEBOARD_H
#define ULTIMATEBOARD_H

#include <vector>
#include "SmallBoard.h"
#include "Move.h"

class UltimateBoard
{
    private:
        SmallBoard m_boards[3][3];
        SmallBoard m_result; // retourne le résultat pour chaque grille ainsi que le résultat total
        int m_forcedRow;     // -1 = libre
        int m_forcedCol;
        Cell m_currentPlayer;

    public:
        UltimateBoard();

        bool play(Move move);
        std::vector<Move> getLegalMoves()  const;
        Cell getWinner() const;
        bool isFinished() const;
        UltimateBoard clone() const { return *this; }

        Cell getCurrentPlayer() const { return m_currentPlayer; }
        int getForcedRow() const { return m_forcedRow; }
        int getForcedCol() const { return m_forcedCol; }

        // Accesseurs pour HeuristicEvaluator
        const SmallBoard& getSmallBoard(int bi) const { return m_boards[bi / 3][bi % 3]; }
        const SmallBoard& getSmallBoard(int r, int c) const { return m_boards[r][c]; }
        const SmallBoard& getMetaBoard() const { return m_result; }
};

#endif
