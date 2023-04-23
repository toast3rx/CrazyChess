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

bool Bot::movePiece(int bitsrc, int bitdst, PlaySide side) {
    Pawns *pawns = side == PlaySide::WHITE ? BitBoard::whitePawns : BitBoard::blackPawns;
    Knights *knights = side == PlaySide::WHITE ? BitBoard::whiteKnights : BitBoard::blackKnights;
    Rooks *rooks = side == PlaySide::WHITE ? BitBoard::whiteRooks : BitBoard::blackRooks;
    Bishops *bishops = side == PlaySide::WHITE ? BitBoard::whiteBishops : BitBoard::blackBishops;
    Queens *queens = side == PlaySide::WHITE ? BitBoard::whiteQueens : BitBoard::blackQueens;
    King *king = side == PlaySide::WHITE ? BitBoard::whiteKing : BitBoard::blackKing;
    
    uint64_t enemyPieces =  PlaySide::WHITE ? BitBoard::blackPieces : BitBoard::whitePieces;
    Pawns *enemyPawns = PlaySide::BLACK ? BitBoard::whitePawns : BitBoard::blackPawns;
    bool enPassant = 0;

    if (pawns->pawns & (1ULL << bitsrc)) {

        std::ofstream output("toataTabla.txt", std::ios::app);
        output << "Side: " << side << std::endl;
        output << "Diferenta este: " << abs(bitdst - bitsrc) << std::endl;
        output << "Mare not: " << !(enemyPieces & (1ULL << bitdst)) << std::endl;

        if (side == PlaySide::WHITE && abs(bitdst - bitsrc) == 9 && !(enemyPieces & (1ULL << bitdst))) {
            BitBoard::clearBit(enemyPawns->pawns, bitsrc + 1);
        }

        if (side == PlaySide::WHITE && abs(bitdst - bitsrc) == 7 && !(enemyPieces & (1ULL << bitdst))) {
            BitBoard::clearBit(enemyPawns->pawns, bitsrc - 1);
        }

        if (side == PlaySide::BLACK && abs(bitdst - bitsrc) == 9 && !(enemyPieces & (1ULL << bitdst))) {
            BitBoard::clearBit(enemyPawns->pawns, bitsrc - 1);
        }

        if (side == PlaySide::BLACK && abs(bitdst - bitsrc) == 7 && !(enemyPieces & (1ULL << bitdst))) {
            output << "AM intrat in if :(" << std::endl;
            BitBoard::clearBit(enemyPawns->pawns, bitsrc + 1);
        }

        BitBoard::clearBit(pawns->pawns, bitsrc);
        BitBoard::setBit(pawns->pawns, bitdst);
        if (abs(bitsrc - bitdst) == 16) {
            enPassant = 1;
        }

    } else if (knights->knights & (1ULL << bitsrc)) {
        BitBoard::clearBit(knights->knights, bitsrc);
        BitBoard::setBit(knights->knights, bitdst);
    } else if (rooks->rooks & (1ULL << bitsrc)) {
        BitBoard::clearBit(rooks->rooks, bitsrc);
        BitBoard::setBit(rooks->rooks, bitdst);
    } else if (bishops->bishops & (1ULL << bitsrc)) {
        BitBoard::clearBit(bishops->bishops, bitsrc);
        BitBoard::setBit(bishops->bishops, bitdst);
    } else if (queens->queens & (1ULL << bitsrc)) {
        BitBoard::clearBit(queens->queens, bitsrc);
        BitBoard::setBit(queens->queens, bitdst);
    } else {
        BitBoard::clearBit(king->king, bitsrc);
        BitBoard::setBit(king->king, bitdst) ;
    }

    return enPassant;
}

void Bot::checkCapture(int bitsrc, int bitdst, PlaySide side) {
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
        captured[pos].insert(Piece::PAWN);
    } else if (knights->knights & (1ULL << bitdst)) {
        BitBoard::clearBit(knights->knights, bitdst);
        captured[pos].insert(Piece::KNIGHT);
    } else if (rooks->rooks & (1ULL << bitdst)) {
        BitBoard::clearBit(rooks->rooks, bitdst);
        captured[pos].insert(Piece::ROOK);
    } else if (bishops->bishops & (1ULL << bitdst)) {
        BitBoard::clearBit(bishops->bishops, bitdst);
        captured[pos].insert(Piece::BISHOP);
    } else if (queens->queens & (1ULL << bitdst)) {
        BitBoard::clearBit(queens->queens, bitdst);
        captured[pos].insert(Piece::QUEEN);
    } else {
        BitBoard::clearBit(king->king, bitdst);
        captured[pos].insert(Piece::QUEEN);
    }
}
 
void Bot::recordMove(Move *move, PlaySide sideToMove)
{
    /* You might find it useful to also separately
     * record last move in another custom field */
    std::string src = move->source.value();
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
        checkCapture(bitsrc, bitdst, sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE);
    } else if (move->isPromotion()) {
        BitBoard::enPassantBlack = 0ULL;
        BitBoard::enPassantWhite = 0ULL;
        bitsrc = filesToNumber[src[0]] + 8 * (src[1] - '0' - 1);
        bitdst = filesToNumber[dst[0]] + 8 * (dst[1] - '0' - 1);
        BitBoard::clearBit(pawns->pawns, bitsrc);
        checkCapture(bitsrc, bitdst, sideToMove == PlaySide::WHITE ? PlaySide::BLACK : PlaySide::WHITE);
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
        switch (move->getReplacement().value()) {
        case Piece::PAWN:
            BitBoard::setBit(pawns->pawns, bitdst);
            break;
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

    std::pair < uint64_t, uint64_t > possibleMoves[999];
    int32_t found = 0;
    /* Try to move bishops
     * I. find them.
     * II. try to make moves.*/
     /*
    for (int i = 0; i < 64; ++i) {
        if (allyBishops->bishops & (1ULL << i)) {
            uint64_t possibleMove = allyBishops->possible_moves[i];
            for (int j = 0; j < 64; ++j) {
                if ((possibleMove & (1ULL << j)) && !((1ULL << j) & allyPieces)) {
                    possibleMoves[found++] = std::make_pair(i, j);
                }
            }
        }
    }
    */
    std::vector<Move *> moves = allyPawns->getMoves(engineSide,
                                                    BitBoard::blackPieces,
                                                    BitBoard::whitePieces,
                                                    BitBoard::allPieces, 
                                                    BitBoard::enPassantWhite,
                                                    BitBoard::enPassantBlack);
                                                    
    std::vector<Move *> rookMoves = allyRooks->getMoves(engineSide,
                                                    BitBoard::blackPieces,
                                                    BitBoard::whitePieces,
                                                    BitBoard::allPieces);
                                                    
    // std::vector<Move *> knightMoves = allyKnights->getMoves(engineSide,
    //                                                 BitBoard::blackPieces,
    //                                                 BitBoard::whitePieces,
    //                                                 BitBoard::allPieces);
    std::vector<Move *> kingMoves = allyKing->getMoves(engineSide,
                                                    BitBoard::blackPieces,
                                                    BitBoard::whitePieces,
                                                    BitBoard::allPieces);
    std::vector<Move *> bishopMoves = allyBishops->getMoves(engineSide,
                                                    BitBoard::blackPieces,
                                                    BitBoard::whitePieces,
                                                    BitBoard::allPieces);

    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    // moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());
    // moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    // moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
    

    // random
    nr = rand() % moves.size();
    recordMove(moves[nr], engineSide);

    std::ofstream pawnMov("kingMoves.txt", std::ios::app);
    for (int i = 0; i < moves.size(); ++i) {
        pawnMov << moves[i]->source.value_or("") << " to " << moves[i]->destination.value_or("") << std::endl;
    }
    pawnMov << std::endl << std::endl;

    pawnMov.close();

    Utils::printBoard(BitBoard::allPieces, "toataTabla.txt");

    return moves[nr];
}

std::string Bot::getBotName() { return Bot::BOT_NAME; }
