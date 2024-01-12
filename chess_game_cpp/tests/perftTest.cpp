#include "move_generation.hpp"
int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  MoveGeneration movGen;
  movGen.generateAllMoves(position, color::white);
  std::vector<Move> allMoves = movGen.getMoves();
  std::cout << allMoves.size();
}