#include "loader.hpp"
#include <iostream>
#include <vector>
int main() {
  std::vector<uint64_t> rook_vec = read_magic_numbers("rook");
  for (auto i : rook_vec) {
    std::cout << i << std::endl;
  }
  std::vector<std::vector<uint64_t>> rook_vec_vec = read_look_up_tables("rook");

  for (auto i : rook_vec_vec) {
    std::cout << std::endl;
    for (auto c : i) {
      std::cout << c << ' ';
    }
  }
}