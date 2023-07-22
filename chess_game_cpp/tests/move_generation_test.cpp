#include "move_generation.hpp"
// #include "position.hpp"
#include <iostream>
#include <string>
#include <vector>

// std::vector<uint64_t> factor_mask(uint64_t mask) {
// std::vector<uint64_t> out_vec(__builtin_popcount(mask));
// int i = 0;
// int counter = 1;
// while (counter != 0) {
//   counter = ((mask) & (mask - 1));
//   // std::cout << counter << std::endl;
//   out_vec[i] = mask ^ counter;
//   mask = counter;
//   i++;
// }
// return out_vec;
// }
void print(uint64_t mask) {
  uint64_t n = mask;
  for (int i = 63; i >= 0; i--) {
    if (i % 8 == 7) {
      std::cout << "\n";
    } else {
      std::cout << ((n >> i) & 1);
    }
  }
  std::cout << "\n";
}

int main() {
  MoveGeneration move_generator;
  uint64_t mask = 0b10101010;
  std::vector<uint64_t> factors = move_generator.factor_mask(mask);
  for (auto factor : factors) {
    std::cout << factor << " ";
  }
  std::cout << std::endl;
  Position position;
  position.set_board_to_initial_configuration();

  uint64_t pushed_pawns =
      move_generator.generate_single_pawn_pushes(position, 1);
  print(pushed_pawns);

  return 0;
}
