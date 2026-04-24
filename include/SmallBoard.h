#ifndef SMALLBOARD_H
#define SMALLBOARD_H

#include <vector>
#include "Cell.h"

class Move;

class SmallBoard
{
    private:
        Cells m_cells;
        Cells m_winner;

    public:
        SmallBoard();
        ~SmallBoard();

        // Getter Setter
        Cells getCells() { return m_cells; }
        void setCells(Cells val) { m_cells = val; }
        Cells getWinner() { return m_winner; }
        void setWinner(Cells val) { m_winner = val; }

        // Methods
        bool play(GameMove move, Cell p);
        Cell getWinner() const;
        bool isComplete() const;
        std::vector<int> getLegalMoves() const;
        SmallBoard clone() const;
};

#endif // SMALLBOARD_H
