#include "UCI.hpp"
// returns unscored move;
Move UCI::getMove(const std::string &moveStr) const {

  color color_ = position.getTurn();
  color oppositeColor = position.getOppositeTurn();
  std::string fromStr = std::string(1, moveStr[0]) + moveStr[1];
  std::string toStr = std::string(1, moveStr[2]) + moveStr[3];
  int to = findIndex(chessSq, toStr);
  int from = findIndex(chessSq, fromStr);
  bool isCaptureVal = (0b1ull << to) & (position.getAllPieces(black));
  Move move(from, to, position.getPieceType(0b1ull << from), color_,
            isCaptureVal);
  return move;
}