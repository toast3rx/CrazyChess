#include "King.h"


std::vector<Move*> King::getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces)
{
    std::vector<Move*> moves;
    uint64_t botPieces = 0;
    if (side == PlaySide::WHITE) {
        botPieces = whitePieces;
    } else {
        botPieces = blackPieces;
    }

    uint64_t currentKing = king;
    // int kingPos = Utils::getBits(currentKing);
    int kingPos = Utils::getOneBitsPositions(currentKing)[0];
    uint64_t possibleMoves = kingMoves[kingPos];
    uint64_t validMoves = possibleMoves & ~botPieces;

    int currentKingMoves = Utils::getBits(validMoves);
    while (currentKingMoves--) {
        int destMove = Utils::popLsb(validMoves);

        int kingRank = kingPos / 8;
        int kingFile = kingPos % 8;

        std::string prev = "";
        char fileChar = kingFile + 'a';
        char rankChar = kingRank + '1';
        prev.append(1, fileChar);
        prev.append(1, rankChar);

        int destRank = destMove / 8;
        int destFile = destMove % 8;

        std::string next = "";
        char fileChar2 = destFile + 'a';
        char rankChar2 = destRank + '1';
        next.append(1, fileChar2);
        next.append(1, rankChar2);

        moves.push_back(Move::moveTo(prev, next));
    }
    return moves;
}

King::King(uint64_t _king) : king(_king) {
    initKingAllMoves();
}

void King::initKingAllMoves() {

    uint64_t FILE_A = Utils::getFile(1);
    uint64_t FILE_H = Utils::getFile(8);

    uint64_t RANK_8 = Utils::getRank(8);
    uint64_t RANK_1 = Utils::getRank(1);

    for (uint64_t square = 0; square < 64; square++) {
        uint64_t prev = (1ULL << square);
        uint64_t next = 0;
        // down-left move
        if ((prev & ~FILE_A & ~RANK_1) != 0) next |= (prev >> 9);

        // down move
        if ((prev & ~RANK_1) != 0) next |= (prev >> 8);

        // down-right move
        if ((prev & ~FILE_H & ~RANK_1) != 0) next |= (prev >> 7);

        // left move 
        if ((prev & ~FILE_A) != 0) next |= (prev >> 1);

        // right move
        if ((prev & ~FILE_H) != 0) next |= (prev << 1);

        // top-left move
        if ((prev & ~FILE_A & ~RANK_8) != 0) next |= (prev << 7);

        // top move
        if ((prev & ~RANK_8) != 0) next |= (prev << 8);

        // top-right move
        if ((prev & ~RANK_8 & ~FILE_H) != 0) next |= (prev << 9);

        kingMoves[square] = next;
    }

    for (uint64_t i = 0; i < 64; i++) {
        Utils::printBoard(kingMoves[i], "KingAllMoves.txt");
    }
}