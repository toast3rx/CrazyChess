#ifndef ROOKS_H
#define ROOKS_H
#include <bits/stdc++.h>
#include "../Utils.h"

class Rooks {
public:
    uint64_t rooks;

    Rooks(uint64_t _rooks);

    void Move();

    void initRookAllMoves();

    void initRookMagicTable();

    uint64_t getRookAttacks(int square, uint64_t blockers);

    uint64_t makeValidMove(int square, uint64_t blockers);

    uint64_t RookMoveFromSquare[64] = {0};

    uint64_t RookAttacks[64][4096] = {{0}};


    constexpr static const uint64_t RookMagic[64] = {
        0x80004000976080ULL,
        0x1040400010002000ULL,
        0x4880200210000980ULL,
        0x5280080010000482ULL,
        0x200040200081020ULL,
        0x2100080100020400ULL,
        0x4280008001000200ULL,
        0x1000a4425820300ULL,
        0x29002100800040ULL,
        0x4503400040201004ULL,
        0x209002001004018ULL,
        0x1131000a10002100ULL,
        0x9000800120500ULL,
        0x10e001804820010ULL,
        0x29000402000100ULL,
        0x2002000d01c40292ULL,
        0x80084000200c40ULL,
        0x10004040002002ULL,
        0x201030020004014ULL,
        0x80012000a420020ULL,
        0x129010008001204ULL,
        0x6109010008040002ULL,
        0x950010100020004ULL,
        0x803a0000c50284ULL,
        0x80004100210080ULL,
        0x200240100140ULL,
        0x20004040100800ULL,
        0x4018090300201000ULL,
        0x4802010a00102004ULL,
        0x2001000900040002ULL,
        0x4a02104001002a8ULL,
        0x2188108200204401ULL,
        0x40400020800080ULL,
        0x880402000401004ULL,
        0x10040800202000ULL,
        0x604410a02001020ULL,
        0x200200206a001410ULL,
        0x86000400810080ULL,
        0x428200040600080bULL,
        0x2001000041000082ULL,
        0x80002000484000ULL,
        0x210002002c24000ULL,
        0x401a200100410014ULL,
        0x5021000a30009ULL,
        0x218000509010010ULL,
        0x4000400410080120ULL,
        0x20801040010ULL,
        0x29040040820011ULL,
        0x4080400024800280ULL,
        0x500200040100440ULL,
        0x2880142001004100ULL,
        0x412020400a001200ULL,
        0x18c028004080080ULL,
        0x884001020080401ULL,
        0x210810420400ULL,
        0x801048745040200ULL,
        0x4401002040120082ULL,
        0x408200210012ULL,
        0x110008200441ULL,
        0x2010002004100901ULL,
        0x801000800040211ULL,
        0x480d000400820801ULL,
        0x820104201280084ULL,
        0x1001040311802142ULL,
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
