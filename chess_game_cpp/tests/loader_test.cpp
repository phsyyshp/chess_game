#include "loader.hpp"
#include "position.hpp"
#include "tables.hpp"
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
  Position position;
  position.setBoardToInitialConfiguration();
  std::vector<std::vector<uint64_t>> board = position.getPieces();
  uint64_t occupancy = board[white][all] | board[black][all];
  // TO DO: add here a very well established debugger and finally solve this
  // issue.
  //  uint64_t attack_mask = getRookAttackMask(c5, occupancy);
  //  std::cout << "attack_mask" << std::endl;
  //  print_board(attack_mask);
  //  std::cout << "pos of piece" << std::endl;
  //  print_board(0b1ULL << c5);
  std::cout << "occupancy" << std::endl;
  int shiftBit = rookTbls[a1].shiftBit;
  square sq = a1;
  uint64_t occupancyTemp = occupancy & rookTbls[sq].mask;
  auto magicIdx =
      (occupancyTemp * rookTbls[sq].magicNum) >> rookTbls[sq].shiftBit;
  print_board(occupancy);
  // std::cout << rookTbls[sq].magicNum << std::endl;
  // std::cout << shiftBit << std::endl;
  // std::cout << magicIdx << std::endl;
  print_board(rookLookUpTables[sq][magicIdx]);
  // uint64_t bishop_attack_mask = getBishopAttackMask(c5, occupancy);
  // std::cout << "___------------------------------------------" << std::endl;

  // std::cout << "bishop stuff" << std::endl;
  // std::cout << "attack_mask" << std::endl;
  // print_board(bishop_attack_mask);
  // std::cout << "pos of piece" << std::endl;

  // print_board(0b1ULL << 25 - 8);
  // std::cout << "pos of obstacle" << std::endl;

  // print_board(0b1ULL << 3 * 8 - 9 + 25);
}