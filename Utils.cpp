#include "Utils.h"

uint64_t Utils::getFile(int f) {
	return (0x0101010101010101ULL << (f - 1));
}

uint64_t Utils::getRank(int r) {
	return (0xffULL << (8 * (r - 1)));
}

/**
 *  Return bit string notation from bitboard to human readable string notation
 *  (ex: 1 -> b1)
 * @param bit
 * @return
 */
std::string Utils::bitToPos(uint64_t bit)
{
	std::string res = "";

	int row = bit / 8;
	int file = bit % 8;

	char rowChar = '1' + row;
	char fileChar = 'a' + file;

	res += fileChar;
	res += rowChar;

	return res;
}

/**
 * Get position of all the 1s in a bitboard
 * @param number chess bitboard
 * @return array of 1s positions
 */
std::vector<int> Utils::getOneBitsPositions(uint64_t number)
{
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

