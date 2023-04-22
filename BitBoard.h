#ifndef BITBOARD_H
#define BITBOARD_H
#include <bits/stdc++.h>
#include "./pieces/Pawns.h"
#include "./pieces/Knights.h"
#include "./pieces/Rooks.h"
#include "./pieces/Bishops.h"
#include "./pieces/Queens.h"
#include "./pieces/King.h"

// in the bitboard bit 0 is a1 and bit 63 is h8
class BitBoard {
 public:

   // white pieces
   static class Pawns* whitePawns;
   static class Rooks* whiteRooks;
   static class Bishops* whiteBishops;
   static class Knights* whiteKnights;
   static class Queens* whiteQueens;
   static class King* whiteKing;

   // black pieces
   static class Pawns* blackPawns;
   static class Rooks* blackRooks;
   static class Bishops* blackBishops;
   static class Knights* blackKnights;
   static class Queens* blackQueens;
   static class King* blackKing;

   static uint64_t blackPieces;
   static uint64_t whitePieces;
   static uint64_t allPieces;

   uint64_t static getRank(int r);
   uint64_t static getFile(int f);

   void static setBit(uint64_t &x, int pos);
   void static clearBit(uint64_t &x, int pos);
   void static updateBlackPieces();
   void static updateWhitePieces();
   void static updateAllPieces();

   std::string static bitToPos(uint64_t bit);
    std::vector<int> static getOneBitsPositions(uint64_t number);

    // constructor
   BitBoard();

};
#endif
