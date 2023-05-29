#ifndef PAWNS_H
#define PAWNS_H
#include <bits/stdc++.h>
#include "../../board/Move.h"
#include "../../bot/PlaySide.h"
#include "../../Utils.h"
#include "../../board/Piece.h"
class BitBoard;

class Pawns {
public:
    uint64_t pawns;
    uint64_t promotedPawns;
    Pawns(uint64_t _pawns);

    void getMoves(PlaySide side, 
                                uint64_t blackPieces, 
                                uint64_t whitePieces, 
                                uint64_t allPieces,
                                uint64_t enPassantWhite, 
                                uint64_t enPassantBlack,
                                std::vector<Move*> &allMoves
                                );

  
        
    void getAttacks(std::vector<Move *> &moves, PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t enPassantWhite, uint64_t enPassantBlack);
    void getPush(std::vector<Move*> &moves, PlaySide side, uint64_t allPieces);
    uint64_t getAllAttacks(PlaySide side);

    int getNumberOfMoves(PlaySide side,
        uint64_t blackPieces,
        uint64_t whitePieces,
        uint64_t allPieces,
        uint64_t enPassantWhite,
        uint64_t enPassantBlack);
};
#endif
