#ifndef ROOKS_H
#define ROOKS_H
#include <bits/stdc++.h>
#include "../../board/Move.h"
#include "../../bot/PlaySide.h"
#include "../../Utils.h"

class Rooks {
public:
    uint64_t rooks;
    bool canCastleLeft = true, canCastleRight = true;

    Rooks(uint64_t _rooks);

    void getMoves(PlaySide side,
        uint64_t blackPieces,
        uint64_t whitePieces,
        uint64_t allPieces,
        std::vector<Move *> &allMoves
    );

    int getNumberOfMoves(PlaySide side,
        uint64_t blackPieces,
        uint64_t whitePieces,
        uint64_t allPieces);

    void updateCastlePermissions(PlaySide engineSide, uint64_t myPieces);

    void initRookAllMoves();

    void initRookMagicTable();

    uint64_t getRookAttacks(int square, uint64_t blockers);

    uint64_t makeValidMove(int square, uint64_t blockers);

    uint64_t getAllAttacks(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces);

    static uint64_t RookMoveFromSquare[64];

    static uint64_t RookAttacks[64][4096];

    constexpr static const uint64_t RookMagic[64] = {
    0x80008120504000ULL,
    0x140100040002002ULL,
    0xa00084080322200ULL,
    0x80280080151000ULL,
    0x2200020004102088ULL,
    0x10009000a440008ULL,
    0x800a0000800100ULL,
    0xa80050000402880ULL,
    0x480800040002884ULL,
    0x8400020100040ULL,
    0x400808010002000ULL,
    0x1001001012248ULL,
    0x2201000800450010ULL,
    0x2000200483004ULL,
    0x4001228240910ULL,
    0x200100010020448aULL,
    0xc048828005724000ULL,
    0x8000404010002002ULL,
    0x2180803000e008ULL,
    0x2004990021001002ULL,
    0x418010009000411ULL,
    0x4005808002010400ULL,
    0x2004000950080aULL,
    0x8001020000840043ULL,
    0xc0008980005060ULL,
    0x2000401000c4ULL,
    0xc001024100306000ULL,
    0x6005002100300008ULL,
    0x40280100110004ULL,
    0x8140040080120080ULL,
    0x9000010080800200ULL,
    0x10200008044ULL,
    0x6000804010800020ULL,
    0x2010082011400440ULL,
    0x220012180803000ULL,
    0x8000980080801000ULL,
    0x1124810400800800ULL,
    0x404808600804400ULL,
    0x84500040100c200ULL,
    0x1f000091000042ULL,
    0x1640114422808001ULL,
    0x240002000808048ULL,
    0x2020004021010012ULL,
    0x1001000250008ULL,
    0x81180040200c0400ULL,
    0x140020004008080ULL,
    0xc0a120b30040088ULL,
    0x5340004c008a0001ULL,
    0x3208002400080ULL,
    0x8000884101220600ULL,
    0x4021022000504300ULL,
    0x522090420a0200ULL,
    0x4000040080680080ULL,
    0x80138084000a0080ULL,
    0x1a20410208900400ULL,
    0x200094806c030a00ULL,
    0x1002801102014262ULL,
    0x1610182001242ULL,
    0x200c08260100aULL,
    0x12002009104046ULL,
    0x22001811200402ULL,
    0x4041008204004801ULL,
    0xa112000809009402ULL,
    0x2201000865420283ULL,
    };

    constexpr static const int RookOnes[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
    };
};
#endif
