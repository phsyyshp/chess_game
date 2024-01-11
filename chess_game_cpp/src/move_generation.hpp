#include "Move.hpp"
#include "position.hpp"
class MoveGeneration {
public:
  std::vector<uint64_t> factor_mask(uint64_t mask);
  uint64_t generateSinglePawnPushes(Position position, const color &coloIn);
  uint64_t generateDoublePawnPushes(Position position, const color &coloIn);
  uint64_t generatePawnPushes(Position position, const color &coloIn);
  void generateKnightMoves(Position position, const color &coloIn);
  void generateBishopMoves(Position position, const color &colorIn);
  void generateRookMoves(Position position, const color &colorIn);
  void generateQueenMoves(Position position, const color &colorIn);
};
