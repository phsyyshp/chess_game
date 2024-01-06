#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
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
void print_board_os(std::ofstream &os, uint64_t pieces) {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      os << ((0b1ULL << (j + i * 8)) & pieces ? '1' : '0');
    }
    os << "\n";
  }
  os << "\n";
}
std::vector<uint64_t> factor_mask(uint64_t mask) {
  std::vector<uint64_t> out_vec(__builtin_popcountll(mask));
  int i = 0;
  uint64_t counter = 1;
  while (counter != 0) {
    counter = ((mask) & (mask - 1));
    out_vec[i] = mask ^ counter;
    mask = counter;
    i++;
  }
  return out_vec;
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
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8); // - (row * 8);
  uint64_t mask = 0ULL;
  uint64_t attack_mask = generate_rook_mask(position, 8);
  attack_mask = remove_bit(attack_mask, linear_position);

  vector<uint64_t> individual_squares = factor_mask(attack_mask);

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
vector<uint64_t> generate_relevant_bishop_occupancy_masks(uint64_t position) {
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8); // - (row * 8);
  uint64_t mask = 0ULL;
  uint64_t attack_mask = generate_bishop_mask(position, 1, 8) |
                         generate_bishop_mask(position, -1, 8);
  attack_mask = remove_bit(attack_mask, linear_position);

  vector<uint64_t> individual_squares = factor_mask(attack_mask);

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

uint64_t generate_bishop_attack_mask(uint64_t occupancy_mask,
                                     uint64_t position) {
  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = floor(linear_position / 8);
  int i, j;
  uint64_t attack_mask = 0ULL;

  // Up-Right Diagonal
  i = row + 1;
  j = column + 1;
  while (i <= 7 && j <= 7) {
    attack_mask |= 0b1uLL << (i * 8 + j);
    if (occupancy_mask & (0b1uLL << (i * 8 + j))) {
      break;
    }
    i++;
    j++;
  }

  // Up-Left Diagonal
  i = row + 1;
  j = column - 1;
  while (i <= 7 && j >= 0) {
    attack_mask |= 0b1uLL << (i * 8 + j);
    if (occupancy_mask & (0b1uLL << (i * 8 + j))) {
      break;
    }
    i++;
    j--;
  }

  // Down-Right Diagonal
  i = row - 1;
  j = column + 1;
  while (i >= 0 && j <= 7) {
    attack_mask |= 0b1uLL << (i * 8 + j);
    if (occupancy_mask & (0b1uLL << (i * 8 + j))) {
      break;
    }
    i--;
    j++;
  }

  // Down-Left Diagonal
  i = row - 1;
  j = column - 1;
  while (i >= 0 && j >= 0) {
    attack_mask |= 0b1uLL << (i * 8 + j);
    if (occupancy_mask & (0b1uLL << (i * 8 + j))) {
      break;
    }
    i--;
    j--;
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
vector<uint64_t> generate_bishop_attack_masks(uint64_t position) {
  vector<uint64_t> relevant_occupancy_masks =
      generate_relevant_bishop_occupancy_masks(position);
  vector<uint64_t> attack_masks;
  for (auto mask : relevant_occupancy_masks) {
    attack_masks.push_back(generate_bishop_attack_mask(mask, position));
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
vector<vector<uint64_t>> generate_entire_bishop_attack_masks() {
  vector<vector<uint64_t>> masks;
  for (int i = 0; i <= 63; i++) {
    masks.push_back(generate_bishop_attack_masks(0b1ULL << i));
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
      // outFile << bitboard << ' ';
      outFile << std::bitset<64>(bitboard).to_string() << ' ';
      // cout << bitboard << endl;
    }
    outFile << '\n'; // new line for each inner vector
  }

  outFile.close();
}
void save_bishop_attacks_cache() {
  vector<vector<uint64_t>> bishop_attacks =
      generate_entire_bishop_attack_masks();

  ofstream outFile("bishop_attacks.txt");
  if (!outFile) {
    cerr << "Failed to open rook_attacks.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &inner_vector : bishop_attacks) {
    for (const auto &bitboard : inner_vector) {
      // outFile << bitboard << ' ';
      // outFile << std::bitset<64>(bitboard).to_string() << ' ';
      print_board_os(outFile, bitboard);
      // cout << bitboard << endl;
    }
    outFile << '\n'; // new line for each inner vector
  }

  outFile.close();
}

uint64_t generate_magic_index(uint64_t bitboard, uint64_t magic_number,
                              int shiftBits) {
  return (bitboard * magic_number) >> shiftBits;
}

uint64_t generate_magic_number(uint64_t position) {
  vector<uint64_t> relevant_rook_occupancy_masks =
      generate_relevant_rook_occupancy_masks(position);
  vector<uint64_t> rook_attack_masks = generate_rook_attack_masks(position);
  int bits = log2(relevant_rook_occupancy_masks.size());
  int shiftBits = 64 - bits;
  vector<uint64_t> lookup_table(1ull << bits,
                                0); // Size of lookup table should be 2^bits
  uint64_t magic_number;

  while (true) {
    random_device rd;
    mt19937_64 rng(rd());
    uniform_int_distribution<uint64_t> distribution;
    magic_number =
        distribution(rng) & distribution(rng) &
        distribution(
            rng); // This generates a number with a few random bits set.

    bool failed = false;
    lookup_table.assign(lookup_table.size(), 0); // Reset the lookup table

    for (size_t i = 0; i < relevant_rook_occupancy_masks.size(); i++) {
      uint64_t index = generate_magic_index(relevant_rook_occupancy_masks[i],
                                            magic_number, shiftBits);

      if (lookup_table[index] == 0) {
        lookup_table[index] = rook_attack_masks[i];
        // cout << "looking" << endl;
      } else if (lookup_table[index] != rook_attack_masks[i]) {
        failed = true; // collision
        // cout << "failed" << endl;
        break;
      }
    }

    if (!failed) {
      break;
    }
  }

  return magic_number;
}
uint64_t generate_bishop_magic_number(uint64_t position) {
  vector<uint64_t> relevant_bishop_occupancy_masks =
      generate_relevant_bishop_occupancy_masks(position);
  vector<uint64_t> bishop_attack_masks = generate_bishop_attack_masks(position);
  int bits = log2(relevant_bishop_occupancy_masks.size());
  int shiftBits = 64 - bits;
  vector<uint64_t> lookup_table(1ull << bits,
                                0); // Size of lookup table should be 2^bits
  uint64_t magic_number;

  while (true) {
    random_device rd;
    mt19937_64 rng(rd());
    uniform_int_distribution<uint64_t> distribution;
    magic_number =
        distribution(rng) & distribution(rng) &
        distribution(
            rng); // This generates a number with a few random bits set.

    bool failed = false;
    lookup_table.assign(lookup_table.size(), 0); // Reset the lookup table

    for (size_t i = 0; i < relevant_bishop_occupancy_masks.size(); i++) {
      uint64_t index = generate_magic_index(relevant_bishop_occupancy_masks[i],
                                            magic_number, shiftBits);

      if (lookup_table[index] == 0) {
        lookup_table[index] = bishop_attack_masks[i];
        // cout << "looking" << endl;
      } else if (lookup_table[index] != bishop_attack_masks[i]) {
        failed = true; // collision
        // cout << "failed" << endl;
        break;
      }
    }

    if (!failed) {
      break;
    }
  }

  return magic_number;
}

vector<uint64_t> generate_rook_lookup_table(uint64_t magic_number,
                                            uint64_t position) {

  vector<uint64_t> relevant_rook_occupancy_masks =
      generate_relevant_rook_occupancy_masks(position);
  vector<uint64_t> rook_attack_masks = generate_rook_attack_masks(position);
  int bits = log2(relevant_rook_occupancy_masks.size());
  int shiftBits = 64 - bits;
  vector<uint64_t> lookup_table(1ull << bits,
                                0); // Size of lookup table should be 2^bits

  lookup_table.assign(lookup_table.size(), 0); // Reset the lookup table

  for (size_t i = 0; i < relevant_rook_occupancy_masks.size(); i++) {
    uint64_t index = generate_magic_index(relevant_rook_occupancy_masks[i],
                                          magic_number, shiftBits);

    lookup_table[index] = rook_attack_masks[i];
  }
  return lookup_table;
}
vector<uint64_t> generate_bishop_lookup_table(uint64_t magic_number,
                                              uint64_t position) {

  vector<uint64_t> relevant_bishop_occupancy_masks =
      generate_relevant_bishop_occupancy_masks(position);
  vector<uint64_t> bishop_attack_masks = generate_bishop_attack_masks(position);
  int bits = log2(relevant_bishop_occupancy_masks.size());
  int shiftBits = 64 - bits;
  vector<uint64_t> lookup_table(1ull << bits,
                                0); // Size of lookup table should be 2^bits

  lookup_table.assign(lookup_table.size(), 0); // Reset the lookup table

  for (size_t i = 0; i < relevant_bishop_occupancy_masks.size(); i++) {
    uint64_t index = generate_magic_index(relevant_bishop_occupancy_masks[i],
                                          magic_number, shiftBits);

    lookup_table[index] = bishop_attack_masks[i];
  }
  return lookup_table;
}

vector<uint64_t> generate_all_magic_numbers() {

  vector<uint64_t> out(64, 0);
  uint64_t position;
  for (int i = 0; i < 64; i++) {
    position = 0b1ULL << i;
    out[i] = generate_magic_number(position);
  }
  return out;
}
vector<uint64_t> generate_all_bishop_magic_numbers() {

  vector<uint64_t> out(64, 0);
  uint64_t position;
  for (int i = 0; i < 64; i++) {
    position = 0b1ULL << i;
    out[i] = generate_bishop_magic_number(position);
  }
  return out;
}

vector<vector<uint64_t>> generate_all_rook_lookup_tables() {
  // open file rook_magic_numbers.txt and save each line to a vector
  // for each line in the vector, generate the lookup table and save it to a
  // vector return the vector
  vector<vector<uint64_t>> out;
  ifstream inFile("rook_magic_numbers.txt");
  if (!inFile) {
    cerr << "Failed to open rook_magic_numbers.txt for reading\n";
    throw runtime_error("Failed to open file");
  }
  string line;
  while (getline(inFile, line)) {
    uint64_t magic_number = stoull(line);
    vector<uint64_t> lookup_table =
        generate_rook_lookup_table(magic_number, 0b1ULL << out.size());
    out.push_back(lookup_table);
  }
  return out;
}
vector<vector<uint64_t>> generate_all_bishop_lookup_tables() {
  // open file rook_magic_numbers.txt and save each line to a vector
  // for each line in the vector, generate the lookup table and save it to a
  // vector return the vector
  vector<vector<uint64_t>> out;
  ifstream inFile("bishop_magic_numbers.txt");
  if (!inFile) {
    cerr << "Failed to open bishop_magic_numbers.txt for reading\n";
    throw runtime_error("Failed to open file");
  }
  string line;
  while (getline(inFile, line)) {
    uint64_t magic_number = stoull(line);
    vector<uint64_t> lookup_table =
        generate_bishop_lookup_table(magic_number, 0b1ULL << out.size());
    out.push_back(lookup_table);
  }
  return out;
}

void save_rook_lookup_tables() {
  vector<vector<uint64_t>> out = generate_all_rook_lookup_tables();
  ofstream outFile("rook_lookup_tables.txt");
  if (!outFile) {
    cerr << "Failed to open rook_lookup_tables.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &inner_vector : out) {
    for (const auto &bitboard : inner_vector) {
      outFile << bitboard << ' ';
    }
    outFile << '\n'; // new line for each inner vector
  }

  outFile.close();
}
void save_bishop_lookup_tables() {
  vector<vector<uint64_t>> out = generate_all_bishop_lookup_tables();
  ofstream outFile("bishop_lookup_tables.txt");
  if (!outFile) {
    cerr << "Failed to open bishop_lookup_tables.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &inner_vector : out) {
    for (const auto &bitboard : inner_vector) {
      outFile << bitboard << ' ';
    }
    outFile << '\n'; // new line for each inner vector
  }

  outFile.close();
}

void save_magic_numbers() {
  vector<uint64_t> out = generate_all_magic_numbers();
  ofstream outFile("rook_magic_numbers.txt");
  if (!outFile) {
    cerr << "Failed to open rook_magic_numbers.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &bitboard : out) {
    outFile << bitboard << '\n';
  }
  cout << "it works" << endl;

  outFile.close();
}
void save_bishop_magic_numbers() {
  vector<uint64_t> out = generate_all_bishop_magic_numbers();
  ofstream outFile("bishop_magic_numbers.txt");
  if (!outFile) {
    cerr << "Failed to open bishop_magic_numbers.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &bitboard : out) {
    outFile << bitboard << '\n';
  }
  cout << "it works" << endl;

  outFile.close();
}
uint64_t generate_white_pawn_attacks(int position) {
  uint64_t position_mask = 0b1uLL << position;
  uint64_t not_A_file = 0b1uLL;
  uint64_t not_H_file = 0b1uLL << 7;
  for (int i = 0; i < 8; i++) {
    not_A_file |= 0b1uLL << (i * 8);
  }
  not_A_file = ~(not_A_file);
  uint64_t h1 = 0b1uLL << 7;
  for (int i = 0; i < 8; i++) {
    not_H_file |= h1 << (i * 8);
  }
  not_H_file = ~(not_H_file);

  return ((position_mask << 9) & (not_A_file)) |
         ((position_mask << 7) & (not_H_file));
}
uint64_t generate_black_pawn_attacks(int position) {
  uint64_t position_mask = 0b1uLL << position;
  uint64_t not_A_file = 0b1uLL;
  uint64_t not_H_file = 0b1uLL << 7;
  for (int i = 0; i < 8; i++) {
    not_A_file |= 0b1uLL << (i * 8);
  }
  not_A_file = ~(not_A_file);
  uint64_t h1 = 0b1uLL << 7;
  for (int i = 0; i < 8; i++) {
    not_H_file |= h1 << (i * 8);
  }
  not_H_file = ~(not_H_file);

  return ((position_mask >> 9) & (not_H_file)) |
         ((position_mask >> 7) & (not_A_file));
}
std::vector<uint64_t> generate_all_white_pawn_attacks() {
  std::vector<uint64_t> out;
  for (int i = 0; i <= 63; i++) {
    out.push_back(generate_white_pawn_attacks(i));
  }
  return out;
}

std::vector<uint64_t> generate_all_black_pawn_attacks() {
  std::vector<uint64_t> out;
  for (int i = 0; i <= 63; i++) {
    out.push_back(generate_black_pawn_attacks(i));
  }
  return out;
}

void save_white_pawn_attack_look_up_table() {

  vector<uint64_t> out = generate_all_white_pawn_attacks();
  ofstream outFile("all_white_pawn_attacks.txt");
  if (!outFile) {
    cerr << "Failed to open all_white_pawn_attacks.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &attack : out) {
    outFile << attack << '\n';
  }

  outFile.close();
}
void save_white_pawn_attacks_cache() {
  vector<uint64_t> white_pawn_attacks = generate_all_white_pawn_attacks();
  ofstream outFile("white_pawn_attacks.txt");
  if (!outFile) {
    cerr << "Failed to open white_pawn_attacks.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &attack : white_pawn_attacks) {
    // outFile << bitboard << ' ';
    // outFile << std::bitset<64>(bitboard).to_string() << ' ';
    print_board_os(outFile, attack);
    // cout << bitboard << endl;
    // outFile << '\n'; // new line for each inner vector
  }

  outFile.close();
}
void save_black_pawn_attack_look_up_table() {
  vector<uint64_t> out = generate_all_black_pawn_attacks();
  ofstream outFile("all_black_pawn_attacks.txt");
  if (!outFile) {
    cerr << "Failed to open all_black_pawn_attacks.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &attack : out) {
    outFile << attack << '\n';
  }

  outFile.close();
}
void save_black_pawn_attacks_cache() {
  vector<uint64_t> black_pawn_attacks = generate_all_black_pawn_attacks();
  ofstream outFile("black_pawn_attacks.txt");
  if (!outFile) {
    cerr << "Failed to open black_pawn_attacks.txt for writing\n";
    throw runtime_error("Failed to open file");
  }
  for (const auto &attack : black_pawn_attacks) {
    // outFile << bitboard << ' ';
    // outFile << std::bitset<64>(bitboard).to_string() << ' ';
    print_board_os(outFile, attack);
    // cout << bitboard << endl;
    // outFile << '\n'; // new line for each inner vector
  }

  outFile.close();
}
uint64_t generate_knight_attack_mask(int position) {
  /*        noNoWe    noNoEa
            +15  +17
             |     |
noWeWe  +6 __|     |__+10  noEaEa
              \   /
               >0<
           __ /   \ __
soWeWe -10   |     |   -6  soEaEa
             |     |
            -17  -15
        soSoWe    soSoEa
        */
  uint64_t attack_mask = 0;
  uint64_t position_mask = 0b1uLL << position;
  uint64_t noNoWe = position_mask << 15;
  uint64_t noNoEa = position_mask << 17;
  uint64_t noWeWe = position_mask << 6;
  uint64_t noEaEa = position_mask << 10;
  uint64_t soSoEa = position_mask >> 15;
  uint64_t soSoWe = position_mask >> 17;
  uint64_t soEaEa = position_mask >> 6;
  uint64_t soWeWe = position_mask >> 10;

  int linear_position = position;
  int column = linear_position % 8;
  int row = linear_position / 8;

  if (column < 1) {
    noNoWe = soSoWe = 0; // left edge
  }
  if (column < 2) {
    noWeWe = soWeWe = 0; // left edge
  }
  if (column > 6) {
    noNoEa = soSoEa = 0;
  }

  if (column > 5) {
    noEaEa = soEaEa = 0; // right edge
  }
  if (row < 2) {
    soSoWe = soSoEa = 0; // bottom edge
  }
  if (row < 1) {
    soWeWe = soEaEa = 0;
  }
  if (row > 5) {
    noNoWe = noNoEa = 0; // top edge
  }
  if (row > 6) {
    noWeWe = noEaEa = 0;
  }
  attack_mask =
      noNoWe | noNoEa | noWeWe | noEaEa | soSoEa | soSoWe | soEaEa | soWeWe;
  return attack_mask;
}
// uint64_t generate_king_attack_mask(int position){
// /*

// */
// }
std::vector<uint64_t> generate_knight_look_up_table() {
  std::vector<uint64_t> look_up_table;
  uint64_t attack_mask;
  for (int i = 0; i < 64; i++) {
    attack_mask = generate_knight_attack_mask(i);
    look_up_table.push_back(attack_mask);
  }
  return look_up_table;
}
void save_knight_look_up_table() {

  vector<uint64_t> out = generate_knight_look_up_table();
  ofstream outFile("knight_lookup_table.txt");
  if (!outFile) {
    cerr << "Failed to open knight_lookup_tables.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &attack : out) {
    outFile << attack << '\n';
  }

  outFile.close();
}
void save_knight_attacks_cache() {
  vector<uint64_t> knight_attacks = generate_knight_look_up_table();
  ofstream outFile("knight_attacks.txt");
  if (!outFile) {
    cerr << "Failed to open knight_attacks.txt for writing\n";
    throw runtime_error("Failed to open file");
  }

  for (const auto &attack : knight_attacks) {
    // outFile << bitboard << ' ';
    // outFile << std::bitset<64>(bitboard).to_string() << ' ';
    print_board_os(outFile, attack);
    // cout << bitboard << endl;
    // outFile << '\n'; // new line for each inner vector
  }

  outFile.close();
}

uint64_t generate_king_attack(int position) {
  uint64_t position_mask = 0b1uLL << position;
  std::vector<int> row_col = position_to_row_col(position_mask);
  int row = row_col[0];
  int col = row_col[1];

  uint64_t we = position_mask >> 1;
  uint64_t ea = position_mask << 1;
  uint64_t no = position_mask << 8;
  uint64_t so = position_mask >> 8;
  uint64_t noWe = position_mask << 7;
  uint64_t noEa = position_mask << 9;
  uint64_t soWe = position_mask >> 9;
  uint64_t soEa = position_mask >> 7;
  if (col == 0) {
    we = noWe = soWe = 0;
  }
  if (col == 7) {
    ea = noEa = soEa = 0;
  }
  if (row == 7) {
    no = noWe = noEa = 0;
  }
  if (row == 0) {
    so = soWe = soEa = 0;
  }
  return we | ea | no | so | noWe | noEa | soWe | soEa;
}
std::vector<uint64_t> generate_king_look_up_table() {
  std::vector<uint64_t> king_look_up_table;
  for (int i; i < 64; i++) {
    king_look_up_table.push_back(generate_king_attack(i));
  }
  return king_look_up_table;
}
void save_king_look_up_table() {
  std::string file_name = "king_look_up_table.txt";
  std::ofstream out(file_name);
  std::vector<uint64_t> king_look_up_table = generate_king_look_up_table();
  if (out) {
    for (auto attack : king_look_up_table) {
      out << attack << std::endl;
    }
  } else {
    std::cerr << "the file can not be opened";
  }
  out.close();
}
void save_king_attacks() {
  std::string file_name = "king_atacks.txt";
  std::ofstream out(file_name);
  std::vector<uint64_t> king_look_up_table = generate_king_look_up_table();
  if (out) {
    for (auto attack : king_look_up_table) {
      print_board_os(out, attack);
    }
  } else {
    std::cerr << "the file can not be opened";
  }
  out.close();
}
void save_rook_masks() {
  std::string file_name = "rook_mask_table.txt";
  std::ofstream out(file_name);
  if (out) {
    for (int i = 0; i < 64; i++) {

      out << remove_bit(generate_rook_mask(0b1ull << i, 8), i) << std::endl;
    }
  } else {
    std::cerr << "the file can not be opened";
  }
  out.close();
}
void save_rook_shifts() {
  std::string file_name = "rook_shifts.txt";
  std::ofstream out(file_name);
  if (out) {
    for (int i = 0; i < 64; i++) {

      out << __builtin_popcountll(
                 remove_bit(generate_rook_mask(0b1ull << i, 8), i))
          << std::endl;
    }
  } else {
    std::cerr << "the file can not be opened";
  }
  out.close();
}
void save_bishop_masks() {
  std::string file_name = "bishop_masks.txt";
  std::ofstream out(file_name);
  std::vector<uint64_t> bishop_masks = generate_bishop_masks();
  if (out) {
    for (int i = 0; i < 64; i++) {

      out << remove_bit(bishop_masks[i], i) << std::endl;
    }
  } else {
    std::cerr << "the file can not be opened";
  }
  out.close();
}
void save_bishop_shifts() {
  std::string file_name = "bishop_shifts.txt";
  std::ofstream out(file_name);
  std::vector<uint64_t> bishop_masks = generate_bishop_masks();
  if (out) {
    for (int i = 0; i < 64; i++) {

      out << __builtin_popcountll(remove_bit(bishop_masks[i], i)) << std::endl;
    }
  } else {
    std::cerr << "the file can not be opened";
  }
  out.close();
}

int main() {
  // vector<uint64_t> out2 = generate_bishop_masks();
  // vector<uint64_t> out3 = generate_rook_masks();
  // vector<uint64_t> out4 = generate_queen_masks();
  // vector<uint64_t> out5 = generate_relevant_rook_occupancy_masks(0b1ULL <<
  // 21); vector<uint64_t> out6 = generate_rook_attack_masks(0b1ULL << 21);
  // // save_magic_numbers();
  // vector<uint64_t> out7 =
  //     generate_rook_lookup_table(3459047088391718912, 0b1ULL << 21);
  // save_rook_lookup_tables();
  // cout << "inmasks" << endl;
  // cout << out5.size() << endl;
  // // cout << generate_rook_mask(0b1ULL, 8) << endl;
  // print_board(generate_rook_mask(0b1ULL << 21, 21));
  // print_board(out5[10]);
  // cout << "attack_mask" << endl;
  // print_board(generate_rook_attack_mask(out5[10], 0b1ULL << 21));
  // int i = 0;
  // // save_rook_attacks_cache();
  // cout << generate_magic_number(0b1ULL << 21) << endl;
  // for (auto mask : out7) {
  //   // cout << "relevant rook occ" << endl;
  //   // print_board(out5[i]);
  //   // cout << "attack mask" << endl;

  //   print_board(mask);
  //   // cout << mask << endl;
  //   // cout << "\n";
  //   // i++;
  // }
  // for (auto mask : out2) {
  //   print_board(mask);
  //   cout << "\n";
  // }
  /*
  save_bishop_magic_numbers();
  save_bishop_attacks_cache();
  save_bishop_lookup_tables();
*/
  // save_knight_attacks_cache();
  // save_knight_look_up_table();
  // save_black_pawn_attacks_cache();
  // save_white_pawn_attacks_cache();
  // save_white_pawn_attack_look_up_table();
  // save_black_pawn_attack_look_up_table();
  // save_king_look_up_table();
  // save_king_attacks();
  save_bishop_masks();
  save_bishop_shifts();
  save_rook_masks();
  save_rook_shifts();

  // uint64_t mask = generate_rook_mask(0b1ULL << 21, 8);
  // print_board(mask);
  return 0;
}