#ifndef SMALLBOARD_H
#define SMALLBOARD_H


class SmallBoard
{
    private:
        Cells m_cells;
        Cells m_winner;

    public:
        SmallBoard();
        ~SmallBoard();

        Cells getCells() { return m_cells; }
        void setCells(Cells val) { m_cells = val; }
        Cells getWinner() { return m_winner; }
        void setWinner(Cells val) { m_winner = val; }
};

#endif // SMALLBOARD_H
