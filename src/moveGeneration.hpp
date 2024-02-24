#pragma once

#include "move.hpp"
#include "moveList.hpp"
#include "position.hpp"
#include <bit>
class MoveGeneration {

public:
  // constractors;
  MoveGeneration() = default;
  MoveGeneration(const Position &pos) : position(pos) {}
  // move generators;
  void generateSinglePawnPushes();
  void generateDoublePawnPushes();
  void generateLeftPawnCaptures();
  void generateRightPawnCaptures();
  void generateLeftEPCaptures();
  void generateRightEPCaptures();
  void generateSinglePawnPromotions();
  void generateLeftPawnPromoCaptures();
  void generateRightPawnPromoCaptures();

  void generateKnightMoves();
  void generateBishopMoves();
  void generateRookMoves();
  void generateQueenMoves();
  void generateKingMoves();
  void generateAllMoves();
  void generateOrderedMoves();

  void generateCastling();
  // attack generators;

  uint64_t generateLeftPawnAttackMaps();
  uint64_t generateRightPawnAttackMaps();
  uint64_t generateKnightAttackMaps();
  uint64_t generateBishopAttackMaps();
  uint64_t generateRookAttackMaps();
  uint64_t generateQueenAttackMaps();
  uint64_t generateAllAttackMaps();
  // legal move generation related operations
  void generateLegalKingMoves();

  // getters
  MoveList &getMoves();
  int getNumberOfMoves() const;
  bool isPseudoLegal(const Move &move);

private:
  MoveList moveList;
  const Position &position;
};