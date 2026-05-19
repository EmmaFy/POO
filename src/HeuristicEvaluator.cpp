#include "HeuristicEvaluator.h"
#include "UltimateBoard.h"
#include "SmallBoard.h"

#include <array>

namespace {
    // Patterns gagnants (les 8 lignes/colonnes/diagonales d'une grille 3x3).
    constexpr std::array<std::array<int, 3>, 8> WIN_PATTERNS = {{
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    }};

    // Pondérations par position dans une grille 3x3.
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

    // Score basé sur la méta-grille
    const SmallBoard& meta = board.getMetaBoard();
    for (int i = 0; i < 9; ++i) {
        Cell owner = meta.getCell(i);
        if (owner == player) {
            score += 50 * POSITION_WEIGHT[i];
        } else if (owner == opponent(player)) {
            score -= 50 * POSITION_WEIGHT[i];
        }
    }

    // Bonus pour les alignements potentiels sur la méta-grille.
    // Multiplicateur 30 : la méta est ce qui détermine la victoire finale.
    score += linePotential(meta, player) * 50;

    // ----- (2) Score basé sur les sous-grilles non finies -----
    for (int bi = 0; bi < 9; ++bi) {
        const SmallBoard& sb = board.getSmallBoard(bi);
        if (sb.isComplete()) continue;

        int boardWeight = POSITION_WEIGHT[bi];

        for (int pos = 0; pos < 9; ++pos) {
            Cell c = sb.getCell(pos);
            if (c == player) {
                score += POSITION_WEIGHT[pos] * boardWeight;
            } else if (c == opponent(player)) {
                score -= POSITION_WEIGHT[pos] * boardWeight;
            }
        }

        score += linePotential(sb, player) * 5;
    }


    // La sous-grille où le prochain joueur EST FORCÉ de jouer compte
    // beaucoup plus que les autres : c'est là que se joue le coup imminent.
    int forcedR = board.getForcedRow();
    int forcedC = board.getForcedCol();
    if (forcedR != -1) {
        int forcedIdx = forcedR * 3 + forcedC;
        const SmallBoard& forcedSb = board.getSmallBoard(forcedIdx);
        Cell nextPlayer = board.getCurrentPlayer();

        if (forcedSb.isComplete()) {
            // Cas particulier : la sous-grille forcée est déjà finie,
            // le prochain joueur a donc le choix libre n'importe où.
            // Avoir le choix libre est un avantage tactique.
            if (nextPlayer == player) score += 30;
            else                      score -= 30;
        } else {
            // Amplification massive de la situation tactique dans la sous-grille forcée.
            // linePotential du point de vue de `player` :
            //   positif si player a des menaces, négatif si opp a des menaces.
            //
            // Si l'adversaire est forcé (nextPlayer == opp) :
            //   menaces de player (positif) -> l'opp doit bloquer = excellent pour nous
            //   menaces de opp (négatif)    -> opp va exécuter immédiatement = catastrophe
            int lp = linePotential(forcedSb, player);

            // Coefficient 20 : plus fort que le multiplicateur 5 standard
            // pour les sous-grilles "normales".
            score += lp * 30;
        }
    }

    return score;
}

int HeuristicEvaluator::linePotential(const SmallBoard& sb, Cell player) const
{
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

        // Une ligne mixte (les deux joueurs ont joué) ne peut plus être gagnée.
        if (myCount > 0 && oppCount > 0) continue;

        // Poids augmentés pour le cas "2 pions + 1 vide" : c'est une menace à 1 coup,
        // ça doit "dominer" le calcul.
        if      (myCount == 2 && emptyCount == 1)   potential += 80;
        else if (myCount == 1 && emptyCount == 2)   potential += 1;
        else if (oppCount == 2 && emptyCount == 1)  potential -= 80;
        else if (oppCount == 1 && emptyCount == 2)  potential -= 1;
    }
    return potential;
}
