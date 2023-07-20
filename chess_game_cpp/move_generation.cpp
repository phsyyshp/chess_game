#include "move_generation.hpp"
#include <cstdint>
#include <vec>

std::vec<uint64_t> MoveGeneration::factor_mask(uint64_t mask) {
  std::vector<uint64_t> out_vec(__builtin_popcount(mask));
  int i = 0;
  int counter = 1;
  while (counter != 0) {
    counter = ((mask) & (mask - 1));
    out_vec[i] = mask ^ counter;
    mask = counter;
    i++;
  }
  return out_vec;
}

void MoveGeneration::generate_legal_moves_of_specific_piece(Position position,
                                                            int piece_type) {}