#include "move_generation.hpp"
class perftTest {

public:
  perftTest() = default;
  perftTest(Position pos) : position(pos) {}
  int perftPseudoLegal(int depth) {
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
      nodes += perftPseudoLegal(depth - 1);
      position = tempPosition;
    }
    return nodes;
  }
  int perft(int depth) {
    Position tempPosition;
    if (depth == 0) {
      return 1;
    }
    int nodes = 0;
    MoveGeneration movGen(position);
    movGen.generateAllMoves();
    for (const auto &move : movGen.getMoves()) {
      tempPosition = position;
      position.makeMove(move);
      if (!position.isInCheck()) {
        nodes += perft(depth - 1);
      }

      position = tempPosition;
    }
    return nodes;
  }
  Position position;
};

void perftDivide(Position position, int depth) {
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  Position tempPosition;
  for (const auto &move : movGen.getMoves()) {
    tempPosition = position;
    position.makeMove(move);
    perftTest test(position);
    std::cout << chessSq[move.getFrom()] << chessSq[move.getTo()] << std::endl;
    std::cout << test.perft(depth - 1) << std::endl;
    position = tempPosition;
  }
}
int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  perftTest test(position);
  int depth = 4;

  std::cout << "Number Of Pseudo-Legal Moves:" << std::endl;
  std::cout << test.perftPseudoLegal(depth) << std::endl;

  std::cout << "Number Of Legal Moves:" << std::endl;
  std::cout << test.perft(depth) << std::endl;
  std::cout << "Divided Perft"
            << "\n";
  Move move(b1, a3, knight, white, false);
  position.makeMove(move);

  Move move2(g7, g6, pawn, black, false);
  position.makeMove(move);

  perftDivide(position, 1);
}