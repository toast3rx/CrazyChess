#include "Bishops.h"
#include "bits/stdc++.h"

Bishops::Bishops(uint64_t _bishops) : bishops(_bishops) {}

void Bishops::Move() {
    uint_8 row = 0, column = 0;
    for (int pos = 0; i < SQUARES; ++pos, ++row, ++column) {
        if (column == COLUMNS) {
            columns = 0;
            ++rows;
        }
        uint8_t aux_row;
        uint8_t aux_column;

        // dreapta sus
        aux_row = row, aux_column = column;
        for (int i = 0; i < min(ROWS - row - 1, COLUMNS - column - 1); ++i) {
            possible_moves[pos] |= (1 << (pos + (i + 1) * 9));
        }
        // stanga sus
        for (int i = 0; i < min(ROWS - row - 1, column); ++i) {
            possible_moves[pos] |= (1 << (pos + (i + 1) * 7));
        }
        // dreapta jos
        for (int i = 0; i < min(row, COLUMNS - column - 1); ++i) {
            possible_moves[pos] |= (1 << (pos - (i + 1) * 7));
        }
        // stanga jos
        for (int i = 0; i < min(row, column); ++i) {
            possible_moves[pos] |= (1 << (pos + (i + 1) * 9));
        }
    }
    /*2^i
    8 X X X X X X X X
    7 X X X X X X X X
    6 X X X X X X X X
    5 X X X X X X X X
    4 X X X X X X X X
    3 X X X X X X X X
    2 X X X X X X X X
    1 X X X X X X X X
      A B C D E F G H
    */
}