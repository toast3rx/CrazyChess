#ifndef QUEEN_H
#define QUEEN_H
#include <bits/stdc++.h>
#include "../../PlaySide.h"
#include "../../bot/Move.h"
#include "../../Utils.h"
#include "../bishop/Bishops.h"
#include "../rook/Rooks.h"

class Queens: public PieceBitBoard {
public:
    uint64_t queens;
    // Bishops *all_moves[SQUARES], *legal_moves[512], *blockers[512], *bishopAttacks[SQUARES][SQUARES * 8];
    // Rooks *RookMoveFromSquare[64] = {0}, *RookAttacks[64][4096] = {{0}};
    Queens(uint64_t _queens);

    // void Move();
    void init();
    
    std::vector<Move *>  getMoves(PlaySide side,
                                Rooks *rooks, 
                                Bishops *bishop, 
                                uint64_t blackPieces, 
                                uint64_t whitePieces, 
                                uint64_t allPieces
                                );

    uint64_t getAllAttacks(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces);
};

#endif
