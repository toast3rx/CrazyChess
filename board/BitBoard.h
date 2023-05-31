#ifndef BITBOARD_H
#define BITBOARD_H
#include <bits/stdc++.h>
#include "../pieces/pawn/Pawns.h"
#include "../pieces/rook/Rooks.h"
#include "../pieces/knight/Knights.h"
#include "../pieces/bishop/Bishops.h"
#include "../pieces/queen/Queens.h"
#include "../pieces/king/King.h"

// in the bitboard bit 0 is a1 and bit 63 is h8
class BitBoard {
 public:

   // white pieces
   static  Pawns *whitePawns ;
   static  Rooks* whiteRooks;
   static  Bishops* whiteBishops;
   static  Knights* whiteKnights;
   static  Queens* whiteQueens;
   static  King* whiteKing;

  //  // black pieces
   static  Pawns* blackPawns;
   static  Rooks* blackRooks;
   static  Bishops* blackBishops;
   static  Knights* blackKnights;
   static  Queens* blackQueens;
   static  King* blackKing;

   static uint64_t blackPieces;
   static uint64_t whitePieces;
   static uint64_t allPieces;

   static uint64_t enPassantWhite;
   static uint64_t enPassantBlack;

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
