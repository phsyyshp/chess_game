#include "move_generation.hpp"
std::vector<Position> perftSingle(std::vector<Position> positions) {
  Position tempPosition;
  std::vector<Position> out;
  for (const auto &position : positions) {

    MoveGeneration movGen;
    movGen.generateAllMoves(position, position.getTurn());
    std::vector<Move> allMoves = movGen.getMoves();
    for (const auto &move : allMoves) {
      tempPosition = position;
      tempPosition.makeMove(move);
      tempPosition.changeTurn();
      out.push_back(tempPosition);
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
int main() { perft(5); }