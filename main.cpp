#include <iostream>
#include "main.h"
#include "Strategy.h"
#include "MinMaxStrategy.h"
#include "HeuristicEvaluator.h"
#include "GameAdapter.h"

int main()
{
    // Game initialization
    game.initialize(10, Level::EASY_1, Mode::DEBUG, false, "Pseudo");

    Evaluator* evaluator = new HeuristicEvaluator();

    while (!game.isAllGameFinish())
    {
        Strategy* myIA = new MinimaxStrategy(4, evaluator);
        GameAdapter adapter(myIA);

        while (!game.isFinish())
        {
            // Get IA move
            GameMove gameMove;
            if (game.getMove(gameMove))
            {
                std::cerr << "IA adverse joue : " << gameMove.row << " " << gameMove.col << std::endl;
                adapter.onOpponentMove(gameMove);
            }

            // Send your move
            GameMove myMove = adapter.computeOurMove(); // met aussi à jour la grille
            std::cerr << "Envoi du coup : " << myMove.row << " " << myMove.col << std::endl;
            game.setMove(myMove);
        }
        delete myIA;
    }
    delete evaluator;
    return 0;
}
