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
}