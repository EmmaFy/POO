#ifndef MOVE_H
#define MOVE_H

class Move {
private:
    int m_row;
    int m_col;

public:
    Move() : m_row(-1), m_col(-1) {}
    Move(int row, int col) : m_row(row), m_col(col) {}

    int  getRow() const { return m_row; }
    void setRow(int v)  { m_row = v; }
    int  getCol() const { return m_col; }
    void setCol(int v)  { m_col = v; }

    bool isValid() const { return m_row >= 0 && m_col >= 0; }
};

#endif
