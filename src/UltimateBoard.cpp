#include "UltimateBoard.h"

UltimateBoard::UltimateBoard()
:forcedBoardRow(-1), forcedBoardCol(-1), currentPlayer(Cell::X)
{
    //ctor
}

UltimateBoard::~UltimateBoard()
{
    //dtor
}

//bool UltimateBoard::play(Move move) {
//    return false;
//}

//std::vector<Move> UltimateBoard::getLegalMoves() const {
//    return std::vector<Move>();
//}
//
//Cell UltimateBoard::getWinner() const {
//    return Cell::EMPTY;
//}

UltimateBoard UltimateBoard::clone() const {
    return *this;
}

Cell UltimateBoard::getWinner() const
{
    return result.getWinner();
}

std::vector<Move> UltimateBoard::getLegalMoves() const
{
    std::vector<Move> moves;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            int bR = r / 3, bC = c / 3;
            // Vérifier si la grille est celle imposée
            if (forcedBoardRow != -1 && (bR != forcedBoardRow || bC != forcedBoardCol)) continue;
            // Vérifier si la petite grille est déjà finie
            if (boards[bR][bC].getWinner() != Cell::Empty || boards[bR][bC].isComplete()) continue;
            // Vérifier si la case est vide
            if (boards[bR][bC].getCell(r % 3, c % 3) == Cell::Empty) {
                moves.push_back({r, c});
            }
        }
    }
    return moves;
}

bool UltimateBoardisFinished() const
{
    return getWinner() != Cell::Empty || getLegalMoves().empty();
}

bool UltimateBoard::play(Move m)
{
    // 1. Vérifier si le mouvement est dans la grille forcée
    int bR = m.row / 3;
    int bC = m.col / 3;
    int localR = m.row % 3;
    int localC = m.col % 3;

    if (forcedBoardRow != -1 && (bR != forcedBoardRow || bC != forcedBoardCol)) return false;
    if (boards[bR][bC].getWinner() != Cell::Empty) return false;

    // 2. Jouer le coup
    if (!boards[bR][bC].play(localR, localC, currentPlayer)) return false;

    // 3. Mettre à jour la grille de résultat globale
    if (boards[bR][bC].getWinner() != Cell::Empty) {
        result.play(bR, bC, boards[bR][bC].getWinner());
    }

    // 4. Déterminer la prochaine grille forcée
    if (boards[localR][localC].getWinner() == Cell::Empty && !boards[localR][localC].isComplete()) {
        forcedBoardRow = localR;
        forcedBoardCol = localC;
    } else {
        forcedBoardRow = -1; // Libre de jouer n'importe où
        forcedBoardCol = -1;
    }

    // 5. Changer de joueur
    currentPlayer = (currentPlayer == Cell::X) ? Cell::O : Cell::X;
    return true;
}
