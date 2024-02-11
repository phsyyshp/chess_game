
#include "moveToStr.hpp"
// returns unscored move;
// Move UCI::getMove(const std::string &moveStr) const {

//   color color_ = position.getTurn();
//   color oppositeColor = position.getOppositeTurn();
//   std::string fromStr = std::string(1, moveStr[0]) + moveStr[1];
//   std::string toStr = std::string(1, moveStr[2]) + moveStr[3];
//   int to = findIndex(chessSq, toStr);
//   int from = findIndex(chessSq, fromStr);
//   uint flag = !position.isEmpty(to) * 4;

//   Move move(from, to, flag);
//   return move;
// }

Move moveToStr::getMove(const std::string &moveStr) const {
  if (isalpha(moveStr[moveStr.length() - 1])) {
  }

  color color_ = position.getTurn();
  color oppositeColor = position.getOppositeTurn();
  std::string fromStr = std::string(1, moveStr[0]) + moveStr[1];
  std::string toStr = std::string(1, moveStr[2]) + moveStr[3];
  int to = findIndex(chessSq, toStr);
  int from = findIndex(chessSq, fromStr);
  int moveType_;
  int promotionType;
  MoveGeneration mg(position);
  mg.generateAllMoves();
  for (const Move &move : mg.getMoves()) {
    int tempTo = move.getTo();
    int tempFrom = move.getFrom();
    int tempFlag = move.getFlags();

    if (isalpha(moveStr[moveStr.length() - 1])) {

      promotionType = findIndex(PROMOTION_TYPE_TO_STR_SHORT,
                                moveStr[moveStr.length() - 1]) +
                      6;
      moveType_ = promotionType + 4 * (!position.isEmpty(to));
      if (tempTo == to && tempFrom == from && moveType_ == tempFlag) {
        return move;
      }
    } else if (tempTo == to && tempFrom == from) {
      return move;
    }
  }
}