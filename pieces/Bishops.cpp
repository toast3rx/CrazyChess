
#include "Bishops.h"
#include <bits/stdc++.h>

Bishops::Bishops(uint64_t _bishops) : bishops(_bishops) {}

void Bishops::Move() {
    int64_t row = 0, column = 0;
    for (int64_t pos = 0; pos < SQUARES; ++pos, ++column) {
        if (column == COLUMNS) {
            column = 0;
            ++row;
        }

        // dreapta sus
        for (int64_t i = 0; i < std::min(ROWS - row - 1, COLUMNS - column - 1); ++i) {
            possible_moves[pos] |= (1ULL << (pos + (i + 1) * 9));
        }
        // stanga sus
        for (int64_t i = 0; i < std::min(ROWS - row - 1, column); ++i) {
            possible_moves[pos] |= (1ULL << (pos + (i + 1) * 7));
        }
        // dreapta jos
        for (int64_t i = 0; i < std::min(row, COLUMNS - column - 1); ++i) {
            possible_moves[pos] |= (1ULL << (pos - (i + 1) * 7));
        }
        // stanga jos
        for (int64_t i = 0; i < std::min(row, column); ++i) {
            possible_moves[pos] |= (1ULL << (pos - (i + 1) * 9));
        }
    }
}

void debug(uint64_t matrix) {
    int64_t aux_row;
    int64_t aux_column;
    for (int64_t i = 0; i < SQUARES; ++i) {
        std::cout << i << "\n";
        // sunt pe pozitia i, afisez tabla cu pozitii de aici.
        for (aux_row = ROWS - 1; aux_row >= 0; --aux_row) {
            for (aux_column = 0; aux_column < COLUMNS; ++aux_column) {
                std::cout << (((matrix) & (1ULL << ((aux_row) * ROWS + aux_column))) > 0) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}