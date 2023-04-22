#include "Rooks.h"


int getLsbIdx(int64_t n) {
    int pos = 0;
    while (!(n & 1)) {
        n >>= 1;
        pos++;
    }
    return pos;
}

int popLsb(int64_t &n) {
    int LsbIndex = getLsbIdx(n);
    n = n & (n - 1);
    return LsbIndex;
}

int getBits(int64_t n) {
    int pos = 0;
    while (n) {
        n >>= 1;
        pos++;
    }

    return pos;
}

int64_t getBlockersFromPerm(int perm, int64_t mask) {
    int64_t blockers = 0;
    int bits = getBits(mask);
    // iterate through all "one" bits of the mask
    for (int i = 0; i < bits; i++) {
        // find it's position
        int bitPos = popLsb(mask);
        // if it is a blocker from the permutation add it to the blockers
        if (perm & (1 << i)) {
            blockers |= (1 << bitPos);
        }
    }
    return blockers;
}

Rooks::Rooks(uint64_t _rooks) : rooks(_rooks) {

    initRookAllMoves();
    initRookOnes();
    initRookMagicTable();

}


void Rooks::Move() {

}

void Rooks::initRookAllMoves() {
    for (int sq = 0; sq < 64; sq++) {
        RookMoveFromSquare[sq] |= BitBoard::getRank((sq / 8) + 1) & ~BitBoard::getRank(1) & ~BitBoard::getRank(8);
        RookMoveFromSquare[sq] |= BitBoard::getFile((sq % 8) + 1) & ~BitBoard::getFile(1) & ~BitBoard::getFile(8);
    }
}

void Rooks::initRookOnes() {
    for (int sq = 0; sq < 64; sq++) {
        int64_t mask = RookMoveFromSquare[sq];
        while (mask) {
            RookOnes[sq] += (mask & 1);
            mask >>= 1;
        }
    }
}

void Rooks::initRookMagicTable() {

    for (int sq = 0; sq < 64; sq++) {

        for (int blockersPerm = 0; blockersPerm < (1 << RookOnes[sq]); blockersPerm++) {
            int64_t blockers = getBlockersFromPerm(blockersPerm, RookMoveFromSquare[sq]);
            RookAttacks[sq][(blockers * RookMagic[sq]) >> (64 - RookOnes[sq])] = makeValidMove(sq, blockers);
        }
    }

}

int64_t Rooks::getRookAttacks(int square, int64_t blockers) {

    blockers &= RookMoveFromSquare[square];
    int64_t key = (blockers * RookMagic[square]) >> (64 - RookOnes[square]);
    return RookAttacks[square][key];
}

int64_t Rooks::makeValidMove(int square, int64_t blockers) {
    int64_t attacks = 0;

    int64_t north = BitBoard::getFile(1) << square;
    int64_t south = BitBoard::getFile(8) >> (63 - square);
    // int64_t east = (1 << (square | 7)) - (1 << square);
    return 0;
}

