#ifndef BITBOARD_H
#define BITBOARD_H
#include <bits/stdc++.h>

// in the bitboard bit 0 is a1 and bit 63 is a7

class BitBoard {
 private:

    // white pieces
    uint64_t whitePawns;
    uint64_t whiteRooks;
    uint64_t whiteBishops;
    uint64_t whiteKnights;
    uint64_t whiteQueens;
    uint64_t whiteKing;

    // black pieces
    uint64_t blackPawns;
    uint64_t blackRooks;
    uint64_t blackBishops;
    uint64_t blackKnights;
    uint64_t blackQueens;
    uint64_t blackKing;

 public:
  
};
#endif
