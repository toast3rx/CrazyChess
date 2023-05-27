#ifndef BOT_H
#define BOT_H

#include "Move.h"
#include "../PlaySide.h"
#include "../bitboard/BitBoard.h"
#include <chrono>

class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */
  static std::unordered_map<char, int> filesToNumber;
  static std::unordered_map<char, int> numberToFiles;
  uint8_t flags;
  Move* bestMove;
  std::multiset<std::pair<int, Piece>> captured[2];
  int nr = 0;
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

  /**
   * Get board's state in a map containing bitbaords for every piece
   * @return  map containing bitbaords for every piece
   */
  std::unordered_map<std::string, uint64_t>  get_current_state();

  /**
   * Set the board with the state given in the map
   * @param state previous state
   * @param captured_prev  previous captured pieces
   */
  void go_back(std::unordered_map<std::string, uint64_t> state, std::multiset<std::pair<int, Piece>> captured_prev[2]);

  /**
   * @brief Generate all moves for pieces with the given side and pieces
   * 
   */
  std::vector<Move *> generateMoves(Pawns *pawns,
    Knights *knights,
    Bishops *bishops,
    Rooks *rooks,
    Queens *queens,
    King *king,
    PlaySide currSide);

  std::vector<Move *> getAvailableMoves(std::vector<Move *> allMoves,
    King *allyKingPiece,
    Knights *enemyKnightsPiece,
    Rooks *enemyRooksPiece,
    Pawns *enemyPawnsPiece,
    Bishops *enemyBishopsPiece,
    Queens *enemyQueensPiece,
    King *enemyKingPiece
  );
  bool movePiece(int src, int dst, PlaySide side);
  void checkCapture(int src, int dst, PlaySide side, int testMove);
  std::vector<Move*> getDropInMoves(PlaySide side);
  static std::string getBotName();
  double negamax(int depth, PlaySide currSide, int alpha, int beta, int test);
  int evaluate(PlaySide side);
  double evaluate2();
  void undoMove(Move *move, PlaySide sideToMove, int mor);


};
#endif
