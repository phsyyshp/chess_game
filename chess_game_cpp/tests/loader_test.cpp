#include "loader.hpp"
#include <iostream>
#include <vector>
void print_board(uint64_t pieces) {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      std::cout << ((0b1ULL << (j + i * 8)) & pieces ? '1' : '0');
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}

int main() {
  std::vector<uint64_t> rook_vec = read_magic_numbers_to_vec("rook");
  //   for (auto i : rook_vec) {
  //     std::cout << i << std::endl;
  //   }
  std::vector<std::vector<uint64_t>> rook_vec_vec = read_look_up_tables("rook");

  //   for (auto i : rook_vec_vec) {
  //     std::cout << std::endl;
  //     for (auto c : i) {
  //       std::cout << c << ' ';
  //     }
  //   }
  uint64_t attack_mask =
      get_attack_mask(0b1ULL << 25 - 8, 0b1ULL << (3 * 8 - 9 + 25), rook_vec,
                      rook_vec_vec, "rook");
  std::cout << "attack_mask" << std::endl;
  print_board(attack_mask);
  std::cout << "pos of piece" << std::endl;

  print_board(0b1ULL << 25 - 8);
  std::cout << "pos of obstacle" << std::endl;

  print_board(0b1ULL << 3 * 8 - 9 + 25);
  std::vector<uint64_t> bishop_vec = read_magic_numbers_to_vec("bishop");
  //   for (auto i : rook_vec) {
  //     std::cout << i << std::endl;
  //   }
  std::vector<std::vector<uint64_t>> bishop_vec_vec =
      read_look_up_tables("bishop");

  //   for (auto i : rook_vec_vec) {
  //     std::cout << std::endl;
  //     for (auto c : i) {
  //       std::cout << c << ' ';
  //     }
  //   }
  uint64_t bishop_attack_mask =
      get_attack_mask(0b1ULL << 25 - 8, 0b1ULL << (3 * 8 - 9 + 25), bishop_vec,
                      bishop_vec_vec, "bishop");
  std::cout << "attack_mask" << std::endl;
  print_board(bishop_attack_mask);
  std::cout << "pos of piece" << std::endl;

  print_board(0b1ULL << 25 - 8);
  std::cout << "pos of obstacle" << std::endl;

  print_board(0b1ULL << 3 * 8 - 9 + 25);
}