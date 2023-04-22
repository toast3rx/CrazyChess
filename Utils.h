#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <string>

// define class with static methods
class Utils {
public:
	static uint64_t getFile(int f);
	static uint64_t getRank(int r);
	static std::string bitToPos(uint64_t bit);
	static std::vector<int> getOneBitsPositions(uint64_t number);
};
#endif