#include "Move.h"

#include <bits/stdc++.h>

#include "Piece.h"

Move::Move(std::optional<std::string> _source,
           std::optional<std::string> _destination,
           std::optional<Piece> _replacement)
    : source(_source), destination(_destination), replacement(_replacement) {}

std::optional<std::string> Move::getSource() { return source; }

std::optional<std::string> Move::getDestination() { return destination; }

std::optional<Piece> Move::getReplacement() { return replacement; }

bool Move::isNormal() {
  return this->source.has_value() && this->destination.has_value() &&
         !this->replacement.has_value();
}

bool Move::isPromotion() {
  return this->source.has_value() && this->destination.has_value() &&
         this->replacement.has_value();
}

bool Move::isDropIn() {
  return !this->source.has_value() && this->destination.has_value() &&
         this->replacement.has_value();
}

Move* Move::moveTo(std::optional<std::string> source,
                   std::optional<std::string> destination) {
  return new Move(source, destination, {});
}

Move* Move::promote(std::optional<std::string> source,
                    std::optional<std::string> destination,
                    std::optional<Piece> replacement) {
  return new Move(source, destination, replacement);
}

Move* Move::dropIn(std::optional<std::string> destination,
                   std::optional<Piece> replacement) {
  return new Move({}, destination, replacement);
}

Move* Move::resign() { return new Move({}, {}, {}); }
