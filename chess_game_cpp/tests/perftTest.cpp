#include "move_generation.hpp"
class perftTest {

public:
  perftTest() = default;
  perftTest(Position pos) : position(pos) {}
  int perft(int depth) {
    Position tempPosition;
    if (depth == 0) {
      return 1;
    }
    int nodes = 0;
    MoveGeneration movGen(position);
    movGen.generateAllMoves();
    // std::vector<Move> allMoves = movGen.getMoves();
    for (const auto &move : movGen.getMoves()) {
      // FIX IT: do undo;
      tempPosition = position;
      position.makeMove(move);

      nodes += perft(depth - 1);
      position = tempPosition;
    }
    return nodes;
  }

  Position position;
};
int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  perftTest test(position);

  std::cout << test.perft(5);
}