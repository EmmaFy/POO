#include "UltimateBoard.h"

UltimateBoard::UltimateBoard()
    : m_forcedRow(-1), m_forcedCol(-1), m_currentPlayer(Cell::X) {}

bool UltimateBoard::play(Move move) {
    int bR = move.getRow() / 3,  bC = move.getCol() / 3;
    int lR = move.getRow() % 3,  lC = move.getCol() % 3;

    // Vérifier contrainte de grille forcée
    if (m_forcedRow != -1 && (bR != m_forcedRow || bC != m_forcedCol)) return false;
    if (m_boards[bR][bC].isComplete()) return false;
    if (!m_boards[bR][bC].play(lR, lC, m_currentPlayer)) return false;

    // Mettre à jour la méta-grille si sous-grille remportée
    Cell sbWin = m_boards[bR][bC].getWinner();
    if (sbWin != Cell::EMPTY)
        m_result.play(bR, bC, sbWin);

    // Prochaine grille forcée = position locale jouée
    if (!m_boards[lR][lC].isComplete()) {
        m_forcedRow = lR;
        m_forcedCol = lC;
    } else {
        // Cible terminée → libre
        m_forcedRow = -1;
        m_forcedCol = -1;
    }

    m_currentPlayer = opponent(m_currentPlayer);
    return true;
}

std::vector<Move> UltimateBoard::getLegalMoves() const {
    std::vector<Move> moves;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int bR = r / 3, bC = c / 3;

            if (m_forcedRow != -1 && (bR != m_forcedRow || bC != m_forcedCol)) continue;

            const SmallBoard& sb = m_boards[bR][bC];
            int localIndex = (r % 3) * 3 + (c % 3);

            if (!sb.isComplete() && sb.getCell(localIndex) == Cell::EMPTY)
                moves.push_back(Move(r, c));
        }
    }
    return moves;
}

Cell UltimateBoard::getWinner() const {
    return m_result.getWinner();
}

bool UltimateBoard::isFinished() const {
    return getWinner() != Cell::EMPTY || getLegalMoves().empty();
}
