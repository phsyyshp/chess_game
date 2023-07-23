#include "move_generation.hpp"
// #include "position.hpp"
#include <iostream>
#include <string>
#include <vector>

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
  std::cout << "factor mask test"
            << "\n";
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
  std::cout << "pushed pawn test";
  print(pushed_pawns);

  return 0;
}
