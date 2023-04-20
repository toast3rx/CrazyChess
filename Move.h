#ifndef MOVE_H
#define MOVE_H

#include <bits/stdc++.h>

#include "Piece.h"

class Move {
 public:
  /* Positions (source, destination) are encoded in coordinate notation
   as strings (i.e. "e1", "f6", "a4" etc.) */
  std::optional<std::string> source;
  std::optional<std::string> destination;

  /*
    Use the following 4 constructors for Move:
    moveTo(src, dst), if emitting a standard move (advance, capture, castle)
    promote(src, dst, replace), if advancing a pawn to last row
    dropIn(dst, replace), if placing a captured piece
    resign(), if you want to resign
   */

  std::optional<std::string> getSource();
  std::optional<std::string> getDestination();
  std::optional<Piece> getReplacement();
  /**
   * Checks whether the move is an usual move/capture
   * @return true if move is NOT a drop-in or promotion, false otherwise
   */
  bool isNormal();
  /**
   * Check whether move is a promotion (pawn advancing to last row)
   * @return true if move is a promotion (promotion field set and source is not
   * null)
   */
  bool isPromotion();
  /**
   * Check whether the move is a crazyhouse drop-in (place a captured enemy
   * piece to fight on your side)
   */
  bool isDropIn();
  /**
   * Emit a move from src to dst. Validity is to be checked by engine (your
   * implementation) Positions are encoded as stated at beginning of file
   * Castles are encoded as follows:
   * source: position of king
   * destination: final position of king (two tiles away)
   * @param source initial tile
   * @param destination destination tile
   * @return move to be sent to board
   */
  static Move* moveTo(std::optional<std::string> source,
                      std::optional<std::string> destination);
  /**
   * Emit a promotion move. Validity is to be checked by engine
   * (i.e. source contains a pawn in second to last row, etc.)
   * @param source initial tile of pawn
   * @param destination next tile (could be diagonal if also capturing)
   * @param replacement piece to promote to (must not be pawn or king)
   * @return move to be sent to board
   */
  static Move* promote(std::optional<std::string> source,
                       std::optional<std::string> destination,
                       std::optional<Piece> replacement);
  /**
   * Emit a drop-in (Crazyhouse specific move where player summons
   * a captured piece onto a free tile. Pawns can not be dropped in first and
   * last rows)
   * @param destination
   * @param replacement
   * @return
   */
  static Move* dropIn(std::optional<std::string> destination,
                      std::optional<Piece> replacement);
  static Move* resign();

 private:
  /* Piece to promote a pawn advancing to last row, or
   *  piece to drop-in (from captured assets) */
  std::optional<Piece> replacement;
  Move(std::optional<std::string> _source,
       std::optional<std::string> _destination,
       std::optional<Piece> _replacement);
};
#endif
