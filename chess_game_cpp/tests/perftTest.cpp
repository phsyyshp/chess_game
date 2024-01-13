#include "move_generation.hpp"
std::vector<Position> perft(std::vector<Position> positions) {
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
int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  MoveGeneration moveGen;
  Position tempPosition;
  moveGen.generateKnightMoves(position, color::white);
  std::vector<Move> allMoves = moveGen.getMoves();
  std::vector<Position> out;

  for (auto move : allMoves) {
    tempPosition = position;
    tempPosition.makeMove(move);
    tempPosition.changeTurn();
    // out.push_back(tempPosition);
    tempPosition.printBoard();

    // std::vector<Position> positions = {position};
    // std::vector<Position> newPoses = perft(positions);
    // std::cout << newPoses.size() << std::endl;
  }
}