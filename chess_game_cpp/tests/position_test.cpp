#include "Move.hpp"
#include "position.hpp"
#include <iostream>
int main() {
  Position newPos;
  newPos.setBoardToInitialConfiguration();
  newPos.printBoard();
  Move move = Move(a2, a7, piece::pawn, color::white, true);
  newPos.makeMove(move);
  newPos.changeTurn();
  newPos.printBoard();
  newPos.changeTurn();

  newPos.undoMove(move);

  newPos.printBoard();
}