#include "constants.hpp"
#include "evaluate.hpp"
#include "position.hpp"
int main() {

  Position position;
  position.setBoardToInitialConfiguration();
  color pieceColor = white;
  int tempSq;
  uint64_t remainingPawns = position.getPieces()[pieceColor][pawn];
  while (remainingPawns) {
    tempSq = __builtin_ctzll(remainingPawns);
    std::cout << "square is:" << tempSq
              << "the indice of that sq:" << chessSq[tempSq] << std::endl;
    int out = pawnSqTbls[pieceColor][tempSq];
    std::cout << out << std::endl;
    remainingPawns ^= (0b1ull << tempSq);
  }
}