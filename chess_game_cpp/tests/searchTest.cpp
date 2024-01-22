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
  int i = 0;
  while (i < 2) {
    (std::cin >> moveStr);
    std::string fromStr = std::string(1, moveStr[0]) + moveStr[1];
    std::string toStr = std::string(1, moveStr[2]) + moveStr[3];
    int to = findIndex(chessSq, toStr);
    int from = findIndex(chessSq, fromStr);
    bool isCaptureVal = (0b1ull << to) & (position.getAllPieces(black));
    Move move(from, to, position.getPieceType(0b1ull << from), white,
              isCaptureVal);
    position.makeMove(move);
    position.printBoard();
    Search search(position);
    search.negaMax(3);
    Move bestMove = search.getBestMove();
    position.makeMove(bestMove);
    position.printBoard();
    i++;
  }
}