#include "move_generation.hpp"
#include <bitset>
#include <cstdint>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;
#include <iostream>

void print_board(uint64_t pieces) {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      cout << ((0b1ULL << (j + i * 8)) & pieces ? '1' : '0');
    }
    cout << "\n";
  }
}
uint64_t selectorf(uint64_t input, int selector) {
  if (selector == 1) {
    return input;
  } else {
    return 0b1ULL >> 1;
  }
}
uint64_t remove_bit(uint64_t input, int bit) {
  return input & ~(0b1ULL << bit);
}
uint64_t generate_bishop_mask(uint64_t position, int slope, int amount) {
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8); // - (row * 8);
  int offset = row - slope * column;
  uint64_t diag_mask = 0ULL;
  for (int i = -amount + column; i < (amount + column); i++) {
    int row_index = slope * i + offset;
    int linear_index = row_index * 8 + i;
    if ((i < 8 && i >= 0) && (row_index < 8 && row_index >= 0)) {
      diag_mask |= 0b1ULL << linear_index;
    }
  }
  return diag_mask;
}

// vector of length 64 uint64_t is output
vector<uint64_t> generate_bishop_masks() {
  vector<uint64_t> masks;

  for (int i = 0; i < 64; i++) {
    masks.push_back(generate_bishop_mask(0b1ULL << i, 1, 8) |
                    generate_bishop_mask(0b1ULL << i, -1, 8));
  }
  return masks;
}
uint64_t generate_rook_mask(uint64_t position, int amount) {
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8);
  uint64_t mask = 0ULL;
  for (int i = 1; i < 7; i++) {
    mask |= 0b1ULL << (row * 8 + i);
  }
  for (int i = 1; i < 7; i++) {
    mask |= 0b1ULL << (i * 8 + column);
  }
  return mask;
}
vector<uint64_t> generate_rook_masks() {
  vector<uint64_t> masks;
  for (int i = 0; i < 64; i++) {
    masks.push_back(generate_rook_mask(0b1ULL << i, 8));
  }
  return masks;
}
vector<uint64_t> generate_queen_masks() {
  vector<uint64_t> masks;
  for (int i = 0; i < 64; i++) {
    masks.push_back(generate_rook_mask(0b1ULL << i, 8) |
                    generate_bishop_mask(0b1ULL << i, 1, 8) |
                    generate_bishop_mask(0b1ULL << i, -1, 8));
  }
  return masks;
}

vector<uint64_t> generate_relevant_occupancy_masks(uint64_t position) {
  MoveGeneration move_generator;
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = linear_position / 8;
  uint64_t mask = 0ULL;
  uint64_t attack_mask = generate_rook_mask(position, 8);
  attack_mask = remove_bit(attack_mask, linear_position);

  vector<uint64_t> individual_squares = move_generator.factor_mask(attack_mask);

  vector<uint64_t> out;
  int n = individual_squares.size();
  for (int i = 0; i < (1 << n); i++) {
    uint64_t temp = 0;
    for (int j = 0; j < n; j++) {
      if (i & (1 << j)) {
        temp |= individual_squares[j];
      }
    }
    out.push_back(temp);
  }
  return out;
}

uint64_t generate_attack_mask(uint64_t occupancy_mask, uint64_t position) {
  MoveGeneration move_generator;
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8);
  int i = 0;
  uint64_t attack_mask = 0ULL;
  while (((occupancy_mask & (0b1uLL << (row * 8 + column + i))) == 0) &
         ((column + i) <= 7)) {
    attack_mask |= 0b1uLL << (row * 8 + column + i);
    i++;
  }

  i = 0;
  while (((occupancy_mask & (0b1uLL << (row * 8 + column - i))) == 0) &
         ((column - i) >= 0)) {
    attack_mask |= 0b1uLL << (row * 8 + column - i);
    i++;
  }
  i = 0;
  while (((occupancy_mask & (0b1uLL << (row * 8 + column + i * 8))) == 0) &
         ((row + i) <= 7)) {
    attack_mask |= 0b1uLL << (row * 8 + column + i * 8);
    i++;
  }
  i = 0;
  while (((occupancy_mask & (0b1uLL << (row * 8 + column - i * 8))) == 0) &
         ((row - i) >= 0)) {
    attack_mask |= 0b1uLL << (row * 8 + column - i * 8);
    i++;
  }
  attack_mask = remove_bit(attack_mask, linear_position);
  return attack_mask;
}
vector<uint64_t> generate_attack_masks(uint64_t position) {
  vector<uint64_t> relevant_occupancy_masks =
      generate_relevant_occupancy_masks(position);
  vector<uint64_t> attack_masks;
  for (auto mask : relevant_occupancy_masks) {
    attack_masks.push_back(generate_attack_mask(mask, position));
  }
  return attack_masks;
}
int main() {
  // uint64_t position = (1ULL << 33);
  // print_board(position);
  // uint64_t out = generate_bishop_mask(position, 1, 5);
  // out |= generate_bishop_mask(position, -1, 5);
  // std::bitset<64> binary_out(out);
  // std::cout << binary_out << "\n";
  uint64_t out = 0b1ULL << 21;
  // print_board(out);
  vector<uint64_t> out2 = generate_bishop_masks();
  vector<uint64_t> out3 = generate_rook_masks();
  vector<uint64_t> out4 = generate_queen_masks();
  vector<uint64_t> out5 = generate_relevant_occupancy_masks(0b1ULL << 21);
  vector<uint64_t> out6 = generate_attack_masks(0b1ULL << 21);
  cout << "inmasks" << endl;
  cout << out5.size() << endl;
  // cout << generate_rook_mask(0b1ULL, 8) << endl;
  print_board(generate_rook_mask(0b1ULL << 21, 21));
  print_board(out5[10]);
  cout << "attack_mask" << endl;
  print_board(generate_attack_mask(out5[10], 0b1ULL << 21));
  for (auto mask : out6) {
    print_board(mask);
    // cout << mask << endl;
    cout << "\n";
  }
  // for (auto mask : out2) {
  //   print_board(mask);
  //   cout << "\n";
  // }
  return 0;
}