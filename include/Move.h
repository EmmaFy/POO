#ifndef MOVE_H
#define MOVE_H


class Move
{
    private:
        int m_row;
        int m_col;

    public:
        Move();
        Move(int _row, int _col);
        ~Move();

        int getRow() { return m_row; }
        void setRow(int val) { m_row = val; }
        int getCol() { return m_col; }
        void setCol(int val) { m_col = val; }

};

#endif // MOVE_H
