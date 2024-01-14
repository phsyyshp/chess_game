#include "move_generation.hpp"
std::vector<Position> perftSingle(std::vector<Position> positions) {
  Position tempPosition;
  std::vector<Position> out;
  for (auto position : positions) {

    MoveGeneration movGen;
    movGen.generateAllMoves(position, position.getTurn());
    std::vector<Move> allMoves = movGen.getMoves();
    for (const auto &move : allMoves) {
      position.makeMove(move);
      position.changeTurn();
      out.push_back(position);
      position.undoMove(move);
      position.changeTurn();
    }
  }
  return out;
}
void perft(int depth) {
  Position position;
  position.setBoardToInitialConfiguration();
  std::vector<Position> positions;
  positions.reserve(10000000);
  positions.push_back(position);
  int i = 0;
  while (i != depth) {
    positions = perftSingle(positions);
    std::cout << positions.size() << std::endl;
    i++;
  }
}
int main() { perft(4); }