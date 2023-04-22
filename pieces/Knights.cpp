#include "Knights.h"
#include <string>


// approved by mihai
uint64_t getRank(int r) {
    return (0xffULL << (8 * (r - 1)));
}

// approved by mihai
// file index starts from 1
uint64_t getFile(int f) {

    return (0x0101010101010101ULL << (f - 1));
}

uint64_t Knights::generateMoves(int square) {
    uint64_t knight = 1ULL << square;

	uint64_t attacks = 0;
	uint64_t FILE_A = getFile(1);
	uint64_t FILE_B = getFile(2);
	
	uint64_t FILE_G = getFile(7);
	uint64_t FILE_H = getFile(8);

    uint64_t RANK_8 = getRank(8);
    uint64_t RANK_7 = getRank(7);

    uint64_t RANK_1 = getRank(1);
    uint64_t RANK_2 = getRank(2);


    // two right, one up (ex: a1 -> c2)
    if ((knight & ~FILE_G & ~FILE_H & ~RANK_8) != 0) attacks |= knight << 10;

    // two right, one down (ex: c2 -> a1)
    if ((knight & ~FILE_G & ~FILE_H & ~RANK_1) != 0) attacks |= knight >> 6;

    // two left, one up (ex: h1 -> f2)
    if ((knight & ~FILE_A & ~FILE_B & ~RANK_8) != 0) attacks |= knight << 6;

    // two left, one down
    if ((knight & ~FILE_A & ~FILE_B & ~RANK_1) != 0) attacks |= knight >> 10;

    // one right, two up (ex: a1 -> b3)
    if ((knight & ~FILE_H & ~RANK_7 & ~RANK_8) != 0) attacks |= knight << 17;

    // one right. two down
    if ((knight & ~FILE_H & ~RANK_2 & ~RANK_1) != 0) attacks |= knight >> 15;

    // one left, two up
    if ((knight & ~FILE_A & ~RANK_7  & ~RANK_8) != 0) attacks |= knight << 15;

    // one left, two down
    if ((knight & ~FILE_A & ~RANK_2 & ~RANK_1) != 0) attacks |= knight >> 17;

    return attacks;
}

Knights::Knights(uint64_t _knights) : knights(_knights) {
    for (int i = 0; i < 64; ++i) {
        knightsMoves[i] = generateMoves(i);
    }
}

// Remove in the future, keep for individual test
std::vector<int> getOneBitsPositions(uint64_t number) {
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

std::string bitToPos(uint64_t bit) {

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
 * Convert all available moves from bitboards
 *  to string coordinates (a1, b1, h6, etc)
 */
void Knights::convertMoves() {

//    std::vector<Move *> res;
    // Get Positions of bits where the knights are situated
    std::vector<int> knightsPos = getOneBitsPositions(knights);

    for (int i = 0; i < knightsPos.size(); ++i) {

        std::string source = bitToPos(knightsPos[i]);

        uint64_t attacksNi = knightsMoves[knightsPos[i]];

        std::vector<int> attacksPos = getOneBitsPositions(attacksNi);

        for (int j = 0; j < attacksPos.size(); ++j) {
//            res.push_back(Move::moveTo(source, bitToPos(attacksPos[j])));
                std::cout << "src: " << source << " dest: " << bitToPos(attacksPos[j]) << "\n";
        }
    }
//    return res;
}


