#include "loader.hpp"
// #include <fstream>
// #include <iostream>
// #include <vector>
std::vector<uint64_t> line_to_nums_vec(const std::string &line) {
  std::vector<uint64_t> out;
  std::string number;
  for (auto c : line) {
    if (c != ' ') {
      number += c;
    } else {
      out.push_back(stoull(number));
      number.clear();
    }
  }
  return out;
}
std::vector<uint64_t> read_magic_numbers_to_vec(std::string piece) {
  std::string file_name = "mask_cache/" + piece + "_magic_numbers.txt";
  std::vector<uint64_t> out;
  std::string temp;
  std::fstream in(file_name);
  if (in) {
    while (getline(in, temp)) {
      out.push_back(stoull(temp));
    }
  } else {
    std::cerr << "the " << file_name << " can not be opened" << std::endl;
  }
  in.close();
  return out;
}
std::vector<std::vector<uint64_t>> read_look_up_tables(std::string piece) {
  std::string file_name = "mask_cache/" + piece + "_look_up_tables.txt";
  std::vector<std::vector<uint64_t>> look_up_tables;
  std::string line;
  std::fstream in(file_name);
  std::vector<uint64_t> look_up_table;
  std::string number;
  if (in) {
    while (getline(in, line)) {
      look_up_table = line_to_nums_vec(line);
      look_up_tables.push_back(look_up_table);
    }
  } else {
    std::cerr << "the " << file_name << " can not be opened" << std::endl;
  }
  in.close();
  return look_up_tables;
}
std::vector<u_int64_t> read_knight_look_up_table() {

  std::string piece_type = "knight";
  std::string file_name = "mask_cache/" + piece_type + "_look_up_tables.txt";
  std::string line;
  std::fstream in(file_name);
  std::vector<uint64_t> look_up_table;
  std::string number;
  if (in) {
    while (getline(in, line)) {
      look_up_table.push_back(stoll(line));
    }
  } else {
    std::cerr << "the " << file_name << " can not be opened" << std::endl;
  }
  in.close();
  return look_up_table;
};

int get_linear_position(const uint64_t &position) {
  // Warning!! it starts from 0, i.e. a1 square is 0;
  return __builtin_ctzll(position);
}
std::vector<int> position_to_row_col(const uint64_t &position) {

  int linear_position = get_linear_position(position);
  int column = linear_position % 8;
  int row = (linear_position / 8);
  return {row, column};
}
int rook_relevant_bits(const uint64_t &position) {
  std::vector<int> row_col_vec = position_to_row_col(position);
  int row = row_col_vec[0];
  int column = row_col_vec[1];
  int relevant_bits;
  bool is_at_sides = (column == 0 || column == 7);
  bool is_at_topbot = (row == 0 || row == 7);
  if ((is_at_topbot && !is_at_sides) || (!is_at_topbot && is_at_sides)) {
    relevant_bits = 11;
  } else if (!is_at_topbot && !is_at_sides) {
    relevant_bits = 10;
  } else {
    relevant_bits = 12;
  }
  return relevant_bits;
}

int bishop_relevant_bits(const uint64_t &position) {
  std::vector<int> row_col_vec = position_to_row_col(position);
  int row = row_col_vec[0];
  int column = row_col_vec[1];

  // Diagonal distances: min distance to any corner in each diagonal direction
  int dist_NE_SW = std::min(row, column);     // Northeast-Southwest diagonal
  int dist_NW_SE = std::min(row, 7 - column); // Northwest-Southeast diagonal
  int dist_SE_NW = std::min(7 - row, column); // Southeast-Northwest diagonal
  int dist_SW_NE =
      std::min(7 - row, 7 - column); // Southwest-Northeast diagonal

  // Maximum distance to the edge in any diagonal direction
  int max_diagonal_distance =
      std::max({dist_NE_SW, dist_NW_SE, dist_SE_NW, dist_SW_NE});

  // Total number of squares on the longest diagonal minus the square the bishop
  // is on
  int relevant_bits = 2 * max_diagonal_distance + 1;

  return relevant_bits;
}

uint64_t generate_magic_index(const uint64_t &bitboard,
                              const uint64_t &magic_number, int shiftBits) {
  return (bitboard * magic_number) >> shiftBits;
}
uint64_t
get_attack_mask(const u_int64_t &position, const u_int64_t &bitboard,
                const std::vector<uint64_t> &magic_numbers,
                const std::vector<std::vector<uint64_t>> &look_up_tables,
                std::string piece_type) {
  // TODO: dont forget to generalize this to bishop;
  int shiftBits;
  if (piece_type == "rook") {

    shiftBits = 64 - rook_relevant_bits(position);
  } else if (piece_type == "bishop") {

    shiftBits = 64 - bishop_relevant_bits(position);
  } else {
    std::cerr << "wrong piece type";
    return 0;
  }
  int linear_position = get_linear_position(position);
  uint64_t magic_index =
      generate_magic_index(bitboard, magic_numbers[linear_position], shiftBits);
  return look_up_tables[linear_position][magic_index];
}
