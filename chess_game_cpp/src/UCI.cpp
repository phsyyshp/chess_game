#include "UCI.hpp"
// returns unscored move;
Move UCI::getMove(const std::string &moveStr) const {

  color color_ = position.getTurn();
  color oppositeColor = position.getOppositeTurn();
  std::string fromStr = std::string(1, moveStr[0]) + moveStr[1];
  std::string toStr = std::string(1, moveStr[2]) + moveStr[3];
  int to = findIndex(chessSq, toStr);
  int from = findIndex(chessSq, fromStr);
  uint isCaptureFlag = !position.isEmpty(to) * 4;

  Move move(from, to, isCaptureFlag);
  return move;
}