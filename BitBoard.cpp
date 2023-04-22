#include "BitBoard.h"



BitBoard::BitBoard() {

    whitePawns = new Pawns(0xff00ULL);
    blackPawns = new Pawns(0xff000000000000ULL);

    whiteRooks = new Rooks(0x81ULL);
    blackRooks = new Rooks(0x8100000000000000ULL);

    whiteBishops = new Bishops(0x24ULL);
    blackBishops = new Bishops(0x2400000000000000ULL);

    // Initialize the knights
    whiteKnights = new Knights(0x42ULL);
    blackKnights = new Knights(0x4200000000000000ULL);

    whiteQueens = new Queens(0x8ULL);
    blackQueens = new Queens(0x8000000000000000ULL);

    whiteKing = new King(0x10ULL);
    blackKing = new King(0x1000000000000000ULL);

    blackPieces = blackPawns->pawns | blackRooks->rooks | blackBishops->bishops | blackKnights->knights | blackQueens->queens | blackKing->king;

    updateWhitePieces();
    updateBlackPieces();
    updateAllPieces();
}


// approved by mihai
uint64_t  BitBoard::getRank(int r) {
    return (0xffULL << (8 * (r - 1)));
}

// approved by mihai
// file index starts from 1
uint64_t  BitBoard::getFile(int f) {
    return (0x0101010101010101ULL << (f - 1));
}

void  BitBoard::setBit(uint64_t &x, int pos) {
    x |= (1ULL << pos);
}

void BitBoard::clearBit(uint64_t &x, int pos) {
    x &= ~(1ULL << pos);
}

void BitBoard::updateAllPieces() {
    allPieces = whitePieces | blackPieces;
}

void BitBoard::updateWhitePieces() {
    whitePieces = whitePawns->pawns | whiteRooks->rooks | whiteBishops->bishops | whiteKnights->knights | whiteQueens->queens | whiteKing->king;
}

void BitBoard::updateBlackPieces() {
    blackPieces = blackPawns->pawns | blackRooks->rooks | blackBishops->bishops | blackKnights->knights | blackQueens->queens | blackKing->king;
}

/**
 *  Return bit string notation from bitboard to human readable string notation
 *  (ex: 1 -> b1)
 * @param bit
 * @return
 */
std::string BitBoard::bitToPos(uint64_t bit) {

    std::string res = "";

    int row = bit / 8;
    int file = bit % 8;

    char rowChar = '1' + row;
    char fileChar ='a' + file;

    res += fileChar;
    res+= rowChar;

    return res;
}

/**
 * Get position of all the 1s in a bitboard
 * @param number chess bitboard
 * @return array of 1s positions
 */
std::vector<int> BitBoard::getOneBitsPositions(uint64_t number) {
    std::vector<int> positions;

    int position = 0;
    while (number != 0) {
        if (number & 1) {
            positions.push_back(position);
        }
        number >>= 1;
        ++position;
    }

    return positions;
}
