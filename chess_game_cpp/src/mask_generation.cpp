#include "move_generation.hpp"
#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;
#include <iostream>
#include <sstream>
void print_board(uint64_t pieces) {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      cout << ((0b1ULL << (j + i * 8)) & pieces ? '1' : '0');
    }
    cout << "\n";
  }
}
uint64_t remove_bit(uint64_t input, int bit) {
  return input & ~(0b1ULL << bit);
}
vector<int> position_to_row_col(uint64_t position) {

  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8);
  return {row, column};
}
uint64_t generate_bishop_mask(uint64_t position, int slope, int amount) {
  vector<int> row_column = position_to_row_col(position);
  int row = row_column[0];
  int column = row_column[1];
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
vector<uint64_t> generate_bishop_masks() {
  vector<uint64_t> masks;

  for (int i = 0; i < 64; i++) {
    masks.push_back(generate_bishop_mask(0b1ULL << i, 1, 8) |
                    generate_bishop_mask(0b1ULL << i, -1, 8));
  }
  return masks;
}
uint64_t generate_rook_mask(uint64_t position, int amount) {
  vector<int> row_column = position_to_row_col(position);
  int row = row_column[0];
  int column = row_column[1];
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

vector<uint64_t> generate_relevant_rook_occupancy_masks(uint64_t position) {
  MoveGeneration move_generator;
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8); // - (row * 8);
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

uint64_t generate_rook_attack_mask(uint64_t occupancy_mask, uint64_t position) {
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8);
  int i = 0;
  uint64_t attack_mask = 0ULL;
  while (((column + i) <= 7)) {
    if ((occupancy_mask & (0b1uLL << (row * 8 + column + i))) == 0) {
      attack_mask |= 0b1uLL << (row * 8 + column + i);
      i++;
    } else {
      attack_mask |= (0b1uLL << (row * 8 + column + i));
      break;
    }
  }
  i = 0;
  while (((column - i) >= 0)) {
    if ((occupancy_mask & (0b1uLL << (row * 8 + column - i))) == 0) {
      attack_mask |= 0b1uLL << (row * 8 + column - i);
      i++;
    } else {
      attack_mask |= (0b1uLL << (row * 8 + column - i));
      break;
    }
  }
  i = 0;
  while (((row + i) <= 7)) {
    if ((occupancy_mask & (0b1uLL << (row * 8 + column + i * 8))) == 0) {
      attack_mask |= 0b1uLL << (row * 8 + column + i * 8);
      i++;
    } else {
      attack_mask |= 0b1uLL << (row * 8 + column + i * 8);
      break;
    }
  }
  i = 0;
  while (((row - i) >= 0)) {
    if ((occupancy_mask & (0b1uLL << (row * 8 + column - i * 8))) == 0) {
      attack_mask |= 0b1uLL << (row * 8 + column - i * 8);
      i++;
    } else {
      attack_mask |= 0b1uLL << (row * 8 + column - i * 8);
      break;
    }
  }
  attack_mask = remove_bit(attack_mask, linear_position);
  return attack_mask;
}
vector<uint64_t> generate_rook_attack_masks(uint64_t position) {
  vector<uint64_t> relevant_occupancy_masks =
      generate_relevant_rook_occupancy_masks(position);
  vector<uint64_t> attack_masks;
  for (auto mask : relevant_occupancy_masks) {
    attack_masks.push_back(generate_rook_attack_mask(mask, position));
  }
  return attack_masks;
}
vector<vector<uint64_t>> generate_entire_rook_attack_masks() {
  vector<vector<uint64_t>> masks;
  for (int i = 0; i <= 63; i++) {
    masks.push_back(generate_rook_attack_masks(0b1ULL << i));
  }
  return masks;
}
void save_rook_attacks_cache() {
  vector<vector<uint64_t>> rook_attacks = generate_entire_rook_attack_masks();

  ofstream outFile("rook_attacks.txt");
  if (!outFile) {
    cerr << "Failed to open rook_attacks.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &inner_vector : rook_attacks) {
    for (const auto &bitboard : inner_vector) {
      outFile << bitboard << ' ';
      // cout << bitboard << endl;
    }
    outFile << '\n'; // new line for each inner vector
  }

  outFile.close();
}
int main() {
  vector<uint64_t> out2 = generate_bishop_masks();
  vector<uint64_t> out3 = generate_rook_masks();
  vector<uint64_t> out4 = generate_queen_masks();
  vector<uint64_t> out5 = generate_relevant_rook_occupancy_masks(0b1ULL << 21);
  vector<uint64_t> out6 = generate_rook_attack_masks(0b1ULL << 21);
  cout << "inmasks" << endl;
  cout << out5.size() << endl;
  // cout << generate_rook_mask(0b1ULL, 8) << endl;
  print_board(generate_rook_mask(0b1ULL << 21, 21));
  print_board(out5[10]);
  cout << "attack_mask" << endl;
  print_board(generate_rook_attack_mask(out5[10], 0b1ULL << 21));
  int i = 0;
  save_rook_attacks_cache();
  // for (auto mask : out6) {
  //   cout << "relevant rook occ" << endl;
  //   print_board(out5[i]);
  //   cout << "attack mask" << endl;

  //   print_board(mask);
  //   // cout << mask << endl;
  //   cout << "\n";
  //   i++;
  // }
  // for (auto mask : out2) {
  //   print_board(mask);
  //   cout << "\n";
  // }
  return 0;
}