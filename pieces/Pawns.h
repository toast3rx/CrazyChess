#ifndef PAWNS_H
#define PAWNS_H
#include <bits/stdc++.h>
#include "../Move.h"
#include "../PlaySide.h"
#include "../Utils.h"
#include "../Bot.h"

class BitBoard;

class Pawns {
public:
    uint64_t pawns;

    Pawns(uint64_t _pawns);

    std::vector<Move*> getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces);
    void getAttacks(std::vector<Move *> &moves, PlaySide side, uint64_t blackPieces, uint64_t whitePieces);
    void getPush(std::vector<Move*> &moves, PlaySide side, uint64_t allPieces);
    void getPromotion(std::vector<Move*> &moves, PlaySide side);
    void getEnPassant(std::vector<Move*> &moves, PlaySide side);
};
#endif
