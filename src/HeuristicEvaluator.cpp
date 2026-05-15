#include "HeuristicEvaluator.h"
#include "UltimateBoard.h"
#include "SmallBoard.h"

#include <array>

namespace {
    // Patterns gagnants (les 8 lignes/colonnes/diagonales d'une grille 3x3).
    // On les réutilise pour évaluer la méta-grille et les sous-grilles.
    constexpr std::array<std::array<int, 3>, 8> WIN_PATTERNS = {{
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    }};

    // Pondérations par position dans une grille 3x3 :
    // le centre vaut plus que les coins, qui valent plus que les bords.
    constexpr std::array<int, 9> POSITION_WEIGHT = {
        3, 2, 3,
        2, 4, 2,
        3, 2, 3
    };
}

int HeuristicEvaluator::evaluate(const UltimateBoard& board, Cell player) const
{
    // Si la partie est déjà gagnée à l'évaluation, on retourne un score extrême.
    Cell winner = board.getWinner();
    if (winner == player)            return 100000;
    if (winner == opponent(player))  return -100000;

    int score = 0;

    // ----- (1) Score basé sur la méta-grille -----
    // Une sous-grille gagnée vaut beaucoup (poids 50 * position),
    // car c'est ce qui détermine la victoire finale.
    const SmallBoard& meta = board.getMetaBoard();
    for (int i = 0; i < 9; ++i) {
        Cell owner = meta.getCell(i);
        if (owner == player) {
            score += 50 * POSITION_WEIGHT[i];
        } else if (owner == opponent(player)) {
            score -= 50 * POSITION_WEIGHT[i];
        }
    }

    // Bonus pour les alignements potentiels sur la méta-grille
    // (deux sous-grilles gagnées sur une même ligne, troisième encore libre).
    score += linePotential(meta, player) * 30;

    // ----- (2) Score basé sur les sous-grilles non finies -----
    // Dans chaque sous-grille en cours, on regarde les positions occupées
    // par chaque joueur, pondérées par leur valeur stratégique.
    for (int bi = 0; bi < 9; ++bi) {
        const SmallBoard& sb = board.getSmallBoard(bi);
        if (sb.isComplete()) continue;  // déjà gagnée/pleine, déjà comptée plus haut

        // Une sous-grille au centre du grand plateau est elle-même plus précieuse.
        int boardWeight = POSITION_WEIGHT[bi];

        for (int pos = 0; pos < 9; ++pos) {
            Cell c = sb.getCell(pos);
            if (c == player) {
                score += POSITION_WEIGHT[pos] * boardWeight;
            } else if (c == opponent(player)) {
                score -= POSITION_WEIGHT[pos] * boardWeight;
            }
        }

        // Alignements en cours dans cette sous-grille
        score += linePotential(sb, player) * 5;
    }

    return score;
}

int HeuristicEvaluator::linePotential(const SmallBoard& sb, Cell player) const
{
    // Compte les alignements "menaçants" de l'adversaire : 2 cases de l'adversaire + 1 vide
    int potential = 0;
    Cell opp = opponent(player);

    for (const auto& pattern : WIN_PATTERNS) {
        int myCount  = 0;
        int oppCount = 0;
        int emptyCount = 0;

        for (int pos : pattern) {
            Cell c = sb.getCell(pos);
            if      (c == player) ++myCount;
            else if (c == opp)    ++oppCount;
            else                  ++emptyCount;
        }

        // Dans le cas d'une ligne mixte (les deux joueurs ont joué dessus) :
        // elle n'a plus de contribution car elle ne peut plus être gagnée.
        if (myCount > 0 && oppCount > 0) continue;

        if (myCount == 2 && emptyCount == 1)        potential += 5;
        else if (myCount == 1 && emptyCount == 2)   potential += 1;
        else if (oppCount == 2 && emptyCount == 1)  potential -= 5;
        else if (oppCount == 1 && emptyCount == 2)  potential -= 1;
    }
    return potential;
}
