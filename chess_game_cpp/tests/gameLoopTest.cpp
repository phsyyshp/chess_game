#include "search.hpp"
template <typename T, size_t N>
int findIndex(const std::array<T, N> &arr, const T &value) {
  auto it = std::find(arr.begin(), arr.end(), value);
  return (it != arr.end()) ? std::distance(arr.begin(), it) : -1;
}

int main() {
  Position position;
  position.setBoardToInitialConfiguration();
  position.printBoard();
  std::string moveStr;
  while (std::cin >> moveStr) {
    std::string fromStr = std::string(1, moveStr[0]) + moveStr[1];
    std::string toStr = std::string(1, moveStr[2]) + moveStr[3];
    int to = findIndex(chessSq, toStr);
    int from = findIndex(chessSq, fromStr);
    bool isCaptureVal = (0b1ull << to) & (position.getAllPieces(black));
    Move move(from, to, position.getPieceType(0b1ull << from), white,
              isCaptureVal);

    MoveGeneration mg(position);
    if (!mg.isPseudoLegal(move)) {

      std::cout << "illegal move try again." << std::endl;
      continue;
    }
    position.makeMove(move);
    position.printBoard();
    Search srch(position);
    Move bestMove = srch.searchAB(5);
    position.makeMove(bestMove);
    position.printBoard();
  }
}