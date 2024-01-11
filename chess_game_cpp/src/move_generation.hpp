#include "Move.hpp"
#include "position.hpp"
class MoveGeneration {
public:
  std::vector<uint64_t> factor_mask(uint64_t mask);
  uint64_t generate_single_pawn_pushes(Position position, int color);
  uint64_t generate_double_pawn_pushes(Position position, int color);
  uint64_t generate_pawn_pushes(Position position, int color);
  void generateKnightMoves(Position position, const color &coloIn);
  void generateBishopMoves(Position position, const color &colorIn){};
  void generateRookMoves(Position position, const color &colorIn){};
  void generateQueenMoves(Position position, const color &colorIn){};
};
