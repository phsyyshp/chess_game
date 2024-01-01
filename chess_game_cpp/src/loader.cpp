#include "loader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
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
std::vector<uint64_t> read_magic_numbers(std::string piece) {
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
std::vector<int> position_to_row_col(uint64_t position) {

  int linear_position = __builtin_ctzll(position);
  int column = linear_position % 8;
  int row = (linear_position / 8);
  return {row, column};
}
int rook_relevant_bits(uint64_t position) {
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
uint64_t generate_magic_index(uint64_t bitboard, uint64_t magic_number,
                              int shiftBits) {
  return (bitboard * magic_number) >> shiftBits;
}
uint64_t get_rook_attack_mask(u_int64_t position, u_int64_t bitboard) {
  int bits = rook_relevant_bits(position);
  int shiftBits = 64 - bits;
  uint64_t magic_index =
      generate_magic_index(bitboard, magic_number, shiftBits);
}
