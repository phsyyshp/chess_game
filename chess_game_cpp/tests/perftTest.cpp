#include "move_generation.hpp"
class perftTest {

public:
  perftTest() = default;
  perftTest(Position pos) : position(pos){};
  uint64_t perftPseudoLegal(int depth) {
    Position tempPosition;
    if (depth == 0) {
      return 1;
    }
    int nodes = 0;
    MoveGeneration movGen(position);
    movGen.generateAllMoves();
    for (auto &move : movGen.getMoves()) {
      // FIX IT: do undo;
      tempPosition = position;
      position.makeMove(move);
      nodes += perftPseudoLegal(depth - 1);
      position = tempPosition;
    }
    return nodes;
  }
  uint64_t perft(int depth) {
    Position tempPosition;
    if (depth == 0) {
      return 1;
    }
    uint64_t nodes = 0;
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
  uint64_t perftBulk(int depth) {

    Position tempPosition;
    MoveGeneration movGen(position);
    movGen.generateAllMoves();
    uint64_t nodes = 0;
    if (depth == 1) {
      return movGen.getMoves().size();
    }
    for (auto &move : movGen.getMoves()) {
      tempPosition = position;
      position.makeMove(move);
      // if (!position.isInCheck()) {
      nodes += perftBulk(depth - 1);
      // }
      position = tempPosition;
    }
    return nodes;
  }

private:
  Position position;
};
MoveList perftDivide(Position position, int depth) {
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  Position tempPosition;
  int i = 0;
  for (const auto &move : movGen.getMoves()) {
    tempPosition = position;
    position.makeMove(move);
    perftTest test(position);
    if (i < 10) {

      std::cout << "│ " << i << "  │ " << chessSq[move.getFrom()]
                << chessSq[move.getTo()] << " │ "
                << test.perftPseudoLegal(depth - 1) << " │ " << std::endl;
    } else {

      std::cout << "│ " << i << " │ " << chessSq[move.getFrom()]
                << chessSq[move.getTo()] << " │ "
                << test.perftPseudoLegal(depth - 1) << " │ " << std::endl;
    }

    position = tempPosition;
    i++;
  }
  return movGen.getMoves();
}
void perftDivideInterface() {
  Position position;
  Position tempPosition;
  int depth;
  char idx;
  position.setBoardToInitialConfiguration();
  std::cout << "enter the depth:" << std::endl;
  std::cin >> depth;
  std::cout << "┌────┬──────┬──────────" << std::endl;

  MoveList out = perftDivide(position, depth);

  std::cout << "enter index of the move:" << std::endl;

  while (std::cin >> idx) {
    if (depth == 1 || idx == 'q') {
      break;
    }
    tempPosition = position;

    position.makeMove(out[idx]);
    depth--;

    position.printBoard();
    std::cout << "moves in depth: " << depth << std::endl;
    std::cout << "┌────┬──────┬──────────" << std::endl;

    out = perftDivide(position, depth);

    std::cout << "enter index of the move for next depth, q to quit:"
              << std::endl;
    position = tempPosition;
  }
}

int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  perftTest test(position);
  int depth = 6;

  std::cout << "Number Of Pseudo-Legal Moves:" << std::endl;
  std::cout << test.perftBulk(depth) << std::endl;
  // std::cout << test.perftPseudoLegal(depth) << std::endl;

  // std::cout << "Number Of Legal Moves:" << std::endl;
  // std::cout << test.perft(depth) << std::endl;
  // perftDivideInterface();
}