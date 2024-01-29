#include "move_generation.hpp"
// #include <string>
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
  // Warning: There may be a bug here!
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
    std::cout << i << " " << chessSq[move.getFrom()] << chessSq[move.getTo()]
              << " " << test.perftPseudoLegal(depth - 1) << " " << std::endl;
    position = tempPosition;
    i++;
  }
  return movGen.getMoves();
}
void perftDivideInterface() {
  Position position;
  int depth;
  int idx;
  position.setBoardToInitialConfiguration();
  std::cout << "depth:" << std::endl;
  std::cin >> depth;
  MoveList out = perftDivide(position, depth);

  std::cout << "choose the move:" << std::endl;
  while (std::cin >> idx) {
    if (depth == 1 || idx == 'q') {
      break;
    }
    position.makeMove(out[idx]);
    depth--;

    std::cout << "moves in depth " << depth << std::endl;
    out = perftDivide(position, depth);
  }
}
int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  perftTest test(position);
  int depth = 5;
  MoveGeneration moveGen(position);

  // moveGen.generateAllMoves();
  // MoveList moveList = moveGen.getMoves();
  // auto move_ = moveList[0];
  // std::cout << chessSq[move_.getFrom()] << chessSq[move_.getTo()] <<
  // std::endl; for (auto move : moveList) {
  //   std::cout << chessSq[move.getFrom()] << chessSq[move.getTo()] <<
  //   std::endl;
  // }

  // while (depth > 0) {
  // std::cout << "Number Of Pseudo-Legal Moves:" << std::endl;
  // std::cout << test.perftBulk(depth) << std::endl;
  // std::cout << test.perftPseudoLegal(depth) << std::endl;

  // std::cout << "Number Of Legal Moves:" << std::endl;
  // std::cout << test.perft(depth) << std::endl;
  // std::cout << "Divided Perft"
  // << "\n";
  // depth--;
  // }
  // Move move(b1, a3, knight, white, false);
  // position.makeMove(move);

  // Move move2(g7, g6, pawn, black, false);
  // position.makeMove(move);

  // perftDivide(position, 1);
  perftDivideInterface();
}