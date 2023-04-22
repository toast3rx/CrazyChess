#ifndef PAWNS_H
#define PAWNS_H
#include <bits/stdc++.h>
#include "../Move.h"
#include "../PlaySide.h"
#include "../Bot.h"

class BitBoard;

class Pawns {
public:
    uint64_t pawns;

    Pawns(uint64_t _pawns);

    std::vector<Move*> getMoves(PlaySide side);
    void getAttacks(std::vector<Move*> moves, PlaySide side);
    void getPush(std::vector<Move*> moves, PlaySide side);
    void getPromotion(std::vector<Move*> moves, PlaySide side);
    void getEnPassant(std::vector<Move*> moves, PlaySide side);
};
#endif
