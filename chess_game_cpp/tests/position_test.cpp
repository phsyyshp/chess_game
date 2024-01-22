#include "Move.hpp"
#include "position.hpp"
#include <iostream>
int main() {
  Position newPos;
  newPos.setBoardToInitialConfiguration();
  newPos.printBoard();
  Move move = Move(d2, d3, piece::pawn, color::white, false);
  newPos.makeMove(move);
  newPos.printBoard();

  move = Move(g8, h6, piece::knight, color::black, false);

  newPos.makeMove(move);
  newPos.printBoard();
  move = Move(c1, h6, piece::bishop, color::white, true);

  newPos.makeMove(move);

  newPos.printBoard();
  newPos.undoMove(move);

  newPos.printBoard();

  std::string FENstr = "rnbqk1nr/pppp1ppp/8/4p3/1b1P4/5N2/PPP1PPPP/RNBQKB1R/";
  std::array<std::array<uint64_t, 6>, 2> FEN = FENtoPieces(FENstr);
  Position position2(FEN, white);
  position2.printBoard();
  std::cout << position2.isInCheck();
}