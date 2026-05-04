#ifndef GAMEADAPTER_H
#define GAMEADAPTER_H


class GameAdapter
{
    public:
        GameAdapter();
        ~GameAdapter();

        UltimateBoard Getboard() { return board; }
        void Setboard(UltimateBoard val) { board = val; }
        IStrategy* Getstrategy() { return strategy; }
        void Setstrategy(IStrategy* val) { strategy = val; }

        void onOpponentMove(GameMove);
        GameMove ComputeOurMove();

    protected:

    private:
        UltimateBoard board;
        IStrategy* strategy;
};

#endif // GAMEADAPTER_H
