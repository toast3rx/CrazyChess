
#include "Bishops.h"

uint64_t Bishops::blockers[512] = { 0 };
uint64_t Bishops::legal_moves[512] = { 0 };
uint64_t Bishops::bishopAttacks[SQUARES][SQUARES * 8] = {{0}};
uint64_t Bishops::all_moves[SQUARES] = {0};

uint64_t Bishops::generate_legal_moves(uint64_t square, uint64_t blockers)
{
    uint64_t legal_moves = 0, new_square = square;
    int64_t iterations;
    int64_t row = square / ROWS;
    int64_t column = square % COLUMNS;

    // dreapta sus
    new_square = square;
    iterations = std::min(ROWS - row - 1, COLUMNS - column - 1);
    iterations = std::max((int64_t)0, iterations);
    for (int64_t i = 0; i < iterations; ++i) {
        new_square += 9;
        legal_moves |= (1ULL << new_square);
        if (blockers & (1ULL << new_square)) {
            break;
        }
    }

    // stanga sus
    new_square = square;
    iterations = std::min(ROWS - row - 1, column);
    iterations = std::max((int64_t)0, iterations);
    for (int64_t i = 0; i < iterations; ++i) {
        new_square += 7;
        legal_moves |= (1ULL << new_square);
        if (blockers & (1ULL << new_square)) {
            break;
        }
    }

    // dreapta jos
    new_square = square;
    iterations = std::min(row, COLUMNS - column - 1);
    iterations = std::max((int64_t)0, iterations);
    for (int64_t i = 0; i < iterations; ++i) {
        new_square -= 7;
        legal_moves |= (1ULL << new_square);
        if (blockers & (1ULL << new_square)) {
            break;
        }
    }

    // stanga jos
    new_square = square;
    iterations = std::min(row, column);
    iterations = std::max((int64_t)0, iterations);
    for (int64_t i = 0; i < iterations; ++i) {
        new_square -= 9;
        legal_moves |= (1ULL << new_square);
        if (blockers & (1ULL << new_square)) {
            break;
        }
    }

    return legal_moves;
}

int32_t get_key(uint64_t square, uint64_t blockers)
{
    return (Bishops::bishopMagic[square] * blockers) >> (SQUARES - Bishops::bishop_bits[square]);
}

int64_t Bishops::count_set_bits(uint64_t n)
{
    int64_t sol = 0;

    while (n) {
        sol += n & 1;
        n >>= 1;
    }
    return sol;
}

void Bishops::ripple_carry(uint64_t square, uint64_t all_moves)
{
    uint64_t subset = 0;
    int64_t set_bits = count_set_bits(all_moves);

    memset(legal_moves, 0, sizeof(legal_moves));
    for (uint64_t i = 0; i < (1ULL << set_bits); ++i) {
        Bishops::blockers[i] = subset;
        Bishops::legal_moves[i] = generate_legal_moves(square, Bishops::blockers[i]);
        Bishops::bishopAttacks[square][get_key(square, subset)] = Bishops::legal_moves[i];
        //debug(legal_moves[i]);
        subset = (subset - all_moves) & all_moves;
    }
}

uint64_t Bishops::generate_all_moves(uint64_t square)
{
    uint64_t all_moves = 0ULL;
    int64_t row = square / ROWS;
    int64_t column = square % COLUMNS;
    int64_t new_square;
    int64_t iterations;

    // dreapta sus
    new_square = square;
    iterations = std::min(ROWS - row - 2, COLUMNS - column - 2);
    iterations = std::max((int64_t)0, iterations);
    for (int64_t i = 0; i < iterations; ++i) {
        new_square += 9;
        all_moves |= (1ULL << new_square);
    }

    // stanga sus
    new_square = square;
    iterations = std::min(ROWS - row - 2, column - 1);
    iterations = std::max((int64_t)0, iterations);
    for (int64_t i = 0; i < iterations; ++i) {
        new_square += 7;
        all_moves |= (1ULL << new_square);
    }

    // dreapta jos
    new_square = square;
    iterations = std::min(row - 1, COLUMNS - column - 2);
    iterations = std::max((int64_t)0, iterations);
    for (int64_t i = 0; i < iterations; ++i) {
        new_square -= 7;
        all_moves |= (1ULL << new_square);
    }

    // stanga jos
    new_square = square;
    iterations = std::min(row - 1, column - 1);
    iterations = std::max((int64_t)0, iterations);
    for (int64_t i = 0; i < iterations; ++i) {
        new_square -= 9;
        all_moves |= (1ULL << new_square);
    }
    return all_moves;
}

void Bishops::init()
{
    for (int64_t square = 0; square < SQUARES; ++square) {
        all_moves[square] = generate_all_moves(square);
        ripple_carry(square, all_moves[square]);
    }
}

Bishops::Bishops(uint64_t _bishops) : bishops(_bishops) {
    init();
}

void Bishops::debug(uint64_t n)
{
    int64_t aux_row;
    int64_t aux_column;

    for (aux_row = ROWS - 1; aux_row >= 0; --aux_row) {
            std::cout << aux_row + 1 << "  ";
        for (aux_column = 0; aux_column < COLUMNS; ++aux_column) {
            std::cout << ((n & (1ULL << ((aux_row) * ROWS + aux_column))) > 0) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   A B C D E F G H\n";
}

uint64_t Bishops::gen_number()
{
    uint64_t rand_nr[] = {0ULL, 0ULL, 0ULL, 0ULL};
    uint64_t sol = 0;

    for (int i = 0; i < 4; ++i) {
        rand_nr[i] = (uint64_t)rand() & 0xFFFF;
        sol |= (rand_nr[i] << (i * 16));
    }
    return sol;
}


uint64_t popLSB(uint64_t &n)
{
    uint64_t pos = 0ULL;
    while (!((1ULL << pos) & n)) {
        ++pos;
    }
    n &= (n - 1);

    return pos;
}

std::pair < std::string, std::string > convert_table_to_positions(uint64_t source, uint64_t &goodSquares, uint64_t myPieces)
{
    uint64_t goodPos = popLSB(goodSquares);
    if ((1 << goodPos) & myPieces) {
        return std::make_pair("RAZVAN", "MATRIX");
    }
    return std::make_pair(Utils::bitToPos(source), Utils::bitToPos(goodPos));
}

std::vector<Move*> Bishops::getMoves(PlaySide side, uint64_t blackPieces, uint64_t whitePieces, uint64_t allPieces)
{
    std::vector< Move* > sol;
    uint64_t myPieces = 0;
    if (side == 0) {
        myPieces = blackPieces;
    } else {
        myPieces = whitePieces;
    }
    uint64_t currentBishops = bishops;

    while (currentBishops) {
        uint64_t bishopPos = popLSB(currentBishops);
        uint64_t blockers = allPieces & Bishops::all_moves[bishopPos];
        uint64_t possibleMoves = Bishops::bishopAttacks[bishopPos][get_key(bishopPos, blockers)];
        while (possibleMoves) {
            std::pair < std::string, std::string > new_pair = convert_table_to_positions(bishopPos, possibleMoves, myPieces);
            if (!new_pair.first.compare("RAZVAN")) {
                sol.push_back(Move::moveTo(new_pair.first, new_pair.second));
            }
        }
    }

    return sol;
}
