#include "Bot.h"

#include <bits/stdc++.h>
#include "./pieces/Knights.h"
#include "./pieces/Bishops.h"
#include "./pieces/King.h"
#include "./pieces/Pawns.h"
#include "./pieces/Rooks.h"
#include "./pieces/Queens.h"
#include "BitBoard.h"

extern PlaySide engineSide;

const std::string Bot::BOT_NAME = "MyBot"; /* Edit this, escaped characters are forbidden */
std::unordered_map<char, int> Bot::filesToNumber = {{'a', 0},
                                                    {'b', 1},
                                                    {'c', 2},
                                                    {'d', 3},
                                                    {'e', 4},
                                                    {'f', 5},
                                                    {'g', 6},
                                                    {'h', 7}};

std::unordered_map<char, int> Bot::numberToFiles = {{0, 'a'},
                                                    {1, 'b'},
                                                    {2, 'c'},
                                                    {3, 'd'},
                                                    {4, 'e'},
                                                    {5, 'f'},
                                                    {6, 'g'},
                                                    {7, 'h'}};
Bot::Bot() { /* Initialize custom fields here */
}

void Bot::recordMove(Move* move, PlaySide sideToMove) {
    /* You might find it useful to also separately
     * record last move in another custom field */
    std::string src = move->source.value();
    std::string dst = move->destination.value();
    int bitsrc, bitdst;

    Pawns* pawns = sideToMove == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights* knights = sideToMove == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks* rooks = sideToMove == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops* bishops = sideToMove == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens* queens = sideToMove == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King* king = sideToMove == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;

    if (move->isNormal()) {
        bitsrc = filesToNumber[src[0]] + 8 * (src[1] - '0');
        bitdst = filesToNumber[dst[0]] + 8 * (dst[1] - '0');
        if (pawns->pawns & (1 << bitsrc)) {
            BitBoard::clearBit(pawns->pawns, bitsrc);
            BitBoard::setBit(pawns->pawns, bitdst);
        } else if (knights->knights & (1 << bitsrc)) {
            BitBoard::clearBit(knights->knights, bitsrc);
            BitBoard::setBit(knights->knights, bitdst);
        } else if (rooks->rooks & (1 << bitsrc)) {
            BitBoard::clearBit(rooks->rooks, bitsrc);
            BitBoard::setBit(rooks->rooks, bitdst);
        } else if (bishops->bishops & (1 << bitsrc)) {
            BitBoard::clearBit(bishops->bishops, bitsrc);
            BitBoard::setBit(bishops->bishops, bitdst);
        } else if (queens->queens & (1 << bitsrc)) {
            BitBoard::clearBit(queens->queens, bitsrc);
            BitBoard::setBit(queens->queens, bitdst);
        } else if (king->king & (1 << bitsrc)) {
            BitBoard::clearBit(king->king, bitsrc);
            BitBoard::setBit(king->king, bitdst);
        }
    } else if (move->isPromotion()) {
        bitsrc = filesToNumber[src[0]] + 8 * (src[1] - '0');
        bitdst = filesToNumber[dst[0]] + 8 * (dst[1] - '0');
        BitBoard::clearBit(pawns->pawns, bitsrc);
        switch(move->getReplacement().value()) {
            case Piece::BISHOP:
                BitBoard::setBit(bishops->bishops, bitdst);
                break;
            case Piece::KNIGHT:
                BitBoard::setBit(knights->knights, bitdst);
                break;
            case Piece::ROOK:
                BitBoard::setBit(rooks->rooks, bitdst);
                break;
            case Piece::QUEEN:
                BitBoard::setBit(queens->queens, bitdst);
                break;
            default:
                break;
        }
    } else {
        bitdst = filesToNumber[dst[0]] + 8 * (dst[1] - '0');
        switch(move->getReplacement().value()) {
            case Piece::PAWN:
                BitBoard::setBit(pawns->pawns, bitdst);
                break;
            case Piece::BISHOP:
                BitBoard::setBit(bishops->bishops, bitdst);
                break;
            case Piece::KNIGHT:
                BitBoard::setBit(knights->knights, bitdst);
                break;
            case Piece::ROOK:
                BitBoard::setBit(rooks->rooks, bitdst);
                break;
            case Piece::QUEEN:
                BitBoard::setBit(queens->queens, bitdst);
                break;
            default:
                break;
        }
    }

    BitBoard::updateWhitePieces();
    BitBoard::updateBlackPieces();
    BitBoard::updateAllPieces();
}
static int nr = -1;
Move* Bot::calculateNextMove() {
  /* Play move for the side the engine is playing (Hint: Main.getEngineSide())
   * Make sure to record your move in custom structures before returning.
   *
   * Return move that you are willing to submit
   * Move is to be constructed via one of the factory methods declared in Move.h */
    nr++;
    Pawns* allyPawns = engineSide == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights* allyKnights = engineSide == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks* allyRooks = engineSide == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops* allyBishops = engineSide == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens* allyQueens = engineSide == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King* allyKing = engineSide == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;
    uint64_t allyPieces = engineSide == PlaySide::WHITE ? BitBoard::whitePieces : BitBoard::blackPieces;
    Pawns* enemyPawns = engineSide == PlaySide::BLACK ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights* enemyKnights = engineSide == PlaySide::BLACK ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks* enemyRooks = engineSide == PlaySide::BLACK ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops* enemyBishops = engineSide == PlaySide::BLACK ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens* enemyQueens = engineSide == PlaySide::BLACK ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King* enemyKing = engineSide == PlaySide::BLACK ? BitBoard::whiteKing : BitBoard::blackKing;
    uint64_t enemyPieces = engineSide == PlaySide::BLACK ? BitBoard::whitePieces : BitBoard::blackPieces;

    std::pair < uint64_t, uint64_t > possibleMoves[999];
    int32_t found = 0;
    /* Try to move bishops
     * I. find them.
     * II. try to make moves.*/
     /*
    for (int i = 0; i < 64; ++i) {
        if (allyBishops->bishops & (1ULL << i)) {
            uint64_t possibleMove = allyBishops->possible_moves[i];
            for (int j = 0; j < 64; ++j) {
                if ((possibleMove & (1ULL << j)) && !((1ULL << j) & allyPieces)) {
                    possibleMoves[found++] = std::make_pair(i, j);
                }
            }
        }
    }
    */
    std::vector<Move*> moves = allyPawns->getMoves(engineSide);
    return moves[nr];
}

std::string Bot::getBotName() { return Bot::BOT_NAME; }
