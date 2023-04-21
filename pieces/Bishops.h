#ifndef BISHOPS_H
#define BISHOPS_H
#define SQUARES 64
#define ROWS 8
#define COLUMNS 8
#include <bits/stdc++.h>

class Bishops {
public:
    uint64_t bishops;
    uint64_t possible_moves[64];

    Bishops(uint64_t _bishops);

    void Move();
};

#endif