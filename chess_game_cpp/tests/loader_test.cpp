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
  // TO DO: add here a very well established debugger and finally solve this
  // issue.

  // test 1
  std::cout << "rook magic number tests" << std::endl;
  uint64_t occupancy = (0b1ull) << b3;

  std::cout << "occupancy" << std::endl;
  print_board(occupancy);

  square sq = a1;
  uint64_t occupancyTemp = occupancy & rookTbls[sq].mask;
  auto magicIdx =
      (occupancyTemp * rookTbls[sq].magicNum) >> rookTbls[sq].shiftBit;
  std::cout << "rook attacks" << std::endl;
  print_board(rookLookUpTables[sq][magicIdx]);

  // test 2
  std::cout << "rook magic number tests" << std::endl;
  occupancy = (0b1ull) << d7;

  std::cout << "occupancy" << std::endl;
  print_board(occupancy);
  sq = d5;
  occupancyTemp = occupancy & rookTbls[sq].mask;
  std::cout << "occupancy temp " << std::endl;
  print_board(occupancyTemp);

  magicIdx = (occupancyTemp * rookTbls[sq].magicNum) >> (rookTbls[sq].shiftBit);

  std::cout << "magic index" << std::endl;
  std::cout << magicIdx << std::endl;
  std::cout << "rook attacks" << std::endl;
  print_board(rookLookUpTables[sq][magicIdx]);
}