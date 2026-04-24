#ifndef SMALLBOARD_H
#define SMALLBOARD_H

#include <vector>
#include "Cell.h"

class GameMove;

class SmallBoard
{
    private:
        Cell m_cells;
        Cell m_winner;

    public:
        SmallBoard();
        ~SmallBoard();

        // Getter Setter
        Cell getCells() { return m_cells; }
        void setCells(Cell val) { m_cells = val; }
        Cell getWinner() { return m_winner; }
        void setWinner(Cell val) { m_winner = val; }

        // Methods
        bool play(GameMove move, Cell p);
        Cell getWinner() const;
        bool isComplete() const;
        std::vector<int> getLegalMoves() const;
        SmallBoard clone() const;
};

#endif // SMALLBOARD_H
