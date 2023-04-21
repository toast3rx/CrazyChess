#include "BitBoard.h"

uint64_t static getFile(int f)
{
	return (0x8080808080808080ULL >> (f - 1));
}
int main() {
	printf("%llu\n", BitBoard.getRank(0));
}