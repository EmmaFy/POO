#ifndef CELL_H
#define CELL_H

enum class Cell {
    EMPTY,
    X,
    O
};

// inline : autorise les définitions multiples dans plusieurs .cpp incluant ce header.
inline Cell opponent(Cell p) {
    if (p == Cell::X) return Cell::O;
    if (p == Cell::O) return Cell::X;
    return Cell::EMPTY;
}

#endif
