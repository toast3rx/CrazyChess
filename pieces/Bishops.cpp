
#include "Bishops.h"
#include "bits/stdc++.h"

Bishops::Bishops(uint64_t _bishops) : bishops(_bishops) {}

void Bishops::Move() {
    uint_8 row = 0, column = 0;
    for (int64_t pos = 0; i < SQUARES; ++pos, ++row, ++column) {
        if (column == COLUMNS) {
            columns = 0;
            ++rows;
        }
        for (int64_t i = 0; i < min(ROWS - row - 1, COLUMNS - column - 1); ++i) {
            possible_moves[pos] |= (1ULL << (pos + (i + 1) * 9));
        }
        // stanga sus
        for (int64_t i = 0; i < min(ROWS - row - 1, column); ++i) {
            possible_moves[pos] |= (1ULL << (pos + (i + 1) * 7));
        }
        // dreapta jos
        for (int64_t i = 0; i < min(row, COLUMNS - column - 1); ++i) {
            possible_moves[pos] |= (1ULL << (pos - (i + 1) * 7));
        }
        // stanga jos
        for (int64_t i = 0; i < min(row, column); ++i) {
            possible_moves[pos] |= (1ULL << (pos + (i + 1) * 9));
        }
    }

    int64_t aux_row;
    int64_t aux_column;
    // for (int64_t i = 0; i < SQUARES; ++i) {
    //     // sunt pe pozitia i, afisez tabla cu pozitii de aici.
    //     for (aux_row = ROWS - 1; aux_row >= 0; --aux_row) {
    //         for (aux_column = 0; aux_column < COLUMNS; ++aux_column) {
    //             cout << (((possible_moves[i]) & (1ULL << ((aux_row) * ROWS + aux_column))) > 0) << " ";
    //         }
    //         cout << "\n";
    //     }
    //     cout << "\n";
    // }
    cout << "ok bro\n";
    
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