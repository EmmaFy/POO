#include "MCTStrategy.h"

#include <algorithm>
#include <cmath>
#include <limits>

// ============================================================================
//                               Node
// ============================================================================

MCTStrategy::Node::Node(const UltimateBoard& s, Move m, Cell p, Node* par)
    : state(s)
    , moveFromParent(m)
    , playerWhoPlayed(p)
    , parent(par)
    , wins(0.0)
    , visits(0)
{
    if (!state.isFinished()) {
        untriedMoves = state.getLegalMoves();
    }
}

MCTStrategy::Node::~Node() {
    for (Node* c : children) {
        delete c;
    }
}

// ============================================================================
//                            MCTStrategy
// ============================================================================

MCTStrategy::MCTStrategy(int iterations, double explorationC, double rolloutRandomness)
    : m_iterations(iterations)
    , m_C(explorationC)
    , m_rolloutRandomness(rolloutRandomness)
    , m_rng(std::random_device{}())
{
}

Move MCTStrategy::chooseMove(const UltimateBoard& board) {
    std::vector<Move> legalMoves = board.getLegalMoves();
    if (legalMoves.empty()) {
        return Move(-1, -1);
    }
    if (legalMoves.size() == 1) {
        return legalMoves[0];
    }

    Cell currentPlayer  = board.getCurrentPlayer();
    Cell previousPlayer = opponent(currentPlayer);

    Node* root = new Node(board, Move(-1, -1), previousPlayer, nullptr);

    for (int i = 0; i < m_iterations; ++i) {
        Node* node = select(root);
        if (!node->untriedMoves.empty()) {
            node = expand(node);
        }
        Cell winner = rollout(node->state);
        backpropagate(node, winner);
    }

    Node* best = nullptr;
    int maxVisits = -1;
    for (Node* child : root->children) {
        if (child->visits > maxVisits) {
            maxVisits = child->visits;
            best = child;
        }
    }

    Move chosen = best ? best->moveFromParent : legalMoves[0];
    delete root;
    return chosen;
}

MCTStrategy::Node* MCTStrategy::select(Node* root) {
    Node* node = root;
    while (node->untriedMoves.empty() && !node->children.empty()) {
        node = bestUCTChild(node, m_C);
    }
    return node;
}

MCTStrategy::Node* MCTStrategy::expand(Node* node) {
    std::uniform_int_distribution<int> dist(0, static_cast<int>(node->untriedMoves.size()) - 1);
    int idx = dist(m_rng);
    Move move = node->untriedMoves[idx];

    std::swap(node->untriedMoves[idx], node->untriedMoves.back());
    node->untriedMoves.pop_back();

    UltimateBoard newState = node->state;
    Cell playerWhoPlayed = newState.getCurrentPlayer();
    newState.play(move);

    Node* child = new Node(newState, move, playerWhoPlayed, node);
    node->children.push_back(child);
    return child;
}

Cell MCTStrategy::rollout(UltimateBoard board) {
    while (!board.isFinished()) {
        std::vector<Move> moves = board.getLegalMoves();
        if (moves.empty()) break;
        Move chosen = chooseRolloutMove(board, moves);
        board.play(chosen);
    }
    return board.getWinner();
}


// SÉLECTION DE COUP EN ROLLOUT

Move MCTStrategy::chooseRolloutMove(const UltimateBoard& board,
                                     const std::vector<Move>& legalMoves) {
    // 1. Aléatoire pur avec probabilité m_rolloutRandomness (typiquement 10%)
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    if (probDist(m_rng) < m_rolloutRandomness) {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(legalMoves.size()) - 1);
        return legalMoves[dist(m_rng)];
    }

    Cell player = board.getCurrentPlayer();
    Cell opp    = opponent(player);

    // 2. Si un coup gagne immédiatement une sous-grille → on le prend
    for (const Move& m : legalMoves) {
        int bigIdx   = (m.getRow() / 3) * 3 + (m.getCol() / 3);
        int localPos = (m.getRow() % 3) * 3 + (m.getCol() % 3);
        const SmallBoard& sb = board.getSmallBoard(bigIdx);
        if (wouldCompleteLine(sb, localPos, player)) {
            return m;
        }
    }

    // 3. Si l'adversaire menace de gagner une sous-grille → on bloque
    for (const Move& m : legalMoves) {
        int bigIdx   = (m.getRow() / 3) * 3 + (m.getCol() / 3);
        int localPos = (m.getRow() % 3) * 3 + (m.getCol() % 3);
        const SmallBoard& sb = board.getSmallBoard(bigIdx);
        if (wouldCompleteLine(sb, localPos, opp)) {
            return m;
        }
    }

    // 4. Sinon : sélection PONDÉRÉE par la qualité du forçage
    // Pour chaque coup, on calcule un poids qui prend en compte :
    //  - position dans la sous-grille (centre > coin > bord)
    //  - où le coup envoie l'adversaire (préférable : sous-grille où on a l'avantage)
    //  - éviter d'envoyer l'adversaire vers une sous-grille terminée (choix libre = mauvais)

    static const double POS_BONUS[9] = {
        1.2, 0.9, 1.2,   // coins forts, bords moyens
        0.9, 1.5, 0.9,   // centre très fort
        1.2, 0.9, 1.2
    };

    std::vector<double> weights(legalMoves.size());
    for (size_t i = 0; i < legalMoves.size(); ++i) {
        const Move& m = legalMoves[i];
        int localPos = (m.getRow() % 3) * 3 + (m.getCol() % 3);

        // Bonus de position locale (préférer centre, coins)
        double w = POS_BONUS[localPos];

        // Pénalité/bonus selon où on envoie l'adversaire
        // localPos est aussi l'indice de la sous-grille où l'adversaire sera forcé
        const SmallBoard& targetSb = board.getSmallBoard(localPos);

        if (targetSb.isComplete()) {
            // Cible déjà finie → adversaire aura le choix libre = mauvais pour nous
            w *= 0.4;
        } else {
            // Compter qui domine dans la sous-grille cible
            int myCount = 0, oppCount = 0;
            for (int p = 0; p < 9; ++p) {
                Cell c = targetSb.getCell(p);
                if (c == player) ++myCount;
                else if (c == opp) ++oppCount;
            }
            // Si l'adversaire domine, on évite de l'y envoyer
            if (oppCount > myCount + 1) w *= 0.5;
            else if (oppCount > myCount) w *= 0.8;
            // Si on domine, c'est neutre/légèrement bon (l'opp est piégé mais reste dangereux)
        }

        weights[i] = w;
    }

    // Tirage aléatoire pondéré par les poids calculés
    double totalWeight = 0.0;
    for (double w : weights) totalWeight += w;

    std::uniform_real_distribution<double> wDist(0.0, totalWeight);
    double r = wDist(m_rng);
    double cumulative = 0.0;
    for (size_t i = 0; i < legalMoves.size(); ++i) {
        cumulative += weights[i];
        if (r <= cumulative) return legalMoves[i];
    }
    return legalMoves.back();  // fallback sécurité
}

bool MCTStrategy::wouldCompleteLine(const SmallBoard& sb, int localPos, Cell player) {
    static const int LINES[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };

    for (int li = 0; li < 8; ++li) {
        bool contains = false;
        for (int k = 0; k < 3; ++k) {
            if (LINES[li][k] == localPos) { contains = true; break; }
        }
        if (!contains) continue;

        int countMine = 0;
        bool blocked = false;
        for (int k = 0; k < 3; ++k) {
            int p = LINES[li][k];
            if (p == localPos) continue;
            Cell c = sb.getCell(p);
            if (c == player) ++countMine;
            else if (c != Cell::EMPTY) { blocked = true; break; }
        }
        if (!blocked && countMine == 2) return true;
    }
    return false;
}

void MCTStrategy::backpropagate(Node* node, Cell winner) {
    while (node != nullptr) {
        node->visits++;
        node->wins += scoreFor(winner, node->playerWhoPlayed);
        node = node->parent;
    }
}

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

double MCTStrategy::scoreFor(Cell winner, Cell playerWhoPlayed) {
    if (winner == playerWhoPlayed) return 1.0;
    if (winner == Cell::EMPTY)     return 0.5;
    return 0.0;
}
