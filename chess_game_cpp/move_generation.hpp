#include <cstdint>
#include <vec>

class MoveGeneration {
public:
  // TODO add attributes
  //   void generate_all_legal_moves(Position position, bool color);
  //   void generate_legal_moves_of_specific_piece(Position position);
  std::vec<uint64_t> factor_mask(uint64_t mask);
}