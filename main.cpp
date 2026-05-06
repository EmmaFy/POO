#include <iostream>
#include "main.h"
#include "Strategy.h"
#include "RandomStrategy.h"
#include "GameAdapter.h"

int main()
{
    // Game initialization
    game.initialize(10, Level::EASY_1, Mode::DEBUG, false, "Pseudo");

    while (!game.isAllGameFinish())
    {
        GameMove myMove{0, 0};

        while (!game.isFinish())
        {
            // Get IA move
            GameMove gameMove;
            game.getMove(gameMove);
            std::cerr << "IA move " << gameMove.row << " " << gameMove.col << std::endl;

            // Send your move
            Strategy* myIA = new RandomStrategy();
            GameAdapter adapter(myIA);
            GameMove myMove=RandomStrategy();
            std::cerr << "Send move " << myMove.row << " " << myMove.col << std::endl;
            game.setMove(myMove);
        }
        delete myIA;
    }

    return 0;
}
