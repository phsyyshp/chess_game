
#include "perftTest.hpp"
#include "UCI.hpp"
#include "move_generation.hpp"

uint64_t perftTest::perftPseudoLegal(int depth) {
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
uint64_t perftTest::perft(int depth) {
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
uint64_t perftTest::perftBulk(int depth) {

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

void perftDivideInterface(std::string FENstr) {
  Position position(FENstr);
  Position tempPosition;
  int depth;
  std::string moveStr;
  UCI uci(position);
  position.printBoard();
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