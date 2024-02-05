#ifndef MOVEGENERATION_HPP
#define MOVEGENERATION_HPP

#include "Move.hpp"
#include "movelist.hpp"
#include "position.hpp"
class MoveGeneration {

public:
  MoveGeneration() = default;
  MoveGeneration(const Position &pos) : position(pos) {}
  void generateSinglePawnPushes();
  void generateDoublePawnPushes();
  void generateLeftPawnCaptures();
  void generateRightPawnCaptures();
  void generateKnightMoves();
  void generateBishopMoves();
  void generateRookMoves();
  void generateQueenMoves();
  void generateKingMoves();
  void generateAllMoves();

  void generateOrderedMoves();
  // getters
  MoveList &getMoves();
  int getNumberOfMoves() const;
  bool isPseudoLegal(const Move &move);

private:
  MoveList moveList;
  Position position;
};
#endif