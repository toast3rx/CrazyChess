#include "Knights.h"


uint64_t Knights::generateMoves(int square)
{
    uint64_t knight = 1ULL << square;

    uint64_t attacks = 0;
    uint64_t FILE_A = Utils::getFile(1);
    uint64_t FILE_B = Utils::getFile(2);

    uint64_t FILE_G = Utils::getFile(7);
    uint64_t FILE_H = Utils::getFile(8);

    uint64_t RANK_8 = Utils::getRank(8);
    uint64_t RANK_7 = Utils::getRank(7);

    uint64_t RANK_1 = Utils::getRank(1);
    uint64_t RANK_2 = Utils::getRank(2);

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
    if ((knight & ~FILE_A & ~RANK_7 & ~RANK_8) != 0) attacks |= knight << 15;

    // one left, two down
    if ((knight & ~FILE_A & ~RANK_2 & ~RANK_1) != 0) attacks |= knight >> 17;

    return attacks;
}

Knights::Knights(uint64_t _knights) : knights(_knights)
{
    for (int i = 0; i < 64; ++i) {
        knightsMoves[i] = generateMoves(i);
    }
}

/**
 * Convert all available moves from bitboards
 *  to string coordinates (a1, b1, h6, etc)
 */
void Knights::getAttacks(std::vector<Move *> &moves, PlaySide side, uint64_t blackPieces, uint64_t whitePieces)
{

    uint64_t allyPieces = (side == PlaySide::WHITE) ? whitePieces : blackPieces;

    //    std::vector<Move *> res;
        // Get Positions of bits where the knights are situated
    std::vector<int> knightsPos = Utils::getOneBitsPositions(knights);

    for (int i = 0; i < knightsPos.size(); ++i) {

        std::string source = Utils::bitToPos(knightsPos[i]);

        uint64_t allAtacksForI = knightsMoves[knightsPos[i]];

        uint64_t availableAttacks = allAtacksForI & ~allyPieces;

        std::vector<int> attacksPos = Utils::getOneBitsPositions(availableAttacks);

        for (int j = 0; j < attacksPos.size(); ++j) {
            moves.push_back(Move::moveTo(source, Utils::bitToPos(attacksPos[j])));
        }
    }
}

/**
 * @brief Retrn all available moves for the knights
 * 
 * @param side  side of the bot
 * @param blackPieces  
 * @param whitePieces 
 * @param allPieces 
 * @return std::vector<Move *> 
 */
std::vector<Move *> Knights::getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces)
{
    std::vector<Move *> result;
    getAttacks(result, side, blackPieces, whitePieces);
    return result;
}


