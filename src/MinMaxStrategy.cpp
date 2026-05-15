#include "../include/MinMaxStrategy.h"
#include "../include/UltimateBoard.h"
#include "../include/Evaluator.h"

#include <algorithm>
#include <limits>

// Constantes : un score "infini" représentant une victoire/défaite certaine.
// On utilise une grande valeur mais pas  la limite (std::numeric_limits<int>::max())
// pour éviter les overflow lors des comparaisons avec ±1.
namespace {
    constexpr int WIN_SCORE  =  1000000;
    constexpr int LOSS_SCORE = -1000000;
}

MinimaxStrategy::MinimaxStrategy(int maxDepth, Evaluator* evaluator)
    : m_maxDepth(maxDepth)
    , m_evaluator(evaluator)
    , m_rootPlayer(Cell::Empty)
{
}

Move MinimaxStrategy::chooseMove(const UltimateBoard& board)
{
    // Identification du joueur
    m_rootPlayer = board.getCurrentPlayer();

    Move bestMove{-1, -1};
    int bestScore = LOSS_SCORE - 1;
    int alpha = LOSS_SCORE - 1;
    int beta  = WIN_SCORE  + 1;

    // On itère sur les coups légaux à la racine et on garde le meilleur.
    // C'est une "racine de minimax" séparée du récursif : ça permet
    // de remonter directement le Move à faire.
    for (const Move& move : board.getLegalMoves()) {
        UltimateBoard copy = board;       // clone via constructeur de copie
        copy.play(move);

        int score = minimize(copy, m_maxDepth - 1, alpha, beta);

        if (score > bestScore) {
            bestScore = score;
            bestMove  = move;
        }
        alpha = std::max(alpha, bestScore);
    }

    return bestMove;
}

int MinimaxStrategy::maximize(UltimateBoard& board, int depth, int alpha, int beta)
{
    // Cas de base 1 : la partie est finie, on retourne un score définitif.
    if (board.isFinished()) {
        return terminalScore(board);
    }
    // Cas de base 2 : on a atteint la profondeur max, on évalue heuristiquement.
    if (depth == 0) {
        return m_evaluator->evaluate(board, m_rootPlayer);
    }

    int bestScore = LOSS_SCORE - 1;

    for (const Move& move : board.getLegalMoves()) {
        UltimateBoard copy = board;
        copy.play(move);

        int score = minimize(copy, depth - 1, alpha, beta);
        bestScore = std::max(bestScore, score);

        // Si on a déjà trouvé un coup au moins aussi bon
        // que ce que l'adversaire nous laissera prendre, inutile d'explorer plus.
        alpha = std::max(alpha, bestScore);
        if (alpha >= beta) {
            break;
        }
    }
    return bestScore;
}

int MinimaxStrategy::minimize(UltimateBoard& board, int depth, int alpha, int beta)
{
    if (board.isFinished()) {
        return terminalScore(board);
    }
    if (depth == 0) {
        return m_evaluator->evaluate(board, m_rootPlayer);
    }

    int bestScore = WIN_SCORE + 1;

    for (const Move& move : board.getLegalMoves()) {
        UltimateBoard copy = board;
        copy.play(move);

        int score = maximize(copy, depth - 1, alpha, beta);
        bestScore = std::min(bestScore, score);

        beta = std::min(beta, bestScore);
        if (alpha >= beta) {
            break;  // alpha cut-off
        }
    }
    return bestScore;
}

int MinimaxStrategy::terminalScore(const UltimateBoard& board) const
{
    Cell winner = board.getWinner();
    if (winner == m_rootPlayer)            return WIN_SCORE;
    if (winner == opponent(m_rootPlayer))  return LOSS_SCORE;
    return 0;  // match nul
}
