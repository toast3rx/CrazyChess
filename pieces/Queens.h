#ifndef QUEEN_H
#define QUEEN_H

#include <bits/stdc++.h>
class BitBoard;
class Queens {
public:
    uint64_t queens;

    Queens(uint64_t _queens);

    void Move();
};

#endif
