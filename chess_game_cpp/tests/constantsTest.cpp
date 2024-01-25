#include "constants.hpp"
#include "position.hpp"
int main() {

  Position position;
  position.setBoardToInitialConfiguration();
  color pieceColor = white;
  int tempSq;
  uint64_t remainingPawns = position.getPieces()[pieceColor][pawn];
  while (remainingPawns) {
    tempSq = __builtin_ctzll(remainingPawns);
    std::cout << tempSq << chessSq[tempSq] << std::endl;
    int out = pawnSqTbls[pieceColor][tempSq];
    std::cout << out << std::endl;
  }
}