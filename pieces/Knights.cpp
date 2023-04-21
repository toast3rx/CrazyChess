#include "Knights.h"
#include "bits/stdc++.h"
#inclde "BitBoard.h"

Knights::Knights(uint64_t _knights) : knightsBitBoard(_knights) {}


uint64_t generateMoves(int square) {
	uint64_t knight = 1 << square;

	uint64_t attacks = 0;
	uint64_t FILE_A = BitBoard.getFile(1);
	uint64_t FILE_B = BitBoard.getFile(2);
	
	uint64_t FILE_G = BitBoard.getFile(7);
	uint64_t FILE_H = BitBoard.getFile(8);

	// two right, one up
	if ((knight & ~FILE_A & ~FILE_B) != 0) {
		attacks |= (knight << 6);
	}


	if ((knight & ~ & ~FILE_B) != 0)

}
