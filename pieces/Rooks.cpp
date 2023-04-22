#include "Rooks.h"

int getLsbIdx(uint64_t n) {
    int pos = 0;
    while ((n & 1) == 0) {
        if (n == 0) {
            return pos;
        }
        n = (n >> 1);
        pos++;
    }
    return pos;
}

int popLsb(uint64_t &n) {
    int LsbIndex = __builtin_ffsll(n) - 1;
    n = n & (n - 1);
    return LsbIndex;
}

int getBits(uint64_t n) {
    return __builtin_popcountll(n);
}

int getRookKey(int square, uint64_t blockers) {

    return (blockers * Rooks::RookMagic[square]) >> (64 - Rooks::RookOnes[square]);
}

uint64_t getBlockersFromPerm(int perm, uint64_t mask, int debug) {
    uint64_t blockers = 0;
    int bits = getBits(mask);
    // iterate through all "one" bits of the mask
    for (int i = 0; i < bits; i++) {
        // find it's position
        int bitPos = popLsb(mask);
        // if it is a blocker from the permutation add it to the blockers
        if (perm & (1 << i)) {
            blockers = (blockers | (1ULL << bitPos));
        }
    }
    return blockers;
}

Rooks::Rooks(uint64_t _rooks) : rooks(_rooks) {

    initRookAllMoves();
    initRookMagicTable();

}


std::vector<Move*> Rooks::getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces) {

    std::vector<Move*> moves;
    uint64_t botPieces = 0;
    uint64_t enemyPieces = 0;
    if (side == PlaySide::WHITE) {
        botPieces = whitePieces;
        enemyPieces = blackPieces;
    } else {
        botPieces = blackPieces;
        enemyPieces = whitePieces;
    }

    uint64_t currentRooks = rooks;
    int rooksCount = getBits(currentRooks);
    while (rooksCount--) {
        int rookPos = popLsb(currentRooks);

        uint64_t possibleMoves = RookAttacks[rookPos][getRookKey(rookPos, allPieces)];
        // uint64_t attackingMoves = possibleMoves & blackPieces;
        uint64_t blockedMoves = possibleMoves & whitePieces;
        possibleMoves = possibleMoves & ~blockedMoves;

        int currentRookMoves = getBits(possibleMoves);
        while (currentRookMoves--) {
            int destPos = popLsb(possibleMoves);

            int rookRank = rookPos / 8;
            int rookFile = rookPos % 8;

            std::string prev = "";
			char fileChar = rookFile + 'a';
			char rankChar = rookRank + '0';
			prev.append(1, fileChar);
			prev.append(1, rankChar);

            int destRank = destPos / 8;
            int destFile = destPos % 8;

            std::string next = "";
            char fileChar2 = destFile + 'a';
			char rankChar2 = destRank + '0';
			next.append(1, fileChar2);
			next.append(1, rankChar2);

            moves.push_back(Move::moveTo(prev, next));
        }
    }

    return moves;

}

void Rooks::initRookAllMoves() {
    for (int sq = 0; sq < 64; sq++) {
        RookMoveFromSquare[sq] = 0;
		uint64_t sideMoves = (Utils::getRank(sq / 8 + 1) & ~Utils::getFile(1) & ~Utils::getFile(8));
		uint64_t upDownMoves = (Utils::getFile(sq % 8 + 1) & ~Utils::getRank(1) & ~Utils::getRank(8));
        RookMoveFromSquare[sq] = ((sideMoves | upDownMoves) & ~(1ULL << sq));
    }
}

void Rooks::initRookMagicTable() {

    for (int sq = 0; sq < 64; sq++) {

        for (int blockersPerm = 0; blockersPerm < (1 << RookOnes[sq]); blockersPerm++) {
            uint64_t blockers = getBlockersFromPerm(blockersPerm, RookMoveFromSquare[sq], 0) & ~(1 << sq);
            RookAttacks[sq][getRookKey(sq, blockers)] = 0;
            RookAttacks[sq][getRookKey(sq, blockers)] = makeValidMove(sq, blockers);
        }
    }
}

uint64_t Rooks::getRookAttacks(int square, uint64_t blockers) {

    blockers &= RookMoveFromSquare[square];
    uint64_t key = getRookKey(square, blockers);

    return RookAttacks[square][key];
}

uint64_t Rooks::makeValidMove(int square, uint64_t blockers) {
    uint64_t attacks = 0;
    int column = square % 8;
    int row = square / 8;

    /// stanga
    for (int j = 1; j <= column; j++) {
        attacks |= (1ULL << (square - j));
        if (attacks & blockers) {
            blockers = (blockers & ~attacks);
            break;
        }
    }

    /// dreapta
    for (int j = 1; j <= 7 - column; j++) {
        attacks |= (1ULL << (square + j));
        if (attacks & blockers) {
            blockers = (blockers & ~attacks);
            break;
        }
    }

    /// sus
    for (int j = 1; j <= 7 - row; j++) {
        attacks |= (1ULL << (square + j * 8));
        if (attacks & blockers) {
            blockers = (blockers & ~attacks);
            break;
        }
    }

    /// jos
    for (int j = 1; j <= row; j++) {
        attacks |= (1ULL << (square - j * 8));
        if (attacks & blockers) {
            blockers = (blockers & ~attacks);
            break;
        }
    }

    return attacks;
}

