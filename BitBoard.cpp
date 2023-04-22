#include "BitBoard.h"

// Initialize all the pieces

Pawns *BitBoard::whitePawns = new Pawns(0xff00ULL);
Pawns *BitBoard::blackPawns = new Pawns(0xff000000000000ULL);

Rooks *BitBoard::whiteRooks = new Rooks(0x81ULL);
Rooks *BitBoard::blackRooks = new Rooks(0x8100000000000000ULL);

Bishops *BitBoard::whiteBishops = new Bishops(0x24ULL);
Bishops *BitBoard::blackBishops = new Bishops(0x2400000000000000ULL);

Knights *BitBoard::whiteKnights = new Knights(0x42ULL);
Knights *BitBoard::blackKnights = new Knights(0x4200000000000000ULL);

Queens *BitBoard::whiteQueens = new Queens(0x8ULL);
Queens *BitBoard::blackQueens = new Queens(0x8000000000000000ULL);

King *BitBoard::whiteKing = new King(0x10ULL);
King *BitBoard::blackKing = new King(0x1000000000000000ULL);

uint64_t BitBoard::blackPieces =
                    BitBoard::blackPawns->pawns |
                    BitBoard::blackRooks->rooks | 
                    BitBoard::blackBishops->bishops | 
                    BitBoard::blackKnights->knights | 
                    BitBoard::blackQueens->queens | 
                    BitBoard::blackKing->king;

uint64_t BitBoard::whitePieces =
                    BitBoard::whitePawns->pawns |
                    BitBoard::whiteRooks->rooks | 
                    BitBoard::whiteBishops->bishops | 
                    BitBoard::whiteKnights->knights | 
                    BitBoard::whiteQueens->queens | 
                    BitBoard::whiteKing->king;

uint64_t BitBoard::allPieces = BitBoard::blackPieces | BitBoard::whitePieces;                    


BitBoard::BitBoard()
{
}


void  BitBoard::setBit(uint64_t &x, int pos)
{
    x |= (1ULL << pos);
}

void BitBoard::clearBit(uint64_t &x, int pos)
{
    x &= ~(1ULL << pos);
}

void BitBoard::updateAllPieces()
{
    allPieces = whitePieces | blackPieces;
}

void BitBoard::updateWhitePieces()
{
    whitePieces = whitePawns->pawns | whiteRooks->rooks | whiteBishops->bishops | whiteKnights->knights | whiteQueens->queens | whiteKing->king;
}

void BitBoard::updateBlackPieces()
{
    blackPieces = blackPawns->pawns | blackRooks->rooks | blackBishops->bishops | blackKnights->knights | blackQueens->queens | blackKing->king;
}
