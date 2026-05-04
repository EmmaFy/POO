#include "RandomStrategy.h"
#include <vector>
#include <random>
#include <algorithm>

RandomStrategy::RandomStrategy()
{

}

RandomStrategy::~RandomStrategy()
{
    //dtor
}

Move RandomStrategy::chooseMove(const UltimateBoard& board) override
{
        std::vector<Move> legalMoves = board.getLegalMoves();

        // Sécurité : si aucun coup n'est possible, on retourne un coup par défaut
        if (legalMoves.empty()) {
            return Move{-1, -1};
        }

        // 2. Initialiser le générateur de nombres aléatoires
        // static pour ne pas le réinitialiser à chaque appel
        static std::random_device rd;
        static std::mt19937 gen(rd());

        // 3. Définir une distribution sur l'index du vecteur
        std::uniform_int_distribution<> dis(0, legalMoves.size() - 1);

        return legalMoves[dis(gen)];
    }
