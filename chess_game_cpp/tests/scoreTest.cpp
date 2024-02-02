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
  s.scoreMoves(ml);
  for (Move move : ml) {
    std::cout << "move:" << chessSq[move.getFrom()] << chessSq[move.getTo()]
              << "score:" << move.getScore() << std::endl;
  }
}
