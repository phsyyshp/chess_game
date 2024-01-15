#include "move_generation.hpp"
class perftTest {

public:
  int perftRec(int depth) {
    Position tempPosition;
    if (depth == 0) {
      return 1;
    }
    int nodes = 0;
    MoveGeneration movGen;
    movGen.generateAllMoves(position, position.getTurn());
    std::vector<Move> allMoves = movGen.getMoves();
    for (const auto &move : allMoves) {
      // FIX IT: do undo;
      tempPosition = position;
      position.makeMove(move);

      nodes += perftRec(depth - 1);
      // position = tempPosition;

      position.undoMove(move);
      if (position.getPieces() != tempPosition.getPieces()) {
        std::cout << "wrong"
                  << "\n";
        position.printBoard();
        std::cout << "correct"
                  << "\n";

        tempPosition.printBoard();
        std::cout << nodes << "\n";
        std::cout << position.capturedInLastMove << "\n";
        std::cout << position.getTurn() << "\n";

        break;
      }
    }
    return nodes;
  }

  Position position;
};
int main() {
  perftTest test;
  Position position;
  position.setBoardToInitialConfiguration();
  test.position = position;

  std::cout << test.perftRec(4);
}