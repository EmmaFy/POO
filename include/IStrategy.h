#ifndef ISTRATEGY_H
#define ISTRATEGY_H


class IStrategy
{
    public:
        IStrategy();
        ~IStrategy();

        Move chooseMove(board);

    protected:

    private:
};

#endif // ISTRATEGY_H
