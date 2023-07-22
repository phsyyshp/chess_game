#include <bitset>
#include <cstdint>
#include <iostream>
#include <math.h>
void print_board(uint64_t pieces) {
  uint64_t n = pieces;
  for (int i = 63; i >= 0; i--) {
    if (i % 8 == 7) {
      std::cout << "\n";
    } else {
      std::cout << ((n >> i) & 1);
    }
  }
  std::cout << "\n";
}
uint64_t generate_bishop_mask(uint64_t position, int slope, int amount) {
  int linear_position = log2(position) + 1;
  int column = linear_position % 8;
  int row = floor(linear_position / 8); // - (row * 8);
  int offset = row - slope * column;
  uint64_t diag_mask = 0ULL;
  for (int i = -amount + column; i < (amount + column); i++) {
    int row_index = slope * i + offset;
    int linear_index = row_index * 8 + i;
    if ((i < 8 && i >= 0) && (row_index < 8 && row_index >= 0)) {
      diag_mask |= 1ULL << linear_index;
    }
  }
  return diag_mask;
}

int main() {
  uint64_t position = (1ULL << 33);
  print_board(position);
  uint64_t out = generate_bishop_mask(position, 1, 5);
  out |= generate_bishop_mask(position, -1, 5);
  // std::bitset<64> binary_out(out);
  // std::cout << binary_out << "\n";
  print_board(out);
  return 0;
}