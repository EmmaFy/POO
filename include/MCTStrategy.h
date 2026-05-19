#ifndef MCT_STRATEGY_H
#define MCT_STRATEGY_H

#include "Strategy.h"
#include "Move.h"
#include "Cell.h"
#include "UltimateBoard.h"
#include "SmallBoard.h"

#include <vector>
#include <random>

/**
 * @brief Stratégie Monte Carlo Tree Search avec rollouts heuristiques.
 *
 * À chaque appel de chooseMove, on construit un arbre de recherche en
 * effectuant N itérations des 4 phases MCTS (selection, expansion, simulation,
 * backpropagation), puis on retourne le coup correspondant à l'enfant le plus
 * visité de la racine.
 *
 * Les rollouts ne sont pas purement aléatoires : ils appliquent des règles
 * tactiques de base (compléter ses propres alignements, bloquer ceux de
 * l'adversaire) pour produire des simulations plus représentatives.
 */
class MCTStrategy : public Strategy {

public:
    /**
     * @param iterations Nombre d'itérations MCTS par coup (3000-10000 typique).
     * @param explorationC Constante d'exploration UCT (~1.41 = sqrt(2)).
     * @param rolloutRandomness Probabilité [0..1] de jouer aléatoire malgré
     *                          les heuristiques (typiquement 0.1).
     */
    MCTStrategy(int iterations = 5000,
                double explorationC = 1.41,
                double rolloutRandomness = 0.1);
    ~MCTStrategy() override = default;

    Move chooseMove(const UltimateBoard& board) override;

private:
    struct Node {
        UltimateBoard state;
        Move moveFromParent;
        Cell playerWhoPlayed;
        Node* parent;
        std::vector<Node*> children;
        std::vector<Move> untriedMoves;
        double wins;
        int visits;

        Node(const UltimateBoard& s, Move m, Cell p, Node* par);
        ~Node();
    };

    int m_iterations;
    double m_C;
    double m_rolloutRandomness;
    std::mt19937 m_rng;

    // Les 4 phases MCTS
    Node* select(Node* root);
    Node* expand(Node* node);
    Cell  rollout(UltimateBoard board);
    void  backpropagate(Node* node, Cell winner);

    // Helpers MCTS
    Node*  bestUCTChild(Node* node, double C) const;
    static double scoreFor(Cell winner, Cell playerWhoPlayed);

    // Helpers pour les rollouts heuristiques
    Move chooseRolloutMove(const UltimateBoard& board,
                           const std::vector<Move>& legalMoves);
    static bool wouldCompleteLine(const SmallBoard& sb, int localPos, Cell player);
};

#endif // MCT_STRATEGY_H
