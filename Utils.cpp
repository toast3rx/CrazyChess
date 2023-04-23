#include "Utils.h"

uint64_t Utils::getFile(int f) {
	return (0x0101010101010101ULL << (f - 1));
}

uint64_t Utils::getRank(int r) {
	return (0xffULL << (8 * (r - 1)));
}

int Utils::popLsb(uint64_t &n) {
    int LsbIndex = __builtin_ffsll(n) - 1;
    n = n & (n - 1);
    return LsbIndex;
}

int Utils::getBits(uint64_t n) {
    return getOneBitsPositions(n).size();
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

/**
 * @brief Print a bitboard with a1 on the bottom left corner
 * 		and h8 on the top right corner in a file
 * @param board  bitboard to print
 * @param fileName name of the file to print the board
 */
void Utils::printBoard(uint64_t board, std::string fileName)
{
	std::ofstream fout(fileName, std::ios::app);

	int64_t aux_row;
	int64_t aux_column;

		// sunt pe pozitia i, afisez tabla cu pozitii de aici.
		for (aux_row = 8 - 1; aux_row >= 0; --aux_row) {
			for (aux_column = 0; aux_column < 8; ++aux_column) {
				fout << ((board & (1ULL << ((aux_row) * 8 + aux_column))) > 0) << " ";
			}
			fout << "\n";
		}
		fout << "\n";

	fout.close();
}
std::pair < std::string, std::string > Utils::convert_table_to_positions(uint64_t source, uint64_t &goodSquares, uint64_t myPieces)
{
    uint64_t goodPos = Utils::popLsb(goodSquares);
    if ((1ULL << goodPos) & myPieces) {
        return std::make_pair("a", "a");
    }
    return std::make_pair(Utils::bitToPos(source), Utils::bitToPos(goodPos));
}

std::uint64_t Utils::posToBit(std::string pos)
{	
	int row = pos[1] - '1';
	int file = pos[0] - 'a';

	return (1ULL << (row * 8 + file));
}


