#include "MCTStrategy.h"

#include <algorithm>
#include <cmath>
#include <limits>

MCTStrategy::Node::Node(const UltimateBoard& s, Move m, Cell p, Node* par)
    : state(s)
    , moveFromParent(m)
    , playerWhoPlayed(p)
    , parent(par)
    , wins(0.0)
    , visits(0)
{
    // On ne calcule les coups légaux que si la partie n'est pas finie.
    // Sinon untriedMoves reste vide et le nœud est terminal.
    if (!state.isFinished()) {
        untriedMoves = state.getLegalMoves();
    }
}

MCTStrategy::Node::~Node() {
    // Destruction récursive : on libère tout l'arbre depuis cette racine.
    for (Node* c : children) {
        delete c;
    }
}

MCTStrategy::MCTStrategy(int iterations, double explorationC)
    : m_iterations(iterations)
    , m_C(explorationC)
    , m_rng(std::random_device{}())  // seed non déterministe
{
}

Move MCTStrategy::chooseMove(const UltimateBoard& board) {
    // Gestion des cas dégénérés
    std::vector<Move> legalMoves = board.getLegalMoves();
    if (legalMoves.empty()) {
        return Move(-1, -1);  // ne devrait pas arriver si l'adapter check isFinished
    }
    if (legalMoves.size() == 1) {
        return legalMoves[0];  // pas la peine de chercher s'il n'y a qu'un coup
    }

    // Création de la racine. Le "joueur qui a joué pour atteindre la racine"
    // est l'opposant du joueur courant, puisque c'est notre
    // tour maintenant. Cette valeur n'est utilisée que pour la backpropagation
    // au niveau racine, mais elle assure la cohérence de l'algorithme.
    Cell currentPlayer  = board.getCurrentPlayer();
    Cell previousPlayer = opponent(currentPlayer);

    Node* root = new Node(board, Move(-1, -1), previousPlayer, nullptr);

    // Boucle principale MCTS
    for (int i = 0; i < m_iterations; ++i) {
        //  Selection
        Node* node = select(root);

        // Expansion (si possible)
        if (!node->untriedMoves.empty()) {
            node = expand(node);
        }

        // Simulation
        Cell winner = rollout(node->state);

        // Backpropagation
        backpropagate(node, winner);
    }

    // on prend l'enfant le plus VISITÉ de la racine
    // (statistiquement plus robuste que le meilleur taux de victoires)
    Node* best = nullptr;
    int maxVisits = -1;
    for (Node* child : root->children) {
        if (child->visits > maxVisits) {
            maxVisits = child->visits;
            best = child;
        }
    }

    Move chosen = best ? best->moveFromParent : legalMoves[0];

    // Libération de tout l'arbre
    delete root;

    return chosen;
}

// Selection — descend dans l'arbre selon UCT
MCTStrategy::Node* MCTStrategy::select(Node* root) {
    Node* node = root;
    // On descend tant que :
    //  - tous les coups du nœud ont été explorés (pas de untriedMoves)
    //  - ET le nœud a des enfants (n'est pas une feuille terminale)
    while (node->untriedMoves.empty() && !node->children.empty()) {
        node = bestUCTChild(node, m_C);
    }
    return node;
}

// Expansion — ajoute un nouvel enfant pour un coup non exploré
MCTStrategy::Node* MCTStrategy::expand(Node* node) {
    // Choix aléatoire d'un coup non encore exploré
    std::uniform_int_distribution<int> dist(0, static_cast<int>(node->untriedMoves.size()) - 1);
    int idx = dist(m_rng);
    Move move = node->untriedMoves[idx];

    // Retire ce coup de la liste (swap-pop : O(1) au lieu de O(n))
    std::swap(node->untriedMoves[idx], node->untriedMoves.back());
    node->untriedMoves.pop_back();

    // Construit l'état après le coup
    UltimateBoard newState = node->state;
    Cell playerWhoPlayed = newState.getCurrentPlayer();  // celui qui joue ICI
    newState.play(move);

    // Crée l'enfant
    Node* child = new Node(newState, move, playerWhoPlayed, node);
    node->children.push_back(child);
    return child;
}

// Simulation (rollout) — partie aléatoire jusqu'à la fin
Cell MCTStrategy::rollout(UltimateBoard board) {
    // Note : board est passé par valeur (copie), on peut le modifier librement.
    // On joue des coups aléatoires jusqu'à ce que la partie soit terminée.
    while (!board.isFinished()) {
        std::vector<Move> moves = board.getLegalMoves();
        if (moves.empty()) break;  // sécurité (ne devrait pas arriver)

        std::uniform_int_distribution<int> dist(0, static_cast<int>(moves.size()) - 1);
        board.play(moves[dist(m_rng)]);
    }
    return board.getWinner();  // Cell::X, Cell::O ou Cell::EMPTY (nul)
}

// Backpropagation — remonte le résultat le long du chemin
void MCTStrategy::backpropagate(Node* node, Cell winner) {
    while (node != nullptr) {
        node->visits++;
        node->wins += scoreFor(winner, node->playerWhoPlayed);
        node = node->parent;
    }
}

// Sélection UCT : retourne l'enfant maximisant exploitation + exploration
MCTStrategy::Node* MCTStrategy::bestUCTChild(Node* node, double C) const {
    Node* best = nullptr;
    double bestScore = -std::numeric_limits<double>::infinity();

    double lnParentVisits = std::log(static_cast<double>(node->visits));

    for (Node* child : node->children) {
        double exploitation = child->wins / static_cast<double>(child->visits);
        double exploration  = C * std::sqrt(lnParentVisits / static_cast<double>(child->visits));
        double uct = exploitation + exploration;

        if (uct > bestScore) {
            bestScore = uct;
            best = child;
        }
    }
    return best;
}

// Score
double MCTStrategy::scoreFor(Cell winner, Cell playerWhoPlayed) {
    if (winner == playerWhoPlayed) return 1.0;  // victoire
    if (winner == Cell::EMPTY)     return 0.5;  // nul
    return 0.0;                                  // défaite
}
