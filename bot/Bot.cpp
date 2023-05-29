#include "Bot.h"

#define DUDUIE 5
extern PlaySide engineSide;

[[maybe_unused]] std::ofstream fout("log.txt", std::ios::app);


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

        checkCapture(bitsrc, bitdst, sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, isTestMove);

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

static int nr = -1;

uint64_t getEnemyAttacks(Knights *enemyKnightsPiece,
                         Rooks *enemyRooksPiece,
                         Pawns *enemyPawnsPiece,
                         Bishops *enemyBishopsPiece,
                         Queens *enemyQueensPiece,
                         King *enemyKingPiece,
                         PlaySide currSide) {
    PlaySide enemySide = currSide == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE;

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

std::unordered_map<std::string, float> seen_positions;
std::time_t timer;

Move *Bot::calculateNextMove() {

    /* Play move for the side the engine is playing (Hint: Main.getEngineSide())
     * Make sure to record your move in custom structures before returning.
     *
     * Return move that you are willing to submit
     * Move is to be constructed via one of the factory methods declared in Move.h */
    // nr++;
    // std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    bestMove = Move::resign();
    int depth = 1;

    timer = std::time(nullptr);

    float score;

    while (true) {

        std::size_t act_time = std::time(nullptr);
        // fout << "Act time in calculateNextMove: " << act_time << "\n";

        if (act_time - timer > DUDUIE) {
            break;
        }

//        fout << "DEPTH IS: " << depth << std::endl;

        score = negamax(depth, engineSide, -1000000000.0, 1000000000.0, 0);

        ++depth;
    }
    // seen_positions.clear();
    // nr = rng() % validMoves.size();

    // fout << "Side to move " << (engineSide == PlaySide::WHITE ? "WHITE" : "BLACK") << std::endl;
    // fout << "BEST MOVE " << bestMove->source.value_or("NULL") << " " << bestMove->destination.value_or("NULL") << std::endl;
    // fout << "SCORE UNDER NEGAMAX " << score << std::endl;
    // fout << std::endl;

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
                                           King *enemyKingPiece,
                                           PlaySide currSide
) {
    // PlaySide engineSidePrev = engineSide;
    // Previous state
    uint64_t whitePawns = BitBoard::whitePawns->pawns;
    uint64_t whiteKnights = BitBoard::whiteKnights->knights;
    uint64_t whiteBishops = BitBoard::whiteBishops->bishops;
    uint64_t whiteRooks = BitBoard::whiteRooks->rooks;
    bool whiteRookCanCastleLeft = BitBoard::whiteRooks->canCastleLeft;
    bool whiteRookCanCastleRight = BitBoard::whiteRooks->canCastleRight;

    uint64_t whiteQueens = BitBoard::whiteQueens->queens;
    uint64_t whiteKing = BitBoard::whiteKing->king;
    bool whiteKingCanCastle = BitBoard::whiteKing->canCastle;

    uint64_t blackPawns = BitBoard::blackPawns->pawns;
    uint64_t blackKnights = BitBoard::blackKnights->knights;
    uint64_t blackBishops = BitBoard::blackBishops->bishops;
    uint64_t blackRooks = BitBoard::blackRooks->rooks;
    bool blackRookCanCastleLeft = BitBoard::blackRooks->canCastleLeft;
    bool blackRookCanCastleRight = BitBoard::blackRooks->canCastleRight;

    uint64_t blackQueens = BitBoard::blackQueens->queens;
    uint64_t blackKing = BitBoard::blackKing->king;
    bool blackKingCanCastle = BitBoard::blackKing->canCastle;

    uint64_t whitePromotedPawns = BitBoard::whitePawns->promotedPawns;
    uint64_t blackPromotedPawns = BitBoard::blackPawns->promotedPawns;

    uint64_t enPassantWhite = BitBoard::enPassantWhite;
    uint64_t enPassantBlack = BitBoard::enPassantBlack;

    std::vector<Move *> validMoves = {};

    for (auto &allMove: allMoves) {

        recordMove(allMove, currSide, 1);

        uint64_t allEnemyAttacks = getEnemyAttacks(enemyKnightsPiece, enemyRooksPiece, enemyPawnsPiece,
                                                   enemyBishopsPiece, enemyQueensPiece, enemyKingPiece, currSide);

        int kingPos = Utils::getOneBitsPositions(allyKingPiece->king)[0];

        if (!(allEnemyAttacks & (1ULL << kingPos))) {
            validMoves.push_back(allMove);
        }

        // restore move
        BitBoard::whitePawns->pawns = whitePawns;
        BitBoard::whiteKnights->knights = whiteKnights;
        BitBoard::whiteBishops->bishops = whiteBishops;
        BitBoard::whiteRooks->rooks = whiteRooks;
        BitBoard::whiteRooks->canCastleLeft = whiteRookCanCastleLeft;
        BitBoard::whiteRooks->canCastleRight = whiteRookCanCastleRight;

        BitBoard::whiteQueens->queens = whiteQueens;
        BitBoard::whiteKing->king = whiteKing;
        BitBoard::whiteKing->canCastle = whiteKingCanCastle;


        BitBoard::blackPawns->pawns = blackPawns;
        BitBoard::blackKnights->knights = blackKnights;
        BitBoard::blackBishops->bishops = blackBishops;
        BitBoard::blackRooks->rooks = blackRooks;
        BitBoard::blackRooks->canCastleLeft = blackRookCanCastleLeft;
        BitBoard::blackRooks->canCastleRight = blackRookCanCastleRight;

        BitBoard::blackQueens->queens = blackQueens;
        BitBoard::blackKing->king = blackKing;
        BitBoard::blackKing->canCastle = blackKingCanCastle;

        BitBoard::whitePawns->promotedPawns = whitePromotedPawns;
        BitBoard::blackPawns->promotedPawns = blackPromotedPawns;

        BitBoard::enPassantWhite = enPassantWhite;
        BitBoard::enPassantBlack = enPassantBlack;

        BitBoard::updateWhitePieces();
        BitBoard::updateBlackPieces();
        BitBoard::updateAllPieces();
    }

    return validMoves;
}


float Bot::getMobilityScore(PlaySide side,
                            Pawns *pawns,
                            Knights *knights,
                            Rooks *rooks,
                            Bishops *bishops,
                            Queens *queens,
                            King *king) {

    float result = 0;

    result += knights->getNumberOfMoves(side,
                                        BitBoard::blackPieces,
                                        BitBoard::whitePieces);
    result += rooks->getNumberOfMoves(side,
                                      BitBoard::blackPieces,
                                      BitBoard::whitePieces,
                                      BitBoard::allPieces);
    result += bishops->getNumberOfMoves(side,
                                        BitBoard::blackPieces,
                                        BitBoard::whitePieces,
                                        BitBoard::allPieces);
    result += queens->getNumberOfMoves(side,
                                       BitBoard::blackPieces,
                                       BitBoard::whitePieces,
                                       BitBoard::allPieces);
    result += king->getNumberOfMoves(side,
                                     BitBoard::blackPieces,
                                     BitBoard::whitePieces,
                                     BitBoard::allPieces);

    std::pair<int, int> pawnsMovesNo = pawns->getNumberOfMoves(side, BitBoard::allPieces);

    float piecesValue = Bot::KNIGHT_BASE_VALUE +
                        Bot::BISHOP_BASE_VALUE +
                        Bot::ROOK_BASE_VALUE +
                        Bot::QUEEN_BASE_VALUE;
    result += pawnsMovesNo.first + 0.5f * piecesValue * pawnsMovesNo.second * 0.1f;

    return result * 10; // score is multiplied by 10 in every evaluation
}

float Bot::evaluate(PlaySide side) {
    Pawns *allyPawns = side == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *allyKnights = side == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *allyRooks = side == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *allyBishops = side == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *allyQueens = side == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *allyKing = side == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;
    Pawns *enemyPawns = side == PlaySide::BLACK ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *enemyKnights = side == PlaySide::BLACK ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *enemyRooks = side == PlaySide::BLACK ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *enemyBishops = side == PlaySide::BLACK ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *enemyQueens = side == PlaySide::BLACK ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *enemyKing = side == PlaySide::BLACK ? BitBoard::whiteKing : BitBoard::blackKing;

    int allyRooksNumber = Utils::getBits(allyRooks->rooks);
    int allyPawnsNumber = Utils::getBits(allyPawns->pawns);
    int allyKnightsNumber = Utils::getBits(allyKnights->knights);
    int allyBishopsNumber = Utils::getBits(allyBishops->bishops);
    int allyQueensNumber = Utils::getBits(allyQueens->queens);
//     int allyKing = Utils::getBits(allyKing->king);
    std::vector<int> allyPawnsPos = Utils::getOneBitsPositions(allyPawns->pawns);
    std::vector<int> allyKnighsPos = Utils::getOneBitsPositions(allyKnights->knights);
    std::vector<int> allyBishopsPos = Utils::getOneBitsPositions(allyBishops->bishops);
    std::vector<int> allyRooksPos = Utils::getOneBitsPositions(allyRooks->rooks);
    std::vector<int> allyQueensPos = Utils::getOneBitsPositions(allyQueens->queens);
    std::vector<int> allyKingPos = Utils::getOneBitsPositions(allyKing->king);

    int enemyRooksNumber = Utils::getBits(enemyRooks->rooks);
    int enemyPawnsNumber = Utils::getBits(enemyPawns->pawns);
    int enemyKnightsNumber = Utils::getBits(enemyKnights->knights);
    int enemyBishopsNumber = Utils::getBits(enemyBishops->bishops);
    int enemyQueensNumber = Utils::getBits(enemyQueens->queens);
//     int enemyKing = Utils::getBits(enemyKing->king);
    std::vector<int> enemyPawnsPos = Utils::getOneBitsPositions(enemyPawns->pawns);
    std::vector<int> enemyKnighsPos = Utils::getOneBitsPositions(enemyKnights->knights);
    std::vector<int> enemyBishopsPos = Utils::getOneBitsPositions(enemyBishops->bishops);
    std::vector<int> enemyRooksPos = Utils::getOneBitsPositions(enemyRooks->rooks);
    std::vector<int> enemyQueensPos = Utils::getOneBitsPositions(enemyQueens->queens);
    std::vector<int> enemyKingPos = Utils::getOneBitsPositions(enemyKing->king);

    float allyScore = 0;
    float enemyScore = 0;
    float allyMaterialScore = allyPawnsNumber * Bot::PAWN_BASE_VALUE, enemyMaterialScore = enemyPawnsNumber * Bot::PAWN_BASE_VALUE;

    ////////////// Get material score for pieces ////////////
    // evaluate position
    float allyPositionScore = 0, enemyPositionScore = 0, positionScore = 0;
    int row, col;

    // ---- Scores for the engine side ----
    for (int i = 0; i < allyPawnsPos.size(); i++) {
        row = allyPawnsPos[i] / 8;
        col = allyPawnsPos[i] % 8;
        allyPositionScore = allyPositionScore + (side == PlaySide::WHITE ? whitePawnsPositions[7 - row][col] : whitePawnsPositions[row][7 - col]);
    }

    for (int i = 0; i < allyKnighsPos.size(); i++) {
        row = allyKnighsPos[i] / 8;
        col = allyKnighsPos[i] % 8;
        allyPositionScore = allyPositionScore + (side == PlaySide::WHITE ? whiteKnightPositions[7 - row][col] : whiteKnightPositions[row][7 - col]);
    }

    for (int i = 0; i < allyBishopsPos.size(); i++) {
        row = allyBishopsPos[i] / 8;
        col = allyBishopsPos[i] % 8;
        allyPositionScore = allyPositionScore + (side == PlaySide::WHITE ? whiteBishopPositions[7 - row][col] : whiteBishopPositions[row][7 - col]);
    }

    for (int i = 0; i < allyRooksPos.size(); i++) {
        row = allyRooksPos[i] / 8;
        col = allyRooksPos[i] % 8;
        allyPositionScore = allyPositionScore + (side == PlaySide::WHITE ? whiteRookPositions[7 - row][col] : whiteRookPositions[row][7 - col]);
    }

    for (int i = 0; i < allyQueensPos.size(); i++) {
        row = allyQueensPos[i] / 8;
        col = allyQueensPos[i] % 8;
        allyPositionScore = allyPositionScore + (side == PlaySide::WHITE ? whiteQueenPositions[7 - row][col] : whiteQueenPositions[row][7 - col]);
    }

    for (int i = 0; i < allyKingPos.size(); i++) {
        row = allyKingPos[i] / 8;
        col = allyKingPos[i] % 8;
        allyPositionScore = allyPositionScore + (side == PlaySide::WHITE ? whiteKingPositions[7 - row][col] : whiteKingPositions[row][7 - col]);
    }

    // ---- Scores for the enemy side ----
    for (int i = 0; i < enemyPawnsPos.size(); i++) {
        row = enemyPawnsPos[i] / 8;
        col = enemyPawnsPos[i] % 8;
        enemyPositionScore = enemyPositionScore + (side == PlaySide::WHITE ? whitePawnsPositions[7 - row][col] : whitePawnsPositions[row][7 - col]);
    }

    for (int i = 0; i < enemyKnighsPos.size(); i++) {
        row = enemyKnighsPos[i] / 8;
        col = enemyKnighsPos[i] % 8;
        enemyPositionScore = enemyPositionScore + (side == PlaySide::WHITE ? whiteKnightPositions[7 - row][col] : whiteKnightPositions[row][7 - col]);
    }

    for (int i = 0; i < enemyBishopsPos.size(); i++) {
        row = enemyBishopsPos[i] / 8;
        col = enemyBishopsPos[i] % 8;
        enemyPositionScore = enemyPositionScore + (side == PlaySide::WHITE ? whiteBishopPositions[7 - row][col] : whiteBishopPositions[row][7 - col]);
    }

    for (int i = 0; i < enemyRooksPos.size(); i++) {
        row = enemyRooksPos[i] / 8;
        col = enemyRooksPos[i] % 8;
        enemyPositionScore = enemyPositionScore + (side == PlaySide::WHITE ? whiteRookPositions[7 - row][col] : whiteRookPositions[row][7 - col]);
    }

    for (int i = 0; i < enemyQueensPos.size(); i++) {
        row = enemyQueensPos[i] / 8;
        col = enemyQueensPos[i] % 8;
        enemyPositionScore = enemyPositionScore + (side == PlaySide::WHITE ? whiteQueenPositions[7 - row][col] : whiteQueenPositions[row][7 - col]);
    }

    for (int i = 0; i < enemyKingPos.size(); i++) {
        row = enemyKingPos[i] / 8;
        col = enemyKingPos[i] % 8;
        enemyPositionScore = enemyPositionScore + (side == PlaySide::WHITE ? whiteKingPositions[7 - row][col] : whiteKingPositions[row][7 - col]);
    }

    allyScore += allyPositionScore;
    enemyScore += enemyPositionScore;

    int pos = side == PlaySide::WHITE ? 1 : 0;

    for (auto it: captured[pos]) {
        switch (it) {
        case Piece::PAWN:
            allyMaterialScore += Bot::PAWN_BASE_VALUE * 10 + 2.5;
            break;
        case Piece::BISHOP:
            allyMaterialScore += Bot::BISHOP_BASE_VALUE * 10 + 2.5;
            break;
        case Piece::KNIGHT:
            allyMaterialScore += Bot::KNIGHT_BASE_VALUE * 10 + 2.5;
            break;
        case Piece::ROOK:
            allyMaterialScore += Bot::ROOK_BASE_VALUE * 10 + 2.5;
            break;
        case Piece::QUEEN:
            allyMaterialScore += Bot::QUEEN_BASE_VALUE * 10 + 2.5;
            break;
        default:
            break;
        }
    }

    for (auto it: captured[pos ^ 1]) {
        switch (it) {
        case Piece::PAWN:
            enemyMaterialScore += Bot::PAWN_BASE_VALUE * 10 + 2.5;
            break;
        case Piece::BISHOP:
            enemyMaterialScore += Bot::BISHOP_BASE_VALUE * 10 + 2.5;
            break;
        case Piece::KNIGHT:
            enemyMaterialScore += Bot::KNIGHT_BASE_VALUE * 10 + 2.5;
            break;
        case Piece::ROOK:
            enemyMaterialScore += Bot::ROOK_BASE_VALUE * 10 + 2.5;
            break;
        case Piece::QUEEN:
            enemyMaterialScore += Bot::QUEEN_BASE_VALUE * 10 + 2.5;
            break;
        default:
            break;
        }
    }

    allyMaterialScore += allyKnightsNumber * Bot::KNIGHT_BASE_VALUE 
                    + allyBishopsNumber * Bot::BISHOP_BASE_VALUE
                    + allyRooksNumber * Bot::ROOK_BASE_VALUE +
                    allyQueensNumber * Bot::QUEEN_BASE_VALUE;
    enemyMaterialScore += enemyKnightsNumber * Bot::KNIGHT_BASE_VALUE
                    + enemyBishopsNumber * Bot::BISHOP_BASE_VALUE
                    + enemyRooksNumber * Bot::ROOK_BASE_VALUE +
                    enemyQueensNumber * Bot::QUEEN_BASE_VALUE;

    //////////// Get mobility score for pieces ////////////
    float allyMobilityScore = Bot::getMobilityScore(side,
                                                    allyPawns,
                                                    allyKnights,
                                                    allyRooks,
                                                    allyBishops,
                                                    allyQueens,
                                                    allyKing);
    float enemyMobilityScore = Bot::getMobilityScore(side,
                                                     enemyPawns,
                                                     enemyKnights,
                                                     enemyRooks,
                                                     enemyBishops,
                                                     enemyQueens,
                                                     enemyKing);

    allyScore = allyMaterialScore + 0.01f * allyMobilityScore;
    enemyScore = enemyMaterialScore + 0.01f * enemyMobilityScore;

    allyScore += allyPositionScore * 0.7f;
    enemyScore += enemyPositionScore * 0.7f;

    return allyScore - enemyScore;
}

int global_best_score = 0;

std::vector<Move *> Bot::generateMoves(PlaySide currSide) {
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

    std::vector<Move *> moves;

    uint64_t enemyAttacks = getEnemyAttacks(enemyKnights, enemyRooks, enemyPawns,
                                            enemyBishops, enemyQueens, enemyKing, currSide);

    std::vector<Move *> castleMove;
    if (allyKing->canCastle) {
        castleMove = allyKing->tryCastle(currSide, allyRooks, enemyAttacks,
                                         BitBoard::blackPieces,
                                         BitBoard::whitePieces,
                                         BitBoard::allPieces
        );

        if (!castleMove.empty()) {
            fout << "WE CAN CASTLE" << std::endl;
        }
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

    std::vector<Move *> dropInMoves = Bot::getDropInMoves(currSide);

    moves.insert(moves.end(), dropInMoves.begin(), dropInMoves.end());
    moves.insert(moves.end(), castleMove.begin(), castleMove.end());

    std::vector<Move *> validMoves = Bot::getAvailableMoves(moves,
                                                            allyKing,
                                                            enemyKnights,
                                                            enemyRooks,
                                                            enemyPawns,
                                                            enemyBishops,
                                                            enemyQueens,
                                                            enemyKing,
                                                            currSide
    );

//    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    return validMoves;
}

float Bot::getScore(Move *a, PlaySide currSide) {
    // sort(validMoves.begin(), validMoves.end(), sortCompare);
    float score = -1000000000.0;

    uint64_t whitePawns = BitBoard::whitePawns->pawns;
    uint64_t whiteKnights = BitBoard::whiteKnights->knights;

    uint64_t whiteBishops = BitBoard::whiteBishops->bishops;

    uint64_t whiteRooks = BitBoard::whiteRooks->rooks;
    bool whiteRooksCanCastleLeft = BitBoard::whiteRooks->canCastleLeft;
    bool whiteRooksCanCastleRight = BitBoard::whiteRooks->canCastleRight;

    uint64_t whiteQueens = BitBoard::whiteQueens->queens;

    uint64_t whiteKing = BitBoard::whiteKing->king;
    bool whiteKingCanCastle = BitBoard::whiteKing->canCastle;

    uint64_t blackPawns = BitBoard::blackPawns->pawns;
    uint64_t blackKnights = BitBoard::blackKnights->knights;
    uint64_t blackBishops = BitBoard::blackBishops->bishops;
    uint64_t blackRooks = BitBoard::blackRooks->rooks;
    bool blackRooksCanCastleLeft = BitBoard::blackRooks->canCastleLeft;
    bool blackRooksCanCastleRight = BitBoard::blackRooks->canCastleRight;

    uint64_t blackQueens = BitBoard::blackQueens->queens;
    uint64_t blackKing = BitBoard::blackKing->king;
    bool blackKingCanCastle = BitBoard::blackKing->canCastle;

    uint64_t whitePromotedPawns = BitBoard::whitePawns->promotedPawns;
    uint64_t blackPromotedPawns = BitBoard::blackPawns->promotedPawns;

    uint64_t enPassantWhite = BitBoard::enPassantWhite;
    uint64_t enPassantBlack = BitBoard::enPassantBlack;

    std::multiset<Piece> captured_backup[2];
    captured_backup[0] = captured[0];
    captured_backup[1] = captured[1];

    recordMove(a, currSide, 0);

    std::string string_pos = "";

    string_pos += std::to_string(BitBoard::whitePawns->pawns);
    string_pos += std::to_string(BitBoard::whiteKnights->knights);
    string_pos += std::to_string(BitBoard::whiteBishops->bishops);
    string_pos += std::to_string(BitBoard::whiteRooks->rooks);
    string_pos += std::to_string(BitBoard::whiteQueens->queens);
    string_pos += std::to_string(BitBoard::whiteKing->king);

    string_pos += std::to_string(BitBoard::blackPawns->pawns);
    string_pos += std::to_string(BitBoard::blackKnights->knights);
    string_pos += std::to_string(BitBoard::blackBishops->bishops);
    string_pos += std::to_string(BitBoard::blackRooks->rooks);
    string_pos += std::to_string(BitBoard::blackQueens->queens);
    string_pos += std::to_string(BitBoard::blackKing->king);


    if (seen_positions.find(string_pos) != seen_positions.end()) {
        // fout << "Found position in seen positions" << std::endl;
        score = seen_positions[string_pos];
    }

    BitBoard::whitePawns->pawns = whitePawns;
    BitBoard::whiteKnights->knights = whiteKnights;
    BitBoard::whiteBishops->bishops = whiteBishops;

    BitBoard::whiteRooks->rooks = whiteRooks;
    BitBoard::whiteRooks->canCastleLeft = whiteRooksCanCastleLeft;
    BitBoard::whiteRooks->canCastleRight = whiteRooksCanCastleRight;

    BitBoard::whiteQueens->queens = whiteQueens;

    BitBoard::whiteKing->king = whiteKing;
    BitBoard::whiteKing->canCastle = whiteKingCanCastle;

    BitBoard::blackPawns->pawns = blackPawns;
    BitBoard::blackKnights->knights = blackKnights;
    BitBoard::blackBishops->bishops = blackBishops;

    BitBoard::blackRooks->rooks = blackRooks;
    BitBoard::blackRooks->canCastleLeft = blackRooksCanCastleLeft;
    BitBoard::blackRooks->canCastleRight = blackRooksCanCastleRight;

    BitBoard::blackQueens->queens = blackQueens;
    BitBoard::blackKing->king = blackKing;
    BitBoard::blackKing->canCastle = blackKingCanCastle;

    BitBoard::whitePawns->promotedPawns = whitePromotedPawns;
    BitBoard::blackPawns->promotedPawns = blackPromotedPawns;

    BitBoard::enPassantWhite = enPassantWhite;
    BitBoard::enPassantBlack = enPassantBlack;

    captured[0] = captured_backup[0];
    captured[1] = captured_backup[1];

    BitBoard::updateWhitePieces();
    BitBoard::updateBlackPieces();
    BitBoard::updateAllPieces();

    return score;
}

std::shared_ptr<std::vector<Move *>> sharedptr;

float Bot::negamax(int depth, PlaySide currSide, float alpha, float beta, int test) {
    std::vector<Move *> validMoves = Bot::generateMoves(currSide);
    //sharedptr = std::make_shared<std::vector<Move *>>(validMoves);
    // game over
    if (depth == 0) {
        if (validMoves.empty()) {
            return -1e9;
        }

        for (int i = 0; i < validMoves.size(); i++)
            delete validMoves[i];
        return evaluate(PlaySide::WHITE) * (currSide == PlaySide::WHITE ? 1 : -1);
    }


    if (test == 0 && validMoves.size() == 1) {
        delete bestMove;
        bestMove = Move::promote(validMoves[0]->source, validMoves[0]->destination, validMoves[0]->replacement);
        return 0;
    }

    float bestScore = -1000000000.7;
    sort(validMoves.begin(), validMoves.end(), [&](Move *&a, Move *&b) {
        return Bot::getScore(a, currSide) > Bot::getScore(b, currSide);
    });

    // for (auto move : validMoves) {
    //     fout << "Move " << move->source.value_or(" ") << " " << move->destination.value_or(" ") << " SCORE " << Bot::getScore(move, currSide) << std::endl;
    // }

    /////sortam aici ?!
    for (auto move: validMoves) {
        // fout << "Move " << move->source.value_or(" ") << " " << move->destination.value_or(" ") << " SCORE " << Bot::getScore(move, currSide) << std::endl;
        std::size_t act_time = std::time(nullptr);

        // fout << "Act time in negamax " << act_time << "\n";

        // if (act_time - timer > DUDUIE) {
        //     break;
        // }
        // fout << "Act time in calculateNextMove: " << act_time << "\n";

        if (act_time - timer > DUDUIE) {
            break;
        }
        uint64_t whitePawns = BitBoard::whitePawns->pawns;
        uint64_t whiteKnights = BitBoard::whiteKnights->knights;
        uint64_t whiteBishops = BitBoard::whiteBishops->bishops;
        uint64_t whiteRooks = BitBoard::whiteRooks->rooks;
        uint64_t whiteQueens = BitBoard::whiteQueens->queens;
        uint64_t whiteKing = BitBoard::whiteKing->king;

        uint64_t blackPawns = BitBoard::blackPawns->pawns;
        uint64_t blackKnights = BitBoard::blackKnights->knights;
        uint64_t blackBishops = BitBoard::blackBishops->bishops;
        uint64_t blackRooks = BitBoard::blackRooks->rooks;
        uint64_t blackQueens = BitBoard::blackQueens->queens;
        uint64_t blackKing = BitBoard::blackKing->king;

        uint64_t whitePromotedPawns = BitBoard::whitePawns->promotedPawns;
        uint64_t blackPromotedPawns = BitBoard::blackPawns->promotedPawns;

        uint64_t enPassantWhite = BitBoard::enPassantWhite;
        uint64_t enPassantBlack = BitBoard::enPassantBlack;

        std::multiset<Piece> captured_backup[2];
        captured_backup[0] = captured[0];
        captured_backup[1] = captured[1];

        recordMove(move, currSide, 0); // rege


        float score = -negamax(depth - 1, currSide == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, -beta,
                               -alpha, 1);

        std::string string_pos = "";

        string_pos += std::to_string(BitBoard::whitePawns->pawns);
        string_pos += std::to_string(BitBoard::whiteKnights->knights);
        string_pos += std::to_string(BitBoard::whiteBishops->bishops);
        string_pos += std::to_string(BitBoard::whiteRooks->rooks);
        string_pos += std::to_string(BitBoard::whiteQueens->queens);
        string_pos += std::to_string(BitBoard::whiteKing->king);

        string_pos += std::to_string(BitBoard::blackPawns->pawns);
        string_pos += std::to_string(BitBoard::blackKnights->knights);
        string_pos += std::to_string(BitBoard::blackBishops->bishops);
        string_pos += std::to_string(BitBoard::blackRooks->rooks);
        string_pos += std::to_string(BitBoard::blackQueens->queens);
        string_pos += std::to_string(BitBoard::blackKing->king);

        act_time = std::time(nullptr);
        // fout << "Act time in calculateNextMove: " << act_time << "\n";

        if (act_time - timer < DUDUIE) {
            if (seen_positions.find(string_pos) != seen_positions.end()) {
                seen_positions[string_pos] = std::max(seen_positions[string_pos], score);
            } else {
                seen_positions[string_pos] = score;
            }
        }
        // fout << "Score: " << score << "Move " << move->source.value_or("-1") << " " << move->destination.value_or("-1") << std::endl;

        // restore move
        BitBoard::whitePawns->pawns = whitePawns;
        BitBoard::whiteKnights->knights = whiteKnights;
        BitBoard::whiteBishops->bishops = whiteBishops;
        BitBoard::whiteRooks->rooks = whiteRooks;
        BitBoard::whiteQueens->queens = whiteQueens;
        BitBoard::whiteKing->king = whiteKing;

        BitBoard::blackPawns->pawns = blackPawns;
        BitBoard::blackKnights->knights = blackKnights;
        BitBoard::blackBishops->bishops = blackBishops;
        BitBoard::blackRooks->rooks = blackRooks;
        BitBoard::blackQueens->queens = blackQueens;
        BitBoard::blackKing->king = blackKing;

        BitBoard::whitePawns->promotedPawns = whitePromotedPawns;
        BitBoard::blackPawns->promotedPawns = blackPromotedPawns;

        BitBoard::enPassantWhite = enPassantWhite;
        BitBoard::enPassantBlack = enPassantBlack;

        captured[0] = captured_backup[0];
        captured[1] = captured_backup[1];

        BitBoard::updateWhitePieces();
        BitBoard::updateBlackPieces();
        BitBoard::updateAllPieces();
        // if (act_time - timer > DUDUIE) {
        //     break;
        // }

        if (score > bestScore) {
            act_time = std::time(nullptr);
            if (test == 0 && act_time - timer < DUDUIE) {
                delete bestMove;
                bestMove = Move::promote(move->source, move->destination, move->replacement);
            }
            bestScore = score;
        }

        if (bestScore > alpha) {
            alpha = bestScore;
        }

        if (alpha >= beta) {
            // fout << "ne am oprit" << std::endl;
            break;
        }

    }

    for (int i = 0; i < validMoves.size(); i++)
        delete validMoves[i];
    // sharedptr.reset();

    return bestScore;
}