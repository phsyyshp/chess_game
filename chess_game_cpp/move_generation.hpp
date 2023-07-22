#include "position.hpp"
#include <cstdint>
#include <vector>

class MoveGeneration {
public:
  // TODO add attributes
  //   void generate_all_legal_moves(Position position, bool color);
  //   void generate_legal_moves_of_specific_piece(Position position);
  std::vector<uint64_t> factor_mask(uint64_t mask);
  uint64_t generate_single_pawn_pushes(Position position, int color);
  uint64_t generate_double_pawn_pushes(Position position, int color);
  uint64_t generate_pawn_pushes(Position position);
};
