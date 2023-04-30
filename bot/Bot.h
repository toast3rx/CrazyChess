#ifndef BOT_H
#define BOT_H

#include "../board/Move.h"
#include "PlaySide.h"
#include "../board/BitBoard.h"
#include <chrono>

class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */
  static std::unordered_map<char, int> filesToNumber;
  static std::unordered_map<char, int> numberToFiles;
  float pawnPositionWhite[64] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5,
                                0.5, -0.5, -1.0, 0.0, 0.0, -1.0, -0.5, 0.5,
                                0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0,
                                0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5,
                                1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0,
                                5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0,
                                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  float pawnPositionBlack[64] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0,
                                1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0,
                                0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5,
                                0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0,
                                0.5, -0.5, -1.0, 0.0, 0.0, -1.0, -0.5, 0.5,
                                0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5,
                                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  constexpr static const float whiteKingPositions[8][8] = {
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
    {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
    { 2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
    { 2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0}
};

constexpr static const float whiteRookPositions[8][8] = {
    { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
    { 0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5},
    {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
    {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
    {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
    {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
    {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
    { 0.0,  0.0,  0.0,  0.5,  0.5,  0.0,  0.0,  0.0}
};

constexpr static const float whiteKnightPositions[8][8] = {
    {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
    {-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0},
    {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
    {-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
    {-3.0,  0.0,  1.5,  2.0,  2.0, -3.0,  0.0, -3.0},
    {-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0},
    {-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0},
    {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}
};

constexpr static const float whiteQueenPositions[8][8] = {
    {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
    {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
    {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
    { 0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
    {-1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
    {-1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}
};

constexpr static const float whiteBishopPositions[8][8] = {
    {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
    {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
    {-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},
    {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
    {-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0},
    {-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
    {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}
};

constexpr static const float whitePawnsPositions[8][8] = {
    { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
    { 5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0},
    { 1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0},
    { 0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5},
    { 0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0},
    { 0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5},
    { 0.5,  1.0,  1.0, -2.0, -2.0,  1.0,  1.0,  0.5},
    { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}
};
  Move* bestMove;
  std::multiset<Piece> captured[2];
  /* Declare custom fields above */
  Bot();

  /**
   * Record move (either by enemy in normal mode, or by either side
   * in force mode) in custom structures
   * @param move received move
   * @param sideToMode side to move
   */
  void recordMove(Move *move, PlaySide sideToMove, int testMove);

  /**
   * Calculates next move, in response to enemyMove
   * @param enemyMove the enemy's last move
   *                  null if this is the opening move, or previous
   *                  move has been recorded in force mode
   * @return your move
   */
  Move* calculateNextMove();

  float getScore(Move *a, PlaySide currSide);
  std::vector<Move *> getDropInMoves(PlaySide side);
  std::vector<Move *> getAvailableMoves(std::vector<Move *> allMoves,
    King *allyKingPiece,
    Knights *enemyKnightsPiece,
    Rooks *enemyRooksPiece,
    Pawns *enemyPawnsPiece,
    Bishops *enemyBishopsPiece,
    Queens *enemyQueensPiece,
    King *enemyKingPiece,
    PlaySide currSide
  );
  bool movePiece(int src, int dst, PlaySide side);
  void checkCapture(int src, int dst, PlaySide side, int testMove);
  static std::string getBotName();
  float negamax(int depth, PlaySide currSide, float alpha, float beta, int test);
  float evaluate(PlaySide side);
  std::vector < Move *> generateMoves(PlaySide currSide);
};
#endif
