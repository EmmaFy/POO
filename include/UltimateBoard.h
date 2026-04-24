#ifndef ULTIMATEBOARD_H
#define ULTIMATEBOARD_H


class UltimateBoard
{
    private:
        SmallBoard m_boards;
        SmallBoard m_result;
        int m_forcedBoards;
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
};

#endif // ULTIMATEBOARD_H
