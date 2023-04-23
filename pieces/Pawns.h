#ifndef PAWNS_H
#define PAWNS_H
#include <bits/stdc++.h>
#include "../Move.h"
#include "../PlaySide.h"
#include "../Utils.h"
#include "../Piece.h"
class BitBoard;

class Pawns {
public:
    uint64_t pawns;
    uint64_t promotedPawns;
    Pawns(uint64_t _pawns);

    std::vector<Move*> getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces, uint64_t enPassantWhite, uint64_t enPassantBlack);
    void getAttacks(std::vector<Move *> &moves, PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t enPassantWhite, uint64_t enPassantBlack);
    void getPush(std::vector<Move*> &moves, PlaySide side, uint64_t allPieces);
    uint64_t getAllAttacks(PlaySide side);
};
#endif
