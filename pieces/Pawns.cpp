#include "Pawns.h"
#include "bits/stdc++.h"

Pawns::Pawns(uint64_t _pawns) : pawns(_pawns) {}

void Pawns::getMoves(PlaySide side, 
								    uint64_t blackPieces, 
									uint64_t whitePieces, 
									uint64_t allPieces, 
									uint64_t enPassantWhite, 
									uint64_t enPassantBlack,
									std::vector<Move *> &allMoves)
{
	std::vector<Move *> moves;
	getAttacks(moves, side, blackPieces, whitePieces, enPassantWhite, enPassantBlack);
	getPush(moves, side, allPieces);
	allMoves.insert(allMoves.end(), moves.begin(), moves.end());
	// return moves;
}

void Pawns::getAttacks(std::vector<Move *> &moves, PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t enPassantWhite, uint64_t enPassantBlack)
{
	uint64_t westAttacks;
	uint64_t eastAttacks;

	if (side == PlaySide::WHITE) {
		eastAttacks = (pawns << 9) & ~Utils::getFile(1);
		westAttacks = (pawns << 7) & ~Utils::getFile(8);
	} else {
		eastAttacks = (pawns >> 7) & ~Utils::getFile(1);
		westAttacks = (pawns >> 9) & ~Utils::getFile(8);
	}

	eastAttacks &= ((side == PlaySide::BLACK) ? whitePieces | enPassantWhite : blackPieces | enPassantBlack);
	westAttacks &= ((side == PlaySide::BLACK) ? whitePieces | enPassantWhite : blackPieces | enPassantBlack);

	for (int i = 0; i < 64; i++) {
		uint64_t bitEast = (1ULL << i) & eastAttacks;
		uint64_t bitWest = (1ULL << i) & westAttacks;
		int rank = i / 8 + 1;
		std::string next = Utils::bitToPos(i);

		if (bitEast) {
			std::string prev = Utils::bitToPos(i + (side == PlaySide::BLACK ? 7 : -9));
			if (rank != (side == PlaySide::BLACK ? 1 : 8)) {
				moves.push_back(Move::moveTo(prev, next));
			} else {
				moves.push_back(Move::promote(prev, next, Piece::QUEEN));
				moves.push_back(Move::promote(prev, next, Piece::BISHOP));
				moves.push_back(Move::promote(prev, next, Piece::ROOK));
				moves.push_back(Move::promote(prev, next, Piece::KNIGHT));
			} 
				
		}

		if (bitWest) {
			std::string prev = Utils::bitToPos(i + (side == PlaySide::BLACK ? 9 : -7));
			if (rank != (side == PlaySide::BLACK ? 1 : 8)) {
				moves.push_back(Move::moveTo(prev, next));
			} else {
				moves.push_back(Move::promote(prev, next, Piece::QUEEN));
				moves.push_back(Move::promote(prev, next, Piece::BISHOP));
				moves.push_back(Move::promote(prev, next, Piece::ROOK));
				moves.push_back(Move::promote(prev, next, Piece::KNIGHT));
			} 
		}
	}
}

void Pawns::getPush(std::vector<Move *> &moves, PlaySide side, uint64_t allPieces)
{
	for (int i = 8; i < 56; i++) {
		uint64_t bit = (1ULL << i) & pawns;
		int rank = i / 8 + 1;

		if ((((side == PlaySide::WHITE) && !(allPieces & (1ULL << (i + 8))))
		 	|| ((side == PlaySide::BLACK) && !(allPieces & (1ULL << (i - 8))))) && bit) {	
			std::string prev = Utils::bitToPos(i);
			std::string next = Utils::bitToPos(i + 8 * (side == PlaySide::BLACK ? -1 : 1));

			if (rank != (side == PlaySide::BLACK ? 2 : 7)) {
				moves.push_back(Move::moveTo(prev, next));
			} else {
				moves.push_back(Move::promote(prev, next, Piece::QUEEN));
				moves.push_back(Move::promote(prev, next, Piece::BISHOP));
				moves.push_back(Move::promote(prev, next, Piece::ROOK));
				moves.push_back(Move::promote(prev, next, Piece::KNIGHT));
			}
		}

		if (rank == (side == PlaySide::BLACK ? 7 : 2)
			&& (((side == PlaySide::WHITE) && !(allPieces & (1ULL << (i + 8))))
		 	|| ((side == PlaySide::BLACK) && !(allPieces & (1ULL << (i - 8)))))
			&& (((side == PlaySide::WHITE) && !(allPieces & (1ULL << (i + 16))))
		 	|| ((side == PlaySide::BLACK) && !(allPieces & (1ULL << (i - 16))))) 
			&& bit) {
			std::string prev = Utils::bitToPos(i);
			std::string next = Utils::bitToPos(i + 16 * (side == PlaySide::BLACK ? -1 : 1));
			moves.push_back(Move::moveTo(prev, next));
		}
	}
}

uint64_t Pawns::getAllAttacks(PlaySide side) {
	uint64_t westAttacks;
	uint64_t eastAttacks;

	if (side == PlaySide::WHITE) {
		eastAttacks = (pawns << 9) & ~Utils::getFile(1);
		westAttacks = (pawns << 7) & ~Utils::getFile(8);
	} else {
		eastAttacks = (pawns >> 7) & ~Utils::getFile(1);
		westAttacks = (pawns >> 9) & ~Utils::getFile(8);
	}


	return eastAttacks | westAttacks;
}