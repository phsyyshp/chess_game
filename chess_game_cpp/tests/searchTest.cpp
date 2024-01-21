#include "search.hpp"

template <typename T, size_t N>
int findIndex(const std::array<T, N> &arr, const T &value) {
  auto it = std::find(arr.begin(), arr.end(), value);
  return (it != arr.end()) ? std::distance(arr.begin(), it) : -1;
}

int main() {

  //   std::string FEN = "r1b1kb1r/1pqn1ppp/pn6/4P3/3NP1P1/2P1BQ2/PP5P/R3KB1R/";
  //   Position position(FENtoPieces(FEN), white);
  //   position.printBoard();
  //   Evaluation eval(position);
  //   Search search(position);
  //   search.negaMax(3);
  //   Move bestMove = search.getBestMove();
  //   std::string toSqStr = chessSq[bestMove.getTo()];
  //   std::string fromSqStr = chessSq[bestMove.getFrom()];

  //   std::cout << fromSqStr << " " << toSqStr << std::endl;
  //   position.makeMove(bestMove);
  Position position;
  position.setBoardToInitialConfiguration();
  position.printBoard();
  std::string moveStr;

  while (std::cin >> moveStr) {
    std::string fromStr = std::string(1, moveStr[0]) + moveStr[1];
    std::string toStr = std::string(1, moveStr[2]) + moveStr[3];
    int to = findIndex(chessSq, toStr);
    int from = findIndex(chessSq, fromStr);
    Move move(from, to, position.getPieceType(0b1ull << from), white,
              (0b1ull << to) & (position.getAllPieces(black)));
    position.makeMove(move);
    Search search(position);
    search.negaMax(3);
    Move bestMove = search.getBestMove();
    position.makeMove(bestMove);
    position.printBoard();
  }
}