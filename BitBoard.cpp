#include "BitBoard.h"

#include <bits/stdc++.h>
#include "pieces/Queens.h"

BitBoard::BitBoard() {

    whitePawns = new Pawns(0xff00ULL);
    blackPawns = new Pawns(0xff000000000000ULL);

    whiteRooks = new Rooks(0x81);
    blackRooks = new Rooks(0x8100000000000000ULL);

    whiteBishops = new Bishops(0x24);
    blackBishops = new Bishops(0x2400000000000000ULL);

    // Initialize the knights
    whiteKnights = new Knights(0x42);
    blackKnights = new Knights(0x4200000000000000ULL);

    whiteQueens = new Queens(0x8ULL);
    blackQueens = new Queens(0x8000000000000000ULL);

    whiteKing = new King(0x10ULL);
    blackKing = new King(0x1000000000000000ULL);

    whitePieces = whitePawns->pawns | whiteRooks->rooks | whiteBishops->bishops | whiteKnights->knights | whiteQueens->queen | whiteKing->king;
    blackPieces = blackPawns->pawns | blackRooks->rooks | blackBishops->bishops | blackKnights->knights | blackQueens->queen | blackKing->king;
}

// approved by mihai
uint64_t static getRank(int r) {
    return (0xffULL << (8 * (r - 1)));
}

// approved by mihai
// file index starts from 1
uint64_t static getFile(int f) {
        
    return (0x0101010101010101ULL << (f - 1));
}



