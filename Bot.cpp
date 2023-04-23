#include "Bot.h"

extern PlaySide engineSide;

const std::string Bot::BOT_NAME = "MyBot"; /* Edit this, escaped characters are forbidden */
std::unordered_map<char, int> Bot::filesToNumber = { {'a', 0},
                                                    {'b', 1},
                                                    {'c', 2},
                                                    {'d', 3},
                                                    {'e', 4},
                                                    {'f', 5},
                                                    {'g', 6},
                                                    {'h', 7} };

std::unordered_map<char, int> Bot::numberToFiles = { {0, 'a'},
                                                    {1, 'b'},
                                                    {2, 'c'},
                                                    {3, 'd'},
                                                    {4, 'e'},
                                                    {5, 'f'},
                                                    {6, 'g'},
                                                    {7, 'h'} };
Bot::Bot()
{ /* Initialize custom fields here */
}

std::vector<Move *> Bot::getDropInMoves(PlaySide side)
{
    uint64_t allPieces = BitBoard::allPieces;
    int pos = side == PlaySide::WHITE ? 1 : 0;

    std::vector<Move *> moves;

    for (auto it : captured[pos]) {
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

bool Bot::movePiece(int bitsrc, int bitdst, PlaySide side)
{
    Pawns *pawns = side == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *knights = side == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *rooks = side == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *bishops = side == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *queens = side == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *king = side == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;

    uint64_t enemyPieces = side == PlaySide::WHITE ? BitBoard::blackPieces : BitBoard::whitePieces;
    Pawns *enemyPawns = side == PlaySide::BLACK ? BitBoard::whitePawns : BitBoard::blackPawns;
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
    }

    return enPassant;
}

void Bot::checkCapture(int bitsrc, int bitdst, PlaySide side, int testMove)
{
    Pawns *pawns = side == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *knights = side == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *rooks = side == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *bishops = side == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *queens = side == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *king = side == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;

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
            }
            else
                captured[pos].insert(Piece::KNIGHT);
        }
    } else if (rooks->rooks & (1ULL << bitdst)) {
        BitBoard::clearBit(rooks->rooks, bitdst);
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert(Piece::PAWN);
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            }
            else
                captured[pos].insert(Piece::ROOK);
        }
    } else if (bishops->bishops & (1ULL << bitdst)) {
        BitBoard::clearBit(bishops->bishops, bitdst);
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert(Piece::PAWN);
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            }
            else
                captured[pos].insert(Piece::BISHOP);
        }
    } else if (queens->queens & (1ULL << bitdst)) {
        BitBoard::clearBit(queens->queens, bitdst);
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert(Piece::PAWN);
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            }
            else
                captured[pos].insert(Piece::QUEEN);
        }
    }
}

void Bot::recordMove(Move *move, PlaySide sideToMove, int isTestMove)
{
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
        case Piece::PAWN:
        {
            BitBoard::setBit(pawns->pawns, bitdst);
            if (!isTestMove) {
                auto it = captured[pos].find(Piece::PAWN);
                if (it != captured[pos].end()) {
                    captured[pos].erase(it);
                }
            }
            break;
        }
        case Piece::BISHOP:
        {
            BitBoard::setBit(bishops->bishops, bitdst);
            if (!isTestMove) {
                auto it = captured[pos].find(Piece::BISHOP);
                if (it != captured[pos].end())
                    captured[pos].erase(it);
            }
            break;
        }
        case Piece::KNIGHT:
        {
            BitBoard::setBit(knights->knights, bitdst);
            if (!isTestMove) {
                auto it = captured[pos].find(Piece::KNIGHT);
                if (it != captured[pos].end())
                    captured[pos].erase(it);
            }
            break;
        }
        case Piece::ROOK:
        {
            BitBoard::setBit(rooks->rooks, bitdst);
            if (!isTestMove) {
                auto it = captured[pos].find(Piece::ROOK);
                if (it != captured[pos].end())
                    captured[pos].erase(it);
            }
            break;
        }
        case Piece::QUEEN:
        {
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
}
static int nr = -1;
Move *Bot::calculateNextMove()
{

    /* Play move for the side the engine is playing (Hint: Main.getEngineSide())
     * Make sure to record your move in custom structures before returning.
     *
     * Return move that you are willing to submit
     * Move is to be constructed via one of the factory methods declared in Move.h */
    nr++;
    Pawns *allyPawns = engineSide == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *allyKnights = engineSide == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *allyRooks = engineSide == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *allyBishops = engineSide == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *allyQueens = engineSide == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *allyKing = engineSide == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;
    uint64_t allyPieces = engineSide == PlaySide::WHITE ? BitBoard::whitePieces : BitBoard::blackPieces;
    Pawns *enemyPawns = engineSide == PlaySide::BLACK ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *enemyKnights = engineSide == PlaySide::BLACK ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *enemyRooks = engineSide == PlaySide::BLACK ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *enemyBishops = engineSide == PlaySide::BLACK ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *enemyQueens = engineSide == PlaySide::BLACK ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *enemyKing = engineSide == PlaySide::BLACK ? BitBoard::whiteKing : BitBoard::blackKing;
    uint64_t enemyPieces = engineSide == PlaySide::BLACK ? BitBoard::whitePieces : BitBoard::blackPieces;

    std::vector<Move *> moves;

    std::vector<Move *> pawnMoves = allyPawns->getMoves(engineSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces,
        BitBoard::enPassantWhite,
        BitBoard::enPassantBlack);

    std::vector<Move *> rookMoves = allyRooks->getMoves(engineSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces);

    std::vector<Move *> knightMoves = allyKnights->getMoves(engineSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces);
    std::vector<Move *> kingMoves = allyKing->getMoves(engineSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces);
    std::vector<Move *> bishopMoves = allyBishops->getMoves(engineSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces);
    std::vector<Move *> queenMoves = allyQueens->getMoves(engineSide, allyRooks, allyBishops,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces);
    std::vector<Move *> dropInMoves = getDropInMoves(engineSide);


    moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());


    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
    moves.insert(moves.end(), queenMoves.begin(), queenMoves.end());
    moves.insert(moves.end(), dropInMoves.begin(), dropInMoves.end());

    std::vector<Move *> validMoves = getAvailableMoves(moves,
        allyKing,
        enemyKnights,
        enemyRooks,
        enemyPawns,
        enemyBishops,
        enemyQueens,
        enemyKing
    );

    if (validMoves.size() == 0) {
        return Move::resign();
    }

    // nr = rand() % validMoves.size();
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    nr = rng () % validMoves.size();


    recordMove(validMoves[nr], engineSide, 0);

    return validMoves[nr];
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
)
{

    PlaySide engineSidePrev = engineSide;
    // Previous state
    uint64_t whitePawns = BitBoard::whitePawns->pawns;
    uint64_t whiteKnights = BitBoard::whiteKnights->knights;
    uint64_t whiteBishops = BitBoard::whiteBishops->bishops;
    uint64_t whiteRooks = BitBoard::whiteRooks->rooks;
    uint64_t whiteQueens = BitBoard::whiteQueens->queens;
    uint64_t whiteKing = BitBoard::whiteKing->king;
    uint64_t whitePieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;

    uint64_t blackPawns = BitBoard::blackPawns->pawns;
    uint64_t blackKnights = BitBoard::blackKnights->knights;
    uint64_t blackBishops = BitBoard::blackBishops->bishops;
    uint64_t blackRooks = BitBoard::blackRooks->rooks;
    uint64_t blackQueens = BitBoard::blackQueens->queens;
    uint64_t blackKing = BitBoard::blackKing->king;
    uint64_t blackPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;

    uint64_t whitePromotedPawns = BitBoard::whitePawns->promotedPawns;
    uint64_t blackPromotedPawns = BitBoard::blackPawns->promotedPawns;

    uint64_t enPassantWhite = BitBoard::enPassantWhite;
    uint64_t enPassantBlack = BitBoard::enPassantBlack;


    std::vector<Move *> validMoves = {};

    for (int i = 0; i < allMoves.size(); ++i) {

        recordMove(allMoves[i], engineSide, 1);

        PlaySide enemySide = engineSide == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE;

        uint64_t knightAttacks = enemyKnightsPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces);
        uint64_t rookAttacks = enemyRooksPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces, BitBoard::allPieces);
        uint64_t pawnAttacks = enemyPawnsPiece->getAllAttacks(enemySide);
        uint64_t bishopAttacks = enemyBishopsPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces, BitBoard::allPieces);
        uint64_t queenAttacks = enemyQueensPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces, BitBoard::allPieces);
        uint64_t kingAttacks = enemyKingPiece->getAllAttacks(enemySide, BitBoard::blackPieces, BitBoard::whitePieces);


        uint64_t allEnemyAttacks = knightAttacks | rookAttacks | pawnAttacks | bishopAttacks | queenAttacks | kingAttacks;

        int kingPos = Utils::getOneBitsPositions(allyKingPiece->king)[0];

        if (!(allEnemyAttacks & (1ULL << kingPos))) {
            validMoves.push_back(allMoves[i]);
        }

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

        BitBoard::updateWhitePieces();
        BitBoard::updateBlackPieces();
        BitBoard::updateAllPieces();
    }

    return validMoves;
}
