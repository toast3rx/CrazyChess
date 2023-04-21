#include "BitBoard.h"

#include <bits/stdc++.h>

BitBoard::BitBoard() {
    whitePawns = 0xff00ULL;
    blackPawns = 0xff000000000000ULL;

    whiteRooks = 0x81;
    blackRooks = 0x8100000000000000ULL;

    whiteBishops = 0x24;
    blackBishops = 0x2400000000000000ULL;

    whiteKnights = 0x42;
    blackKnights = 0x4200000000000000ULL;

    whiteQueens = 0x10ULL;
    blackQueens = 0x1000000000000000ULL;

    whiteKing = 0x8ULL;
    blackKing = 0x8000000000000000ULL;

}


