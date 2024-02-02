#include "search.hpp"
int main() {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  //   position.setBoardToInitialConfiguration();
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  MoveList ml = movegen.getMoves();
  Search s(position);

  Move move_ = ml[ml.size() - 1];
  move_.print();
  s.scoreMoves(ml);
  Position tempPosition;
  Move move = ml[ml.size() - 1];
  move.print();
  //   for (Move move : ml) {
  //     move.print();
  //     tempPosition = position;
  //     position.makeMove(move);
  //     // position.printBoard();
  //     position = tempPosition;
  //   }
}
