#ifndef KING_H
#define KING_H

#include <bits/stdc++.h>
#include "../Move.h"
#include "../PlaySide.h"
#include "../Utils.h"

class King {
public:
    uint64_t king;

    uint64_t kingMoves[64] = {0};

    King(uint64_t _king);

    std::vector<Move*> getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces);

    uint64_t getAllAttacks(PlaySide side, uint64_t blackPieces, uint64_t whitePieces);
    
    void initKingAllMoves();
};

#endif
