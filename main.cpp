#include <iostream>
#include "main.h"
#include "Strategy.h"
#include "MinMaxStrategy.h"
#include "HeuristicEvaluator.h"
#include "GameAdapter.h"
#include "MCTStrategy.h"

int main()
{
    // Game initialization
    game.initialize(10, Level::MEDIUM_2, Mode::ARENA, false, "Pseudo");

    Evaluator* evaluator = new HeuristicEvaluator();

    while (!game.isAllGameFinish())
    {
        Strategy* myIA = new MCTStrategy(5000, 1.41); //new MinimaxStrategy(4, evaluator);
        GameAdapter adapter(myIA);

        while (!game.isFinish())
        {
            // Get IA move
            GameMove gameMove;
            game.getMove(gameMove);
            std::cerr << "IA adverse joue : " << gameMove.row << " " << gameMove.col << std::endl;

            if (gameMove.row != -1 && gameMove.col != -1) {
                adapter.onOpponentMove(gameMove); // on ignore, à nous de jouer
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
