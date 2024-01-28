#include "movelist.hpp"
#include "move_generation.hpp"
#include "position.hpp"
int main() {

  Position position;
  position.setBoardToInitialConfiguration();
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  MoveList moveList = movGen.getMoves();

  Move move = moveList[2];
  std::cout << chessSq[move.getFrom()] << chessSq[move.getTo()] << std::endl;
  std::cout << moveList.size() << std::endl;
  for (int i = 0; i < movGen.getMoves().size(); i++) {
    // for (const auto &move : movGen.getMoves()) {
    std::cout << "la" << std::endl;
    position.makeMove(movGen.getMoves()[i]);
    // if (!position.isInCheck()) {
    // }
  }
}