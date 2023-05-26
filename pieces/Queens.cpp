#include "Queens.h"
#include <bits/stdc++.h>

Queens::Queens(uint64_t _queens) : queens(_queens) {
    // init();
}

int32_t get_key_bishop(uint64_t square, uint64_t blockers)
{
    return (Bishops::bishopMagic[square] * blockers) >> (SQUARES - Bishops::bishop_bits[square]);
}
int32_t get_key_rook(uint64_t square, uint64_t blockers) {

    return (blockers * Rooks::RookMagic[square]) >> (64 - Rooks::RookOnes[square]);
}

void Queens::getMoves(PlaySide side, 
                                    Rooks *rooks, 
                                    Bishops *bishop, 
                                    uint64_t blackPieces, 
                                    uint64_t whitePieces, 
                                    uint64_t allPieces,
                                    std::vector<Move*> &allMoves
                                    )
{
    std::vector< Move* > sol;
    uint64_t myPieces = 0;
    if (side == 0) {
        myPieces = blackPieces;
    } else {
        myPieces = whitePieces;
    }
    uint64_t currentQueens = queens;
    while (currentQueens) {
        uint64_t queenPos = Utils::popLsb(currentQueens);
        uint64_t blockers = allPieces & Bishops::all_moves[queenPos];
        uint64_t possibleMoves = Bishops::bishopAttacks[queenPos][get_key_bishop(queenPos, blockers)];

        /* Scot acele mutari care mi-ar fi luat piesele mele. Daca lucrul asta este verificat atunci sterge liniile de cod in plus*/
        uint64_t blockedMoves = possibleMoves & myPieces;
        possibleMoves = possibleMoves & ~blockedMoves;
        /* -----------------------------------------------------------------------------------------------------------------------*/

        while (possibleMoves) {
            std::pair < std::string, std::string > new_pair = Utils::convert_table_to_positions(queenPos, possibleMoves, myPieces);
            if (new_pair.first.compare("a")) {
                sol.push_back(Move::moveTo(new_pair.first, new_pair.second));
            }
        }
    }

    currentQueens = queens;    
    int rooksCount = Utils::getBits(currentQueens);
    while (rooksCount--) {
        int rookPos = Utils::popLsb(currentQueens);

        uint64_t possibleMoves = Rooks::RookAttacks[rookPos][get_key_rook(rookPos, allPieces & Rooks::RookMoveFromSquare[rookPos])];
        // uint64_t attackingMoves = possibleMoves & blackPieces;
        uint64_t blockedMoves = possibleMoves & myPieces;
        possibleMoves = possibleMoves & ~blockedMoves;

        int currentRookMoves = Utils::getBits(possibleMoves);
        while (currentRookMoves--) {
            int destPos = Utils::popLsb(possibleMoves);

            int rookRank = rookPos / 8 + 1;
            int rookFile = rookPos % 8;

            std::string prev = "";
			char fileChar = rookFile + 'a';
			char rankChar = rookRank + '0';
			prev.append(1, fileChar);
			prev.append(1, rankChar);

            int destRank = destPos / 8 + 1;
            int destFile = destPos % 8;

            std::string next = "";
            char fileChar2 = destFile + 'a';
			char rankChar2 = destRank + '0';
			next.append(1, fileChar2);
			next.append(1, rankChar2);

            sol.push_back(Move::moveTo(prev, next));
        }
    }

    allMoves.insert(allMoves.end(), sol.begin(), sol.end());
    // return sol;
}

uint64_t Queens::getAllAttacks(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces)
{
    uint64_t attacks = 0ULL;
    uint64_t myPieces = 0;
    if (side == 0) {
        myPieces = blackPieces;
    } else {
        myPieces = whitePieces;
    }
    uint64_t currentQueens = queens;
    while (currentQueens) {

        uint64_t queenPos = Utils::popLsb(currentQueens);
        uint64_t blockersBishop = allPieces & Bishops::all_moves[queenPos];
        uint64_t blockersRook = allPieces & Rooks::RookMoveFromSquare[queenPos];
        uint64_t possibleMovesFromBishop = Bishops::bishopAttacks[queenPos][get_key_bishop(queenPos, blockersBishop)];
        uint64_t possibleMovesFromRook = Rooks::RookAttacks[queenPos][get_key_rook(queenPos, blockersRook)];
        uint64_t possibleMoves = possibleMovesFromBishop | possibleMovesFromRook;
        
        /* Scot acele mutari care mi-ar fi luat piesele mele. Daca lucrul asta este verificat atunci sterge liniile de cod in plus*/
        uint64_t blockedMoves = possibleMoves & myPieces;
        possibleMoves = possibleMoves & ~blockedMoves;
        /* -----------------------------------------------------------------------------------------------------------------------*/;

        attacks |= possibleMoves;
        
    }


    return attacks;
}

