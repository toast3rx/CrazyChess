//
// Created by toaster on 21.04.2023.
//
#include "../Knights.h"
#include    <fstream>

int main() {

	//out file
	std::ofstream fout("knightMoves.txt");

   Knights *knights = new Knights(0x42ULL);

   int64_t aux_row;
   int64_t aux_column;
   for (int64_t i = 0; i < 64; ++i) {
//
       fout << Utils::bitToPos(i) << "\n";
       // sunt pe pozitia i, afisez tabla cu pozitii de aici.
       for (aux_row = 8 - 1; aux_row >= 0; --aux_row) {
           for (aux_column = 0; aux_column < 8; ++aux_column) {
               if (aux_row * 8 + aux_column == i) {
                   fout << "X ";
                   continue;
               }
               fout << ((knights->knightsMoves[i] & (1ULL << ((aux_row) * 8 + aux_column))) > 0) << " ";
           }
           fout << "\n";
        }
       fout << "\n";
   }
//
//    int bit = 2UL;
//    int rank = bit / 8;
//    int file = bit % 8;
//
//    char fileChar = 'a' + file;
//    char rankChar = '1' + rank;
//
//
//    printf("%c%c\n", fileChar, rankChar);

   return 0;
}