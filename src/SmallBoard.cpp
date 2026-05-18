#include "SmallBoard.h"

static const int WIN_PAT[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8},
    {0,3,6}, {1,4,7}, {2,5,8},
    {0,4,8}, {2,4,6}
};

SmallBoard::SmallBoard() : m_winner(Cell::EMPTY) {
    for (int i = 0; i < 9; ++i) m_cells[i] = Cell::EMPTY;
}

Cell SmallBoard::computeWinner() const {
    for (auto& p : WIN_PAT)
        if (m_cells[p[0]] != Cell::EMPTY &&
            m_cells[p[0]] == m_cells[p[1]] &&
            m_cells[p[1]] == m_cells[p[2]])
            return m_cells[p[0]];
    return Cell::EMPTY;
}

bool SmallBoard::play(int row, int col, Cell player) {
    int idx = row * 3 + col;
    if (m_winner != Cell::EMPTY || m_cells[idx] != Cell::EMPTY) return false;
    m_cells[idx] = player;
    m_winner = computeWinner();
    return true;
}

bool SmallBoard::isComplete() const {
    if (m_winner != Cell::EMPTY) return true;
    for (int i = 0; i < 9; ++i)
        if (m_cells[i] == Cell::EMPTY) return false;
    return true;
}

std::vector<int> SmallBoard::getLegalMoves() const {
    std::vector<int> moves;
    if (m_winner != Cell::EMPTY) return moves;
    for (int i = 0; i < 9; ++i)
        if (m_cells[i] == Cell::EMPTY) moves.push_back(i);
    return moves;
}
