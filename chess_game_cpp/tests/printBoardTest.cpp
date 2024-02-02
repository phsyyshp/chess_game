#include "position.hpp"
int main() {
  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  Move move(d2, d6, queen, white, true);
  std::cout << "before the move" << std::endl;
  position.printBoard();
  position.makeMove(move);
  std::cout << "after the move" << std::endl;
  position.printBoard();
}