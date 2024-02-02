#include "search.hpp"
int main() {

  Position position;
  position.setBoardToInitialConfiguration();
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
