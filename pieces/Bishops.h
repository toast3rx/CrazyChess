#ifndef BISHOPS_H
#define BISHOPS_H
#define SQUARES 64
#define ROWS 8
#define COLUMNS 8
#include <bits/stdc++.h>
#include "../PlaySide.h"
#include "../Move.h"
#include "../Utils.h"

class Bishops {
public:
    uint64_t bishops;

    static uint64_t all_moves[SQUARES], legal_moves[512], blockers[512], bishopAttacks[SQUARES][SQUARES * 8];

    Bishops(uint64_t _bishops);

    void getMoves(PlaySide side,
        uint64_t blackPieces,
        uint64_t whitePieces,
        uint64_t allPieces,
        std::vector<Move *> &allMoves
    );

    // std::pair < std::string, std::string > convert_table_to_positions(uint64_t source, uint64_t &goodSquares);
    uint64_t getAllAttacks(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces);
    void debug(uint64_t matrix);

    int64_t count_set_bits(uint64_t n);

    uint64_t generate_legal_moves(uint64_t square, uint64_t blockers);

    void ripple_carry(uint64_t square, uint64_t all_moves);

    uint64_t generate_all_moves(uint64_t square);

    uint64_t gen_number();

    void init();

    constexpr static const int64_t bishop_bits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
    };

    constexpr static const uint64_t bishopMagic[64] = {
        18018832060792964ULL, //0
        9011737055478280ULL, //1
        10347943761865012333ULL, //2
        10998938195575784868ULL, //3
        7956748525303704411ULL, //4
        18031700236641835792ULL, //5
        1189093273034424848ULL, //6
        270357282336932352ULL, //7
        10006224325097106190ULL, //8
        2267763835016ULL, //9
        2652629010991292674ULL, //10
        3546096678203420275ULL,
        17359108340835127981ULL,
        12881015096104330159ULL,
        288591295206670341ULL,
        576743344005317120ULL,
        18016669532684544ULL,
        5300762462814072912ULL,
        7954485487732523753ULL,
        4789579870309065244ULL,
        11123751115238155413ULL, //20
        3643975679990966818ULL,
        16814832921411457085ULL,
        4904774771507990871ULL,
        153157607757513217ULL,
        6445543949133956131ULL,
        12307214133066695312ULL,
        5336769962121892128ULL,
        7180145251292561440ULL,
        13477942455076720659ULL,
        64176571732267010ULL, //30
        2883994442349787136ULL,
        12221565824535126022ULL,
        1281281035057075400ULL,
        17006441046234761216ULL,
        9675423050623352960ULL, //35
        //2305913523989908488ULL, // 35
        //9675423050623352960ULL, // 36
        9223945990515460104ULL, // 36
        11347532921517179534ULL,
        6084669920662849585ULL,
        1632283111001038382ULL,
        3745455711285981674ULL, //40
        15066537569854514296ULL,
        15549671816024920068ULL,
        14641502197028752389ULL,
        11945503501565715457ULL,
        18176551888390997120ULL,
        2495038618901754118ULL,
        9228440811230794258ULL,
        360435373754810368ULL,
        108227545293391872ULL,
        16035429330847284879ULL, //50
        2317207584112111218ULL,
        3319518482661643128ULL,
        17930820645405082470ULL,
        6522483364660839184ULL,
        1127033795058692ULL,
        2815853729948160ULL,
        317861208064ULL,
        1628769708709515315ULL,
        7888417619712190480ULL,
        14859242225265869825ULL, //60
        5515347926142181645ULL,
        9878791228517523968ULL,
        4616190786973859872ULL,
    };
};

#endif