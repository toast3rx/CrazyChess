#ifndef KNIGHT_H
#define KNIGHT_H

#include <bits/stdc++.h>
#include <vector>
#include <string>
#include "../Move.h"
#include "../Utils.h"


class Knights {
	public:
		uint64_t knights;
		uint64_t knightsMoves[64];

		Knights(uint64_t _pawns);

        uint64_t generateMoves(int square);

        void convertMoves ();
};

#endif