#include "Pawns.h"
#include "bits/stdc++.h"

Pawns::Pawns(uint64_t _pawns) : pawns(_pawns) {}

std::vector<Move *> Pawns::getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces)
{
	std::vector<Move *> moves;
	getAttacks(moves, side, whitePieces, blackPieces);
	getPush(moves, side, allPieces);
	getPromotion(moves, side);
	getEnPassant(moves, side);
	return moves;
}

void Pawns::getAttacks(std::vector<Move *> &moves, PlaySide side, uint64_t blackPieces, uint64_t whitePieces)
{
	uint64_t westAttacks;
	uint64_t eastAttacks;

	if (side == PlaySide::WHITE) {
		eastAttacks = (pawns << 7) & ~Utils::getFile(1);
		westAttacks = (pawns << 9) & ~Utils::getFile(8);
	} else {
		eastAttacks = (pawns >> 9) & ~Utils::getFile(8);
		westAttacks = (pawns >> 7) & ~Utils::getFile(1);
	}

	eastAttacks &= side == PlaySide::BLACK ? blackPieces : whitePieces;
	westAttacks &= side == PlaySide::BLACK ? blackPieces : whitePieces;

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

void Pawns::getPush(std::vector<Move *> &moves, PlaySide side, uint64_t allPieces)
{
	uint64_t pawnMoves = pawns & ~Utils::getRank(7);

	for (int i = 8; i < 56; i++) {
		uint64_t bit = (1ULL << i) & pawnMoves;
		int file = i % 8;
		int rank = i / 8 + 1;

		if (!(allPieces & (1ULL << (i + 8)))) {
			// std::string prev = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank);
			// std::string next = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank + 1);

			// Codul de sus nu merge, to_string se comporta dubios
			// Cel de jos merge foarte bine
			// Approved by Mihai
			std::string prev = "";
			char fileChar = file + 'a';
			char rankChar = rank + '0';

			prev.append(1, fileChar);
			prev.append(1, rankChar);

			std::string next = "";
			char rankChar2 = rank + 1 + '0';
			next.append(1, fileChar);
			next.append(1, rankChar2);
			
			moves.push_back(Move::moveTo(prev, next));
		}

		if (!(allPieces & (1ULL << (i + 16))) && rank == 1) {
			std::string prev = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank);
			std::string next = std::to_string(Bot::numberToFiles[file]) + std::to_string(rank + 2);

			moves.push_back(Move::moveTo(prev, next));
		}
	}
}

void Pawns::getPromotion(std::vector<Move *> &moves, PlaySide side)
{

}

void Pawns::getEnPassant(std::vector<Move *> &moves, PlaySide side)
{

};

