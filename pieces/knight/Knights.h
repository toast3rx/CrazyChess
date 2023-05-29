#ifndef KNIGHT_H
#define KNIGHT_H

#include <bits/stdc++.h>
#include <vector>
#include <string>
#include "../../board/Move.h"
#include "../../Utils.h"
#include "../../bot/PlaySide.h"


class Knights {
public:
    uint64_t knights;
    uint64_t knightsMoves[64];

    Knights(uint64_t _pawns);

    uint64_t generateMoves(int square);

    int getNumberOfMoves(PlaySide side,
                         uint64_t blackPieces,
                         uint64_t whitePieces
    );

    void getAttacks(std::vector<Move *> &moves, PlaySide side, uint64_t blackPieces, uint64_t whitePieces);

    uint64_t getAllAttacks(PlaySide side, uint64_t blackPieces, uint64_t whitePieces);

    void getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces,
                  std::vector<Move *> &allMoves);
};

#endif