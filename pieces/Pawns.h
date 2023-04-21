#ifndef PAWNS_H
#define PAWNS_H
#include <bits/stdc++.h>

class Pawns {
public:
    uint64_t pawns;

    Pawns(uint64_t _pawns);

    void promote();
    void Move();
};
#endif
