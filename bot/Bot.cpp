#include "Bot.h"

extern PlaySide engineSide;

std::ofstream fout("log.txt", std::ios::app);

const std::string Bot::BOT_NAME = "SuperUltraMegaAlphaCrazyHouseMatrixGiobaniDestroyer"; /* Edit this, escaped characters are forbidden */
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
        int l = it.second == Piece::PAWN ? 8 : 0;
        int r = it.second == Piece::PAWN ? 56 : 64;
        for (int i = l; i < r; i++) {
            if (!(allPieces & (1ULL << i))) {
                std::string str = Utils::bitToPos(i);
                switch (it.second) {
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
            flags |= 2;
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

void Bot::checkCapture(int bitsrc, int bitdst, PlaySide side, int testMove)
{
    Pawns *pawns = side == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *knights = side == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *rooks = side == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *bishops = side == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *queens = side == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    PlaySide otherSide = side == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE;
    int pos = otherSide == PlaySide::WHITE ? 1 : 0;

    if ((pawns->pawns & (1ULL << bitdst))) {
        BitBoard::clearBit(pawns->pawns, bitdst);
        flags |= 1;
        if (!testMove) {
            captured[pos].insert({ nr, Piece::PAWN });
        }
    } else if (knights->knights & (1ULL << bitdst)) {
        BitBoard::clearBit(knights->knights, bitdst);
        flags |= 1;
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert({ nr, Piece::PAWN });
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            } else
                captured[pos].insert({ nr, Piece::KNIGHT });
        }
    } else if (rooks->rooks & (1ULL << bitdst)) {
        BitBoard::clearBit(rooks->rooks, bitdst);
        flags |= 1;
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert({ nr, Piece::PAWN });
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            } else
                captured[pos].insert({ nr, Piece::ROOK });
        }
    } else if (bishops->bishops & (1ULL << bitdst)) {
        BitBoard::clearBit(bishops->bishops, bitdst);
        flags |= 1;
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert({ nr, Piece::PAWN });
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            } else
                captured[pos].insert({ nr, Piece::BISHOP });
        }
    } else if (queens->queens & (1ULL << bitdst)) {
        BitBoard::clearBit(queens->queens, bitdst);
        flags |= 1;
        if (!testMove) {
            if (pawns->promotedPawns & (1ULL << bitdst)) {
                captured[pos].insert({ nr, Piece::PAWN });
                BitBoard::clearBit(pawns->promotedPawns, bitdst);
            } else
                captured[pos].insert({ nr, Piece::QUEEN });
        }
    }
}

void Bot::recordMove(Move *move, PlaySide sideToMove, int isTestMove)
{
    /* You might find it useful to also separately
     * record last move in another custom field */
    nr++;

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

        //    (bitsrc, bitdst, sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, isTestMove); // TODO Ce face aici?

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
                std::multiset<std::pair<int, Piece>>::iterator it;
                for (it = captured[pos].begin(); it != captured[pos].end(); ++it)
                    if (it->second == Piece::PAWN)
                        break;

                if (it != captured[pos].end()) {
                    captured[pos].erase(it);
                }
            }
            break;
        }
        case Piece::BISHOP: {
            BitBoard::setBit(bishops->bishops, bitdst);
            if (!isTestMove) {
                std::multiset<std::pair<int, Piece>>::iterator it;
                for (it = captured[pos].begin(); it != captured[pos].end(); ++it)
                    if (it->second == Piece::BISHOP)
                        break;

                if (it != captured[pos].end()) {
                    captured[pos].erase(it);
                }

            }
            break;
        }
        case Piece::KNIGHT: {
            BitBoard::setBit(knights->knights, bitdst);
            if (!isTestMove) {
                std::multiset<std::pair<int, Piece>>::iterator it;
                for (it = captured[pos].begin(); it != captured[pos].end(); ++it)
                    if (it->second == Piece::KNIGHT)
                        break;

                if (it != captured[pos].end())
                    captured[pos].erase(it);
            }
            break;
        }
        case Piece::ROOK: {
            BitBoard::setBit(rooks->rooks, bitdst);
            if (!isTestMove) {
                std::multiset<std::pair<int, Piece>>::iterator it;
                for (it = captured[pos].begin(); it != captured[pos].end(); ++it)
                    if (it->second == Piece::ROOK)
                        break;

                if (it != captured[pos].end())
                    captured[pos].erase(it);
            }
            break;
        }
        case Piece::QUEEN: {
            BitBoard::setBit(queens->queens, bitdst);
            if (!isTestMove) {
                std::multiset<std::pair<int, Piece>>::iterator it;
                for (it = captured[pos].begin(); it != captured[pos].end(); ++it)
                    if (it->second == Piece::QUEEN)
                        break;

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

void Bot::undoMove(Move *move, PlaySide sideToMove, int mor)
{
    nr--;
    std::string src = move->destination.value_or("");
    std::string dst = move->source.value_or("");
    int bitsrc, bitdst;

    Move *reverseMove = Move::moveTo(move->destination, move->source);
//    swap(move->source, move->destination);
    bitsrc = filesToNumber[src[0]] + 8 * (src[1] - '0' - 1);
    bitdst = filesToNumber[dst[0]] + 8 * (dst[1] - '0' - 1);

    Pawns *pawns = sideToMove == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *knights = sideToMove == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *rooks = sideToMove == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *bishops = sideToMove == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *queens = sideToMove == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *king = sideToMove == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;

    Pawns *enemyPawns = sideToMove == PlaySide::BLACK ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *enemyKnights = sideToMove == PlaySide::BLACK ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *enemyRooks = sideToMove == PlaySide::BLACK ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *enemyBishops = sideToMove == PlaySide::BLACK ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *enemyQueens = sideToMove == PlaySide::BLACK ? BitBoard::whiteQueens : BitBoard::blackQueens;

    if (move->isNormal()) {
        // Check if it was a castling move
        if (flags & 2) {

            BitBoard::clearBit(king->king, bitsrc);
            BitBoard::setBit(king->king, bitdst);
            if (bitsrc == 2) {
                BitBoard::clearBit(rooks->rooks, 3);
                BitBoard::setBit(rooks->rooks, 0);
            } else if (bitsrc == 6) {
                BitBoard::clearBit(rooks->rooks, 5);
                BitBoard::setBit(rooks->rooks, 7);
            } else if (bitsrc == 58) {
                BitBoard::clearBit(rooks->rooks, 59);
                BitBoard::setBit(rooks->rooks, 56);
            } else if (bitsrc == 62) {
                BitBoard::clearBit(rooks->rooks, 61);
                BitBoard::setBit(rooks->rooks, 63);
            }

            // rooks->updateCastlePermissions(engineSide, pieces);

            // Rooks *enemyRooks = sideToMove == PlaySide::WHITE ? BitBoard::blackRooks : BitBoard::whiteRooks;
            // enemyRooks->updateCastlePermissions(sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, pieces);

            // King *enemyKing = sideToMove == PlaySide::WHITE ? BitBoard::blackKing : BitBoard::whiteKing;
            // enemyKing->updateCastlePermissions(sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE, pieces);

            // king->updateCastlePermissions(engineSide, pieces);


            BitBoard::updateWhitePieces();
            BitBoard::updateBlackPieces();
            BitBoard::updateAllPieces();
            return;
        }

//        recordMove(move, sideToMove, 0);
        recordMove(reverseMove, sideToMove, 0);
        nr--;
        // Pawns *pawns = sideToMove == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
        // if ((pawns->pawns & (1ULL << bitdst))) {
        //     BitBoard::enPassantWhite = 0ULL;
        //     BitBoard::enPassantBlack = 0ULL;
        // }

        // Check for captured piec
        
        
        if (flags & 1) {
            
            int pos = sideToMove == PlaySide::WHITE ? 1 : 0;
            auto it = captured[pos].end();
            --it;

            switch (it->second) {
            case Piece::PAWN:
                BitBoard::setBit(enemyPawns->pawns, bitsrc);
                break;
            case Piece::BISHOP:
                BitBoard::setBit(enemyBishops->bishops, bitsrc);
                break;
            case Piece::KNIGHT:
                BitBoard::setBit(enemyKnights->knights, bitsrc);
                break;
            case Piece::ROOK:
                BitBoard::setBit(enemyRooks->rooks, bitsrc);
                break;
            case Piece::QUEEN:
                BitBoard::setBit(enemyQueens->queens, bitsrc);
                break;
            default:
                break;
            }

            if (it != captured[pos].end())
                captured[pos].erase(it);
        }


    } else if (move->isPromotion()) {

        BitBoard::clearBit(pawns->promotedPawns, bitsrc);
        switch (move->getReplacement().value()) {
        case Piece::BISHOP:
            BitBoard::clearBit(bishops->bishops, bitsrc);
            break;
        case Piece::KNIGHT:
            BitBoard::clearBit(knights->knights, bitsrc);
            break;
        case Piece::ROOK:
            BitBoard::clearBit(rooks->rooks, bitsrc);
            break;
        case Piece::QUEEN:
            BitBoard::clearBit(queens->queens, bitsrc);
            break;
        default:
            break;
        }
        BitBoard::setBit(pawns->pawns, bitdst);

        if ((flags & 1)) {
            int pos = sideToMove == PlaySide::WHITE ? 1 : 0;
            auto it = captured[pos].end();
            --it;

            switch (it->second) {
            case Piece::PAWN:
                BitBoard::setBit(enemyPawns->pawns, bitsrc);
                break;
            case Piece::BISHOP:
                BitBoard::setBit(enemyBishops->bishops, bitsrc);
                break;
            case Piece::KNIGHT:
                BitBoard::setBit(enemyKnights->knights, bitsrc);
                break;
            case Piece::ROOK:
                BitBoard::setBit(enemyRooks->rooks, bitsrc);
                break;
            case Piece::QUEEN:
                BitBoard::setBit(enemyQueens->queens, bitsrc);
                break;
            default:
                break;
            }

            if (it != captured[pos].end())
                captured[pos].erase(it);
        }
    } else {
        switch (move->getReplacement().value()) {
        case Piece::PAWN:
            BitBoard::clearBit(pawns->pawns, bitsrc);
            break;
        case Piece::BISHOP:
            BitBoard::clearBit(bishops->bishops, bitsrc);
            break;
        case Piece::KNIGHT:
            BitBoard::clearBit(knights->knights, bitsrc);
            break;
        case Piece::ROOK:
            BitBoard::clearBit(rooks->rooks, bitsrc);
            break;
        case Piece::QUEEN:
            BitBoard::clearBit(queens->queens, bitsrc);
            break;
        default:
            break;
        }
        int pos = sideToMove == PlaySide::WHITE ? 1 : 0;
        captured[pos].insert({ mor, move->getReplacement().value() });
    }

    BitBoard::updateWhitePieces();
    BitBoard::updateBlackPieces();
    BitBoard::updateAllPieces();
}

uint64_t getEnemyAttacks(Knights *enemyKnightsPiece,
    Rooks *enemyRooksPiece,
    Pawns *enemyPawnsPiece,
    Bishops *enemyBishopsPiece,
    Queens *enemyQueensPiece,
    King *enemyKingPiece)
{
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

Move *Bot::calculateNextMove()
{

    /* Play move for the side the engine is playing (Hint: Main.getEngineSide())
     * Make sure to record your move in custom structures before returning.
     *
     * Return move that you are willing to submit
     * Move is to be constructed via one of the factory methods declared in Move.h */
    flags = 0;

    negamax(1,
        engineSide,
        INT_MIN, INT_MAX,
        0);

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
)
{
    // PlaySide engineSidePrev = engineSide;
    // Previous state
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

    std::multiset<std::pair<int, Piece>> captured_bckp[2];
    captured_bckp[0] = captured[0];
    captured_bckp[1] = captured[1];
    uint8_t flags_bckp = flags;

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

        captured[0] = captured_bckp[0];
        captured[1] = captured_bckp[1];
        flags = flags_bckp;

        BitBoard::updateWhitePieces();
        BitBoard::updateBlackPieces();
        BitBoard::updateAllPieces();
    }

    return validMoves;
}


double Bot::evaluate2()
{

    double materialScore = 0;
    double mobilityScore = 0;
    // Get value of material

    materialScore = 200 * (Utils::getBits(BitBoard::whiteKing->king) - Utils::getBits(BitBoard::blackKing->king)) +
        9 * (Utils::getBits(BitBoard::whiteQueens->queens) - Utils::getBits(BitBoard::blackQueens->queens)) +
        5 * (Utils::getBits(BitBoard::whiteRooks->rooks) - Utils::getBits(BitBoard::blackRooks->rooks)) +
        3 * (Utils::getBits(BitBoard::whiteBishops->bishops) - Utils::getBits(BitBoard::blackBishops->bishops)) +
        3 * (Utils::getBits(BitBoard::whiteKnights->knights) - Utils::getBits(BitBoard::blackKnights->knights)) +
        1 * (Utils::getBits(BitBoard::whitePawns->pawns) - Utils::getBits(BitBoard::blackPawns->pawns));

    // std::vector<Move *> whiteMovesAll = generateMoves(BitBoard::whitePawns,
    //     BitBoard::whiteKnights,
    //     BitBoard::whiteBishops,
    //     BitBoard::whiteRooks,
    //     BitBoard::whiteQueens,
    //     BitBoard::whiteKing,
    //     PlaySide::WHITE);

    // std::vector<Move *> whiteValidMoves = getAvailableMoves(whiteMovesAll,
    //     BitBoard::whiteKing,
    //     BitBoard::blackKnights,
    //     BitBoard::blackRooks,
    //     BitBoard::blackPawns,
    //     BitBoard::blackBishops,
    //     BitBoard::blackQueens,
    //     BitBoard::blackKing);


    // std::vector<Move *> blackMovesAll = generateMoves(BitBoard::blackPawns,
    //     BitBoard::blackKnights,
    //     BitBoard::blackBishops,
    //     BitBoard::blackRooks,
    //     BitBoard::blackQueens,
    //     BitBoard::blackKing,
    //     PlaySide::BLACK);

    // std::vector<Move *> blackValidMoves = getAvailableMoves(blackMovesAll,
    //     BitBoard::blackKing,
    //     BitBoard::whiteKnights,
    //     BitBoard::whiteRooks,
    //     BitBoard::whitePawns,
    //     BitBoard::whiteBishops,
    //     BitBoard::whiteQueens,
    //     BitBoard::whiteKing);

    // mobilityScore = 0.1 * (whiteValidMoves.size() - blackValidMoves.size());

    return materialScore + 0.5 * mobilityScore;
}


double Bot::negamax(int depth,
    PlaySide currSide,
    int alpha, int beta,
    int test
)
{
    if (depth == 0) {

        return evaluate2() * (currSide == PlaySide::WHITE ? 1 : -1);
    }

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

    std::vector<Move *> moves;
    std::vector<Move *> validMoves;
    std::queue<Move *> validMoves_q;

    moves = generateMoves(allyPawns,
        allyKnights,
        allyBishops,
        allyRooks,
        allyQueens,
        allyKing,
        currSide);

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

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle(validMoves.begin(), validMoves.end(), std::default_random_engine(seed));

    for (auto move : validMoves) {
        validMoves_q.push(move);
    }

    moves.clear();
    dropInMoves.clear();
    validMoves.clear();

    // game over
    // if (depth == 0) {
    //     if (validMoves.size() == 0) {
    //         return INT_MIN;
    //     }
    //     return evaluate(currSide);
    // }

    double bestScore = INT_MIN;


    while (!validMoves_q.empty()) {

        // Utils::printBoard(BitBoard::whitePieces, "log.txt");
        Utils::printBoard(BitBoard::blackPieces, "log.txt");

        auto move = validMoves_q.front();
        validMoves_q.pop();
        int mor = -1;


        if (move->isDropIn()) {
            int pos = currSide == PlaySide::WHITE ? 1 : 0;
            std::multiset<std::pair<int, Piece>>::iterator it;
            for (it = captured[pos].begin(); it != captured[pos].end(); ++it)
                if (it->second == move->getReplacement().value()) {
                    mor = it->first;
                    break;
                }
        }

        uint8_t flags_backup = flags;
        uint64_t enPassantWhite_backup = BitBoard::enPassantWhite;
        uint64_t enPassantBlack_backup = BitBoard::enPassantBlack;

        recordMove(move, currSide, 0); // rege

        double score = -negamax(depth - 1,
            currSide == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE,
            -beta, -alpha,
            1);

        undoMove(move, currSide, mor);


        // fout << "Move " << move->source.value_or("") << " " << move->destination.value_or("") << " " << score << std::endl;

        flags = flags_backup;
        BitBoard::enPassantWhite = enPassantWhite_backup;
        BitBoard::enPassantBlack = enPassantBlack_backup;

        if (score > bestScore) {
            if (test == 0)
                bestMove = move;
//                 bestMove = new Move(*move);
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

std::vector<Move *> Bot::generateMoves(Pawns *pawns,
    Knights *knights,
    Bishops *bishops,
    Rooks *rooks,
    Queens *queens,
    King *king,
    PlaySide currSide)
{
    std::vector<Move *> moves;

    std::vector<Move *> pawnMoves = pawns->getMoves(currSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces,
        BitBoard::enPassantWhite,
        BitBoard::enPassantBlack
    );

    std::vector<Move *> knightMoves = knights->getMoves(currSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces
    );

    std::vector<Move *> bishopMoves = bishops->getMoves(currSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces
    );

    std::vector<Move *> rookMoves = rooks->getMoves(currSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces
    );

    std::vector<Move *> queenMoves = queens->getMoves(currSide,
        rooks,
        bishops,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces
    );

    std::vector<Move *> kingMoves = king->getMoves(currSide,
        BitBoard::blackPieces,
        BitBoard::whitePieces,
        BitBoard::allPieces
    );

    moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
    moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    moves.insert(moves.end(), queenMoves.begin(), queenMoves.end());
    moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());

    return moves;
}