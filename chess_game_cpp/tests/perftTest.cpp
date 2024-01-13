#include "move_generation.hpp"
std::vector<Position> perftSingle(std::vector<Position> positions) {
  Position tempPosition;
  std::vector<Position> out;
  for (auto position : positions) {

    MoveGeneration movGen;
    movGen.generateAllMoves(position, position.getTurn());
    std::vector<Move> allMoves = movGen.getMoves();
    for (auto move : allMoves) {
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

  std::vector<Position> positions = {position};
  int i = 0;
  while (i != depth) {
    positions = perftSingle(positions);

    std::cout << positions.size() << std::endl;

    i++;
  }
}
int main() {
  // Position position;
  // position.setBoardToInitialConfiguration();

  // std::vector<Position> positions = {position};
  // std::vector<Position> newPoses = perftSingle(positions);
  // std::cout << newPoses.size() << std::endl;

  // newPoses = perftSingle(newPoses);
  // newPoses = perftSingle(newPoses);
  // newPoses = perftSingle(newPoses);
  // newPoses = perftSingle(newPoses);

  // std::cout << newPoses.size() << std::endl;
  // // for (auto pos : newPoses) {
  // //   pos.printBoard();
  // // }
  perft(4);
}