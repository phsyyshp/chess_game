#include "move_generation.hpp"
int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  uint64_t allPieces =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  std::cout << allPieces;
  MoveGeneration movGen;
  // movGen.generateAllMoves(position, color::white);
  movGen.generateKingMoves(position, color::white);
  std::vector<Move> allMoves = movGen.getMoves();
  std::cout << allMoves.size();
}