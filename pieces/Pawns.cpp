#include "Pawns.h"
#include "bits/stdc++.h"

Pawns::Pawns(uint64_t _pawns) : pawns(_pawns) {}

std::vector<Move*> Pawns::getMoves(PlaySide side) {
    std::vector<Move*> moves;
    getAttacks(moves, side);
    getPush(moves, side);
    getPromotion(moves, side);
    getEnPassant(moves, side);
    return moves;
}

void Pawns::getAttacks(std::vector<Move *> moves, PlaySide side) {
    uint64_t westAttacks;
    uint64_t eastAttacks;

    if (side == PlaySide::WHITE) {
        eastAttacks = (pawns << 7) & ~BitBoard::getFile(1);
        westAttacks = (pawns << 9) & ~BitBoard::getFile(8);
    } else {
        eastAttacks = (pawns >> 9) & ~BitBoard::getFile(8);
        westAttacks = (pawns >> 7) & ~BitBoard::getFile(1);
    }

    eastAttacks &= side == PlaySide::BLACK ? BitBoard::blackPieces : BitBoard::whitePieces;
    westAttacks &= side == PlaySide::BLACK ? BitBoard::blackPieces : BitBoard::whitePieces;

    for (int i = 8; i < 56; i++) {
        uint64_t bitEast = (1ULL << i) & eastAttacks;
        uint64_t bitWest = (1ULL << i) & westAttacks;
        int file = i % 8;
        int rank = i / 8 + 1;
        std::string next = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank + 1);

        if (bitEast) {
            std::string prev = std::to_string(Bot::numberToFiles[file - 1]) + std::to_string(rank + 1 * PlaySide::BLACK ? 1 : -1);
            moves.push_back(Move::moveTo(prev, next));
        }

        if (bitWest) {
            std::string prev = std::to_string(Bot::numberToFiles[file + 1]) + std::to_string(rank + 1 * PlaySide::BLACK ? 1 : -1);
            moves.push_back(Move::moveTo(prev, next));
        }
    }
}

void Pawns::getPush(std::vector<Move *> moves, PlaySide side) {
    uint64_t pawnMoves = pawns & ~BitBoard::getRank(7);

    for (int i = 8; i < 56; i++) {
        uint64_t bit = (1ULL << i) & pawnMoves;
        int file = i % 8;
        int rank = i / 8 + 1;

        if (!(BitBoard::allPieces & (1ULL << (i + 8)))) {
            std::string prev = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank);
            std::string next = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank + 1);
            moves.push_back(Move::moveTo(prev, next));
        }

        if (!(BitBoard::allPieces & (1ULL << (i + 16))) && rank == 1) {
            std::string prev = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank);
            std::string next = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank + 2);
            moves.push_back(Move::moveTo(prev, next));
        }
    }
}

void Pawns::getPromotion(std::vector<Move *> moves, PlaySide side) {

}

void Pawns::getEnPassant(std::vector<Move *> moves, PlaySide side) {

};

