#ifndef SMALLBOARD_H
#define SMALLBOARD_H

#include <vector>
#include "Cell.h"

class SmallBoard {
private:
    Cell m_cells[9];
    Cell m_winner;

    Cell computeWinner() const;

public:
    SmallBoard();

    bool play(int row, int col, Cell player);

    Cell getCell(int idx)         const { return m_cells[idx]; }
    Cell getCell(int r, int c)    const { return m_cells[r * 3 + c]; }
    Cell getWinner()              const { return m_winner; }
    bool isComplete()             const;
    std::vector<int> getLegalMoves() const;
    SmallBoard clone()            const { return *this; }
};

#endif
