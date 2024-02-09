#include "UCI.hpp"
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
      if (position.makeMove(move)) {
        nodes += perft(depth - 1);
      }
      position = tempPosition;
    }
    return nodes;
  }
  // FIX IT: the exit node gives wrong number due to illegal moves;
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
      if (position.makeMove(move)) {
        nodes += perftBulk(depth - 1);
      }
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
  int nodes = 0;
  int totalNodes = 0;
  MoveList ml;
  std::vector<std::string> printVec;
  for (const auto &move : movGen.getMoves()) {
    tempPosition = position;
    if (position.makeMove(move)) {
      ml.push_back(move);
      perftTest test(position);
      nodes = test.perft(depth - 1);
      totalNodes += nodes;
      std::string perftResult = "│ " + chessSq[move.getFrom()] +
                                chessSq[move.getTo()] + " │ " +
                                std::to_string(nodes);
      printVec.push_back(perftResult);
    }
    position = tempPosition;
  }

  std::sort(printVec.begin(), printVec.end());
  for (auto line : printVec) {
    std::cout << line << std::endl;
  }
  std::cout << "  Total:" << totalNodes << std::endl;
  return ml;
}
void perftDivideInterface() {
  Position position;
  Position tempPosition;
  int depth;
  std::string moveStr;
  position.setBoardToInitialConfiguration();
  UCI uci(position);
  std::cout << "enter the depth:" << std::endl;
  std::cin >> depth;
  std::cout << "┌──────┬─────────" << std::endl;

  MoveList out = perftDivide(position, depth);

  std::cout << "enter index of the move:" << std::endl;

  while (std::cin >> moveStr) {
    if (depth == 1 || moveStr == "quit") {
      break;
    }
    position.makeMove(uci.getMove(moveStr));
    position.printBoard();
    depth--;

    // position.printBoard();
    std::cout << "moves in depth: " << depth << std::endl;
    std::cout << "┌────┬──────┬──────────" << std::endl;

    out = perftDivide(position, depth);

    std::cout << "enter index of the move for next depth, q to quit:"
              << std::endl;
  }
}

int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  perftTest test(position);
  int depth = 5;

  // std::cout << "Number Of Legal Moves(bulk):" << std::endl;
  // std::cout << test.perftBulk(depth) << std::endl;
  // std::cout << test.perftPseudoLegal(depth) << std::endl;
  std::cout << "Number Of Legal Moves:" << std::endl;
  std::cout << test.perft(depth) << std::endl;
  perftDivideInterface();
  // UCI uci(position);

  // position.makeMove(uci.getMove("a2a4"));
  // position.makeMove(uci.getMove("b7b5"));

  // position.makeMove(uci.getMove("a4b5"));
  // position.makeMove(uci.getMove("a7a5"));

  // perftDivide(position, depth);
}