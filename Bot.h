#ifndef BOT_H
#define BOT_H

#include "Move.h"
#include "PlaySide.h"
#include "BitBoard.h"
#include <chrono>

class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */
  static std::unordered_map<char, int> filesToNumber;
  static std::unordered_map<char, int> numberToFiles;
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

  int getScore(Move *a, PlaySide currSide);
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
  int negamax(int depth, PlaySide currSide, int alpha, int beta, int test);
  int evaluate(PlaySide side);
  std::vector < Move *> generateMoves(PlaySide currSide);
};
#endif