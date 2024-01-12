#include "Move.hpp"
#include "position.hpp"
class MoveGeneration {

public:
  MoveGeneration() = default;
  std::vector<uint64_t> factor_mask(uint64_t mask);
  void generateSinglePawnPushes(Position position, const color &coloIn);
  void generateDoublePawnPushes(Position position, const color &coloIn);
  void generateLeftPawnCaptures(Position position, const color &colorIn);
  void generateRightPawnCaptures(Position position, const color &colorIn);
  void generateKnightMoves(Position position, const color &coloIn);
  void generateBishopMoves(Position position, const color &colorIn);
  void generateRookMoves(Position position, const color &colorIn);
  void generateQueenMoves(Position position, const color &colorIn);
  void generateKingMoves(Position position, const color &colorIn);
  void generateAllMoves(Position position, const color &colorIn);
  // getters
  std::vector<Move> getMoves() const;

private:
  std::vector<Move> moveList;
};
