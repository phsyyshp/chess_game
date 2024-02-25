
#include "perftTest.hpp"

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
uint64_t perftTest::perft(int depth, const Position &position) {
  Position tempPosition;
  if (depth == 0) {
    return 1;
  }
  uint64_t nodes = 0;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (const auto &move : movGen.getMoves()) {
    tempPosition = position;
    if (tempPosition.makeMove(move)) {
      nodes += perft(depth - 1, tempPosition);
    }
  }
  return nodes;
}
uint64_t perftTest::perftZobrist(uint64_t depth, const Position &position) {
  Position tempPosition;
  if (depth == 0) {
    return 1;
  }
  uint64_t nodes = 0;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  if (tt[(position.getZobrist() ^ depth) % TT_SIZE].zobristKey != 0) {
    if (position.getZobrist() ==
        tt[(position.getZobrist() ^ depth) % TT_SIZE].zobristKey) {
      // std::cout << "la\n";
      return tt[(position.getZobrist() ^ depth) % TT_SIZE].nodes;
    }
  }
  for (const auto &move : movGen.getMoves()) {
    tempPosition = position;
    if (tempPosition.makeMove(move)) {
      // if (position.getZobrist() !=
      // Zobrist::generateTotalZobristKey(position)) {
      //   std::cout << move.toStr() << "\n";
      //   position.printBoard();
      //   assert(position.getZobrist() ==
      //          Zobrist::generateTotalZobristKey(position));
      // }
      // std::cout << "0\n";
      nodes += perftZobrist(depth - 1, tempPosition);
    }
  }
  tt[(position.getZobrist() ^ depth) % TT_SIZE] =
      perftTTentry{position.getZobrist(), nodes};
  return nodes;
}
// FIX IT: the exit node gives wrong number due to illegal moves;
uint64_t perftTest::perftBulk(int depth, const Position &position) {

  Position tempPosition;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  uint64_t nodes = 0;
  if (depth == 1) {
    return movGen.getMoves().size();
  }
  for (auto &move : movGen.getMoves()) {
    tempPosition = position;
    if (tempPosition.makeMove(move)) {
      nodes += perftBulk(depth - 1, tempPosition);
    }
  }
  return nodes;
}

MoveList perftDivide(const Position &position, int depth) {
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  Position tempPosition;
  int nodes = 0;
  int totalNodes = 0;
  MoveList ml;
  std::vector<std::string> printVec;
  for (const auto &move : movGen.getMoves()) {
    tempPosition = position;
    if (tempPosition.makeMove(move)) {
      ml.push_back(move);
      perftTest test(tempPosition);
      nodes = test.perft(depth - 1, tempPosition);
      totalNodes += nodes;
      std::string perftResult =
          "│ " + move.toStr() + " | " + std::to_string(nodes);
      printVec.push_back(perftResult);
    }
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
  moveToStr uci(position);
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
  moveToStr uci(position);
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