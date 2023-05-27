#include "Bot.h"

extern PlaySide engineSide;

std::ofstream fout("log.txt", std::ios::app);

const std::string Bot::BOT_NAME = "SuperUltraMegaAlphaCrazyHouseMatrixGiobaniDestroyer"; /* Edit this, escaped characters are forbidden */
std::unordered_map<char, int> Bot::filesToNumber = {{'a', 0},
                                                    {'b', 1},
                                                    {'c', 2},
                                                    {'d', 3},
                                                    {'e', 4},
                                                    {'f', 5},
                                                    {'g', 6},
                                                    {'h', 7}};

std::unordered_map<char, int> Bot::numberToFiles = {{0, 'a'},
                                                    {1, 'b'},
                                                    {2, 'c'},
                                                    {3, 'd'},
                                                    {4, 'e'},
                                                    {5, 'f'},
                                                    {6, 'g'},
                                                    {7, 'h'}};

Bot::Bot() { /* Initialize custom fields here */
}

std::vector<Move *> Bot::getDropInMoves(PlaySide side) {
    uint64_t allPieces = BitBoard::allPieces;
    int pos = side == PlaySide::WHITE ? 1 : 0;

    std::vector<Move *> moves;

    for (auto it: captured[pos]) {
        int l = it == Piece::PAWN ? 8 : 0;
        int r = it == Piece::PAWN ? 56 : 64;
        for (int i = l; i < r; i++) {
            if (!(allPieces & (1ULL << i))) {
                std::string str = Utils::bitToPos(i);
                switch (it) {
                    case Piece::PAWN:
                        moves.push_back(Move::dropIn(str, Piece::PAWN));
                        break;
                    case Piece::BISHOP:
                        moves.push_back(Move::dropIn(str, Piece::BISHOP));
                        break;
                    case Piece::KNIGHT:
                        moves.push_back(Move::dropIn(str, Piece::KNIGHT));
                        break;
                    case Piece::ROOK:
                        moves.push_back(Move::dropIn(str, Piece::ROOK));
                        break;
                    case Piece::QUEEN:
                        moves.push_back(Move::dropIn(str, Piece::QUEEN));
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return moves;
}

bool Bot::movePiece(int bitsrc, int bitdst, PlaySide side) {
    Pawns *pawns = side == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *knights = side == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *rooks = side == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *bishops = side == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *queens = side == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *king = side == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;

    uint64_t enemyPieces = side == PlaySide::WHITE ? BitBoard::blackPieces : BitBoard::whitePieces;
    bool enPassant = 0;

    if (pawns->pawns & (1ULL << bitsrc)) {

        // Bug: If you replace BitBoard::blackPawns/whitePawns with enemyPawns, it doesn't work
        // Reason: No idea
        // Approved by Mihai
        if (side == PlaySide::WHITE && abs(bitdst - bitsrc) == 9 && !(enemyPieces & (1ULL << bitdst))) {
            BitBoard::clearBit(BitBoard::blackPawns->pawns, bitsrc + 1);
        }

        if (side == PlaySide::WHITE && abs(bitdst - bitsrc) == 7 && !(enemyPieces & (1ULL << bitdst))) {
            BitBoard::clearBit(BitBoard::blackPawns->pawns, bitsrc - 1);
        }


        if (side == PlaySide::BLACK && abs(bitdst - bitsrc) == 9 && !(enemyPieces & (1ULL << bitdst))) {
            BitBoard::clearBit(BitBoard::whitePawns->pawns, bitsrc - 1);
        }

        if (side == PlaySide::BLACK && abs(bitdst - bitsrc) == 7 && !(enemyPieces & (1ULL << bitdst))) {
            BitBoard::clearBit(BitBoard::whitePawns->pawns, bitsrc + 1);
        }

        BitBoard::clearBit(pawns->pawns, bitsrc);
        BitBoard::setBit(pawns->pawns, bitdst);
        if (abs(bitsrc - bitdst) == 16) {
            enPassant = 1;
        }

    } else if (knights->knights & (1ULL << bitsrc)) {
        if (pawns->promotedPawns & (1ULL << bitsrc)) {
            BitBoard::clearBit(pawns->promotedPawns, bitsrc);
            BitBoard::setBit(pawns->promotedPawns, bitdst);
        }
        BitBoard::clearBit(knights->knights, bitsrc);
        BitBoard::setBit(knights->knights, bitdst);
    } else if (rooks->rooks & (1ULL << bitsrc)) {

        if (pawns->promotedPawns & (1ULL << bitsrc)) {
            BitBoard::clearBit(pawns->promotedPawns, bitsrc);
            BitBoard::setBit(pawns->promotedPawns, bitdst);
        }
        BitBoard::clearBit(rooks->rooks, bitsrc);
        BitBoard::setBit(rooks->rooks, bitdst);
    } else if (bishops->bishops & (1ULL << bitsrc)) {
        if (pawns->promotedPawns & (1ULL << bitsrc)) {
            BitBoard::clearBit(pawns->promotedPawns, bitsrc);
            BitBoard::setBit(pawns->promotedPawns, bitdst);
        }
        BitBoard::clearBit(bishops->bishops, bitsrc);
        BitBoard::setBit(bishops->bishops, bitdst);
    } else if (queens->queens & (1ULL << bitsrc)) {
        if (pawns->promotedPawns & (1ULL << bitsrc)) {
            BitBoard::clearBit(pawns->promotedPawns, bitsrc);
            BitBoard::setBit(pawns->promotedPawns, bitdst);
        }
        BitBoard::clearBit(queens->queens, bitsrc);
        BitBoard::setBit(queens->queens, bitdst);
    } else {
        BitBoard::clearBit(king->king, bitsrc);
        BitBoard::setBit(king->king, bitdst);
        if (abs(bitsrc - bitdst) == 2) {
            if (bitdst == 2) {
                BitBoard::clearBit(rooks->rooks, 0);
                BitBoard::setBit(rooks->rooks, 3);
            } else if (bitdst == 6) {
                BitBoard::clearBit(rooks->rooks, 7);
                BitBoard::setBit(rooks->rooks, 5);
            } else if (bitdst == 58) {
                BitBoard::clearBit(rooks->rooks, 56);
                BitBoard::setBit(rooks->rooks, 59);
            } else if (bitdst == 62) {
                BitBoard::clearBit(rooks->rooks, 63);
                BitBoard::setBit(rooks->rooks, 61);
            }
        }
    }

    return enPassant;
}

void Bot::checkCapture(int bitsrc, int bitdst, PlaySide side, int testMove) {
    Pawns *pawns = side == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *knights = side == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *rooks = side == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *bishops = side == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *queens = side == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    PlaySide otherSide = side == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE;
    int pos = otherSide == PlaySide::WHITE ? 1 : 0;

    if ((pawns->pawns & (1ULL << bitdst))) {
        BitBoard::clearBit(pawns->pawns, bitdst);
        if (!testMove) {
            captured[pos].insert(Piece::PAWN);
        }
    } else if (knights->knights & (1ULL << bitdst)) {
        BitBoard::clearBit(knights->knights, bitdst);
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert(Piece::PAWN);
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            } else
                captured[pos].insert(Piece::KNIGHT);
        }
    } else if (rooks->rooks & (1ULL << bitdst)) {
        BitBoard::clearBit(rooks->rooks, bitdst);
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert(Piece::PAWN);
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            } else
                captured[pos].insert(Piece::ROOK);
        }
    } else if (bishops->bishops & (1ULL << bitdst)) {
        BitBoard::clearBit(bishops->bishops, bitdst);
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert(Piece::PAWN);
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            } else
                captured[pos].insert(Piece::BISHOP);
        }
    } else if (queens->queens & (1ULL << bitdst)) {
        BitBoard::clearBit(queens->queens, bitdst);
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert(Piece::PAWN);
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            } else
                captured[pos].insert(Piece::QUEEN);
        }
    }
}

void Bot::recordMove(Move *move, PlaySide sideToMove, int isTestMove) {
    /* You might find it useful to also separately
     * record last move in another custom field */

    std::string src = move->source.value_or("");
    std::string dst = move->destination.value();
    int bitsrc, bitdst;
    bool enPassant = 0;

    Pawns *pawns = sideToMove == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *knights = sideToMove == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *rooks = sideToMove == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *bishops = sideToMove == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *queens = sideToMove == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *king = sideToMove == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;

    uint64_t pieces = pawns->pawns | knights->knights | rooks->rooks | bishops->bishops | queens->queens | king->king;

    if (move->isNormal()) {

        bitsrc = filesToNumber[src[0]] + 8 * (src[1] - '0' - 1);
        bitdst = filesToNumber[dst[0]] + 8 * (dst[1] - '0' - 1);

        enPassant = movePiece(bitsrc, bitdst, sideToMove);
        if (enPassant) {

            if (sideToMove == PlaySide::BLACK) {

                BitBoard::setBit(BitBoard::enPassantBlack, bitsrc + 8);
            } else {
                BitBoard::setBit(BitBoard::enPassantWhite, bitsrc - 8);
            }
        } else {
            BitBoard::enPassantBlack = 0ULL;
            BitBoard::enPassantWhite = 0ULL;
        }


        checkCapture(bitsrc, bitdst, sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, isTestMove);
    } else if (move->isPromotion()) {

        BitBoard::enPassantBlack = 0ULL;
        BitBoard::enPassantWhite = 0ULL;
        bitsrc = filesToNumber[src[0]] + 8 * (src[1] - '0' - 1);
        bitdst = filesToNumber[dst[0]] + 8 * (dst[1] - '0' - 1);
        BitBoard::clearBit(pawns->pawns, bitsrc);

//        (bitsrc, bitdst, sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, isTestMove); // TODO Ce face aici?

        BitBoard::setBit(pawns->promotedPawns, bitdst);
        switch (move->getReplacement().value()) {
            case Piece::BISHOP:
                BitBoard::setBit(bishops->bishops, bitdst);
                break;
            case Piece::KNIGHT:
                BitBoard::setBit(knights->knights, bitdst);
                break;
            case Piece::ROOK:
                BitBoard::setBit(rooks->rooks, bitdst);
                break;
            case Piece::QUEEN:
                BitBoard::setBit(queens->queens, bitdst);
                break;
            default:
                break;
        }
    } else {
        BitBoard::enPassantBlack = 0ULL;
        BitBoard::enPassantWhite = 0ULL;
        bitdst = filesToNumber[dst[0]] + 8 * (dst[1] - '0' - 1);
        int pos = sideToMove == PlaySide::WHITE ? 1 : 0;
        switch (move->getReplacement().value()) {
            case Piece::PAWN: {
                BitBoard::setBit(pawns->pawns, bitdst);
                if (!isTestMove) {
                    auto it = captured[pos].find(Piece::PAWN);
                    if (it != captured[pos].end()) {
                        captured[pos].erase(it);
                    }
                }
                break;
            }
            case Piece::BISHOP: {
                BitBoard::setBit(bishops->bishops, bitdst);
                if (!isTestMove) {
                    auto it = captured[pos].find(Piece::BISHOP);
                    if (it != captured[pos].end())
                        captured[pos].erase(it);
                }
                break;
            }
            case Piece::KNIGHT: {
                BitBoard::setBit(knights->knights, bitdst);
                if (!isTestMove) {
                    auto it = captured[pos].find(Piece::KNIGHT);
                    if (it != captured[pos].end())
                        captured[pos].erase(it);
                }
                break;
            }
            case Piece::ROOK: {
                BitBoard::setBit(rooks->rooks, bitdst);
                if (!isTestMove) {
                    auto it = captured[pos].find(Piece::ROOK);
                    if (it != captured[pos].end())
                        captured[pos].erase(it);
                }
                break;
            }
            case Piece::QUEEN: {
                BitBoard::setBit(queens->queens, bitdst);
                if (!isTestMove) {
                    auto it = captured[pos].find(Piece::QUEEN);
                    if (it != captured[pos].end())
                        captured[pos].erase(it);
                }
                break;
            }
            default:
                break;
        }
    }

    BitBoard::updateWhitePieces();
    BitBoard::updateBlackPieces();
    BitBoard::updateAllPieces();

    if (isTestMove == 0) {
        rooks->updateCastlePermissions(engineSide, pieces);

        Rooks *enemyRooks = sideToMove == PlaySide::WHITE ? BitBoard::blackRooks : BitBoard::whiteRooks;
        enemyRooks->updateCastlePermissions(sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, pieces);

        King *enemyKing = sideToMove == PlaySide::WHITE ? BitBoard::blackKing : BitBoard::whiteKing;
        enemyKing->updateCastlePermissions(sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, pieces);

        king->updateCastlePermissions(engineSide, pieces);
    }

}

uint64_t getEnemyAttacks(Knights *enemyKnightsPiece,
                         Rooks *enemyRooksPiece,
                         Pawns *enemyPawnsPiece,
                         Bishops *enemyBishopsPiece,
                         Queens *enemyQueensPiece,
                         King *enemyKingPiece) {
    PlaySide enemySide = engineSide == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE;

    uint64_t knightAttacks = enemyKnightsPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces);
    uint64_t rookAttacks = enemyRooksPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces,
                                                          BitBoard::allPieces);
    uint64_t pawnAttacks = enemyPawnsPiece->getAllAttacks(enemySide);
    uint64_t bishopAttacks = enemyBishopsPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces,
                                                              BitBoard::allPieces);
    uint64_t queenAttacks = enemyQueensPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces,
                                                            BitBoard::allPieces);
    uint64_t kingAttacks = enemyKingPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces);


    return knightAttacks | rookAttacks | pawnAttacks | bishopAttacks | queenAttacks | kingAttacks;
}

Move *Bot::calculateNextMove() {

    /* Play move for the side the engine is playing (Hint: Main.getEngineSide())
     * Make sure to record your move in custom structures before returning.
     *
     * Return move that you are willing to submit
     * Move is to be constructed via one of the factory methods declared in Move.h */

    negamax(2,
            engineSide,
            INT_MIN, INT_MAX,
            0,
            get_current_state());

    recordMove(bestMove, engineSide, 0);

    return bestMove;
}

std::string Bot::getBotName() { return Bot::BOT_NAME; }

std::vector<Move *> Bot::getAvailableMoves(std::vector<Move *> allMoves,
                                           King *allyKingPiece,
                                           Knights *enemyKnightsPiece,
                                           Rooks *enemyRooksPiece,
                                           Pawns *enemyPawnsPiece,
                                           Bishops *enemyBishopsPiece,
                                           Queens *enemyQueensPiece,
                                           King *enemyKingPiece
) {
    // PlaySide engineSidePrev = engineSide;
    // Previous state
    std::unordered_map<std::string , uint64_t> state = get_current_state();

    std::multiset<Piece> captured_backup[2];
    captured_backup[0] = captured[0];
    captured_backup[1] = captured[1];

    std::vector<Move *> validMoves = {};

    for (size_t i = 0; i < allMoves.size(); ++i) {

        recordMove(allMoves[i], engineSide, 1);

        uint64_t allEnemyAttacks = getEnemyAttacks(enemyKnightsPiece, enemyRooksPiece, enemyPawnsPiece,
                                                   enemyBishopsPiece, enemyQueensPiece, enemyKingPiece);

        int kingPos = Utils::getOneBitsPositions(allyKingPiece->king)[0];

        if (!(allEnemyAttacks & (1ULL << kingPos))) {
            validMoves.push_back(allMoves[i]);
        }

        // restore move
        go_back(state, captured_backup);
    }

    return validMoves;
}

int Bot::evaluate(PlaySide side) {
    Pawns *allyPawns = side == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *allyKnights = side == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *allyRooks = side == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *allyBishops = side == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *allyQueens = side == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
//    King *allyKing = side == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;

    Pawns *enemyPawns = side == PlaySide::BLACK ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *enemyKnights = side == PlaySide::BLACK ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *enemyRooks = side == PlaySide::BLACK ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *enemyBishops = side == PlaySide::BLACK ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *enemyQueens = side == PlaySide::BLACK ? BitBoard::whiteQueens : BitBoard::blackQueens;
//    King *enemyKing = side == PlaySide::BLACK ? BitBoard::whiteKing : BitBoard::blackKing;

    int allyRooksNumber = Utils::getBits(allyRooks->rooks);
    int allyPawnsNumber = Utils::getBits(allyPawns->pawns);
    int allyKnightsNumber = Utils::getBits(allyKnights->knights);
    int allyBishopsNumber = Utils::getBits(allyBishops->bishops);
    int allyQueensNumber = Utils::getBits(allyQueens->queens);
    // int allyKing = Utils::getBits(allyKing->king);

    int enemyRooksNumber = Utils::getBits(enemyRooks->rooks);
    int enemyPawnsNumber = Utils::getBits(enemyPawns->pawns);
    int enemyKnightsNumber = Utils::getBits(enemyKnights->knights);
    int enemyBishopsNumber = Utils::getBits(enemyBishops->bishops);
    int enemyQueensNumber = Utils::getBits(enemyQueens->queens);
    // int enemyKing = Utils::getBits(enemyKing->king); 

    // daca mutam noi avem scorurile: -3 5 6 10 -7 2 6 -1
    // celalalt le ia cu minus adica o sa ia: 3 -6 7 1
    // 6 -1
    // 1
    int allyScore = allyPawnsNumber + allyKnightsNumber * 3 + allyBishopsNumber * 3 + allyRooksNumber * 5 +
                    allyQueensNumber * 9;
    int enemyScore = enemyPawnsNumber + enemyKnightsNumber * 3 + enemyBishopsNumber * 3 + enemyRooksNumber * 5 +
                     enemyQueensNumber * 9;

    return allyScore - enemyScore;
}

int Bot::negamax(int depth,
                 PlaySide currSide,
                 int alpha, int beta,
                 int test,
                 std::unordered_map<std::string, uint64_t> prev_state
) {

    std::vector<Move *> moves;
    std::vector<Move *> validMoves;

    // TODO move this block of code in a new function
    {
        Pawns *allyPawns = currSide == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
        Knights *allyKnights = currSide == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
        Rooks *allyRooks = currSide == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
        Bishops *allyBishops = currSide == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
        Queens *allyQueens = currSide == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
        King *allyKing = currSide == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;
        Pawns *enemyPawns = currSide == PlaySide::BLACK ? BitBoard::whitePawns : BitBoard::blackPawns;
        Knights *enemyKnights = currSide == PlaySide::BLACK ? BitBoard::whiteKnights : BitBoard::blackKnights;
        Rooks *enemyRooks = currSide == PlaySide::BLACK ? BitBoard::whiteRooks : BitBoard::blackRooks;
        Bishops *enemyBishops = currSide == PlaySide::BLACK ? BitBoard::whiteBishops : BitBoard::blackBishops;
        Queens *enemyQueens = currSide == PlaySide::BLACK ? BitBoard::whiteQueens : BitBoard::blackQueens;
        King *enemyKing = currSide == PlaySide::BLACK ? BitBoard::whiteKing : BitBoard::blackKing;


        uint64_t enemyAttacks = getEnemyAttacks(enemyKnights, enemyRooks, enemyPawns,
                                                enemyBishops, enemyQueens, enemyKing);

        std::vector<Move *> castleMove;
        if (allyKing->canCastle) {
            castleMove = allyKing->tryCastle(currSide, allyRooks, enemyAttacks,
                                             BitBoard::blackPieces,
                                             BitBoard::whitePieces,
                                             BitBoard::allPieces
            );
        }

        allyPawns->getMoves(currSide,
                            BitBoard::blackPieces,
                            BitBoard::whitePieces,
                            BitBoard::allPieces,
                            BitBoard::enPassantWhite,
                            BitBoard::enPassantBlack,
                            moves
        );

        allyRooks->getMoves(currSide,
                            BitBoard::blackPieces,
                            BitBoard::whitePieces,
                            BitBoard::allPieces,
                            moves
        );


        allyKnights->getMoves(currSide,
                              BitBoard::blackPieces,
                              BitBoard::whitePieces,
                              BitBoard::allPieces,
                              moves
        );

        allyKing->getMoves(currSide,
                           BitBoard::blackPieces,
                           BitBoard::whitePieces,
                           BitBoard::allPieces,
                           moves);

        allyBishops->getMoves(currSide,
                              BitBoard::blackPieces,
                              BitBoard::whitePieces,
                              BitBoard::allPieces,
                              moves
        );

        allyQueens->getMoves(currSide, allyRooks, allyBishops,
                             BitBoard::blackPieces,
                             BitBoard::whitePieces,
                             BitBoard::allPieces,
                             moves);

        std::vector<Move *> dropInMoves = getDropInMoves(currSide);

        moves.insert(moves.end(), dropInMoves.begin(), dropInMoves.end());
        validMoves = getAvailableMoves(moves,
                                        allyKing,
                                        enemyKnights,
                                        enemyRooks,
                                        enemyPawns,
                                        enemyBishops,
                                        enemyQueens,
                                        enemyKing
        );
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle(validMoves.begin(), validMoves.end(), std::default_random_engine(seed));

    // game over
    if (depth == 0) {
        if (validMoves.size() == 0) {
            return INT_MIN;
        }
        return evaluate(currSide);
    }

    int bestScore = INT_MIN;

    for (auto move: validMoves) {

        std::multiset<Piece> captured_backup[2];
        captured_backup[0] = captured[0];
        captured_backup[1] = captured[1];

        recordMove(move, currSide, 0); // rege

        int score = -negamax(depth - 1,
                             currSide == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE,
                             -beta, -alpha,
                             1,
                             get_current_state());

        go_back(prev_state, captured_backup);

        if (score > bestScore) {
            if (test == 0)
                bestMove = move;
            bestScore = score;
        }

        if (bestScore > alpha) {
            alpha = bestScore;
        }

        if (alpha >= beta)
            break;
    }

    return bestScore;
}

std::unordered_map<std::string, uint64_t> Bot::get_current_state() {
    std::unordered_map<std::string, uint64_t> state;

    state["whitePawns"] = BitBoard::whitePawns->pawns;
    state["whiteKnights"] = BitBoard::whiteKnights->knights;
    state["whiteBishops"] = BitBoard::whiteBishops->bishops;
    state["whiteRooks"] = BitBoard::whiteRooks->rooks;
    state["whiteQueens"] = BitBoard::whiteQueens->queens;
    state["whiteKing"] = BitBoard::whiteKing->king;

    state["blackPawns"] = BitBoard::blackPawns->pawns;
    state["blackKnights"] = BitBoard::blackKnights->knights;
    state["blackBishops"] = BitBoard::blackBishops->bishops;
    state["blackRooks"] = BitBoard::blackRooks->rooks;
    state["blackQueens"] = BitBoard::blackQueens->queens;
    state["blackKing"] = BitBoard::blackKing->king;

    state["whitePromotedPawns"] = BitBoard::whitePawns->promotedPawns;
    state["blackPromotedPawns"] = BitBoard::blackPawns->promotedPawns;
    state["enPassantWhite"] = BitBoard::enPassantWhite;
    state["enPassantBlack"] = BitBoard::enPassantBlack;

    return state;
}

void Bot::go_back(std::unordered_map<std::string, uint64_t> state,
                  std::multiset<Piece> captured_prev[2]) {
    BitBoard::whitePawns->pawns = state["whitePawns"];
    BitBoard::whiteKnights->knights = state["whiteKnights"];
    BitBoard::whiteBishops->bishops = state["whiteBishops"];
    BitBoard::whiteRooks->rooks = state["whiteRooks"];
    BitBoard::whiteQueens->queens = state["whiteQueens"];
    BitBoard::whiteKing->king = state["whiteKing"];

    BitBoard::blackPawns->pawns = state["blackPawns"];
    BitBoard::blackKnights->knights = state["blackKnights"];
    BitBoard::blackBishops->bishops = state["blackBishops"];
    BitBoard::blackRooks->rooks = state["blackRooks"];
    BitBoard::blackQueens->queens = state["blackQueens"];
    BitBoard::blackKing->king = state["blackKing"];

    BitBoard::whitePawns->promotedPawns = state["whitePromotedPawns"];
    BitBoard::blackPawns->promotedPawns = state["blackPromotedPawns"];
    BitBoard::enPassantWhite = state["enPassantWhite"];
    BitBoard::enPassantBlack = state["enPassantBlack"];

    captured[0] = captured_prev[0];
    captured[1] = captured_prev[1];

    BitBoard::updateWhitePieces();
    BitBoard::updateBlackPieces();
    BitBoard::updateAllPieces();
}