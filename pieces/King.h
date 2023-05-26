#ifndef KING_H
#define KING_H

#include <bits/stdc++.h>
#include "../Move.h"
#include "../PlaySide.h"
#include "../Utils.h"
#include "Rooks.h"

class King {
public:
    uint64_t king;
    bool canCastle = true;

    uint64_t kingMoves[64] = {0};

    King(uint64_t _king);

    void getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces, std::vector<Move*> &allMoves);
    std::vector<Move *> tryCastle(PlaySide side, Rooks *rooks, uint64_t enemyAttacks,
                                    uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces);

    uint64_t getAllAttacks(PlaySide side, uint64_t blackPieces, uint64_t whitePieces);
    
    void initKingAllMoves();

    void updateCastlePermissions(PlaySide engineSide, uint64_t myPieces);
};

#endif
