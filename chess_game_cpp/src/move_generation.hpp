#ifndef MOVEGENERATION_HPP
#define MOVEGENERATION_HPP

#include "Move.hpp"
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
  // getters

  std::vector<Move> getMoves() const;
  size_t getNumberOfMoves() const;

private:
  std::vector<Move> moveList;
  Position position;
};
#endif