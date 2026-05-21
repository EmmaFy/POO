#ifndef MCT_STRATEGY_H
#define MCT_STRATEGY_H

#include "Strategy.h"
#include "Move.h"
#include "Cell.h"
#include "UltimateBoard.h"

#include <vector>
#include <random>

/**
 * @brief Stratégie Monte Carlo Tree Search.
 *
 * À chaque appel de chooseMove, on construit un arbre de recherche en
 * effectuant N itérations des 4 phases MCTS (selection, expansion, simulation,
 * backpropagation), puis on retourne le coup correspondant à l'enfant le plus
 * visité de la racine.
 */
class MCTStrategy : public Strategy {

public:
    /**
     * @param iterations Nombre d'itérations MCTS par coup (typique: 5000-20000).
     * Plus c'est élevé, plus l'IA est forte mais lente.
     * @param explorationC Constante d'exploration UCT (typique: sqrt(2) ~ 1.41).
     * Plus c'est élevé, plus on explore (vs exploiter).
     */
    MCTStrategy(int iterations = 5000, double explorationC = 1.41);
    ~MCTStrategy() override = default;

    Move chooseMove(const UltimateBoard& board) override;

private:
    /**
     * Un nœud de l'arbre MCTS. Chaque nœud représente un état du plateau
     * atteint après qu'un certain joueur a fait un certain coup.
     */
    struct Node {
        UltimateBoard state;            // état du plateau à ce nœud
        Move moveFromParent;            // le coup qui a mené ici
        Cell playerWhoPlayed;           // qui a fait ce coup
        Node* parent;                   // nul pour la racine
        std::vector<Node*> children;    // enfants déjà créés
        std::vector<Move> untriedMoves; // coups légaux pas encore explorés
        double wins;                    // somme des scores (1.0 victoire, 0.5 nul, 0.0 défaite)
        int visits;                     // nombre de simulations passant par ce nœud

        Node(const UltimateBoard& s, Move m, Cell p, Node* par);
        ~Node();  // détruit récursivement tous les enfants
    };

    int m_iterations;
    double m_C;
    std::mt19937 m_rng;  // générateur pseudo-aléatoire pour expansion et rollouts

    // Les 4 phases MCTS
    Node* select(Node* root);
    Node* expand(Node* node);
    Cell  rollout(UltimateBoard board);
    void  backpropagate(Node* node, Cell winner);

    // Helpers
    Node*  bestUCTChild(Node* node, double C) const;
    static double scoreFor(Cell winner, Cell playerWhoPlayed);
};

#endif // MCT_STRATEGY_H
