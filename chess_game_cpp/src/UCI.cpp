#include "UCI.hpp"
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

Move UCI::getMove(const std::string &moveStr) const {
  color color_ = position.getTurn();
  color oppositeColor = position.getOppositeTurn();
  std::string fromStr = std::string(1, moveStr[0]) + moveStr[1];
  std::string toStr = std::string(1, moveStr[2]) + moveStr[3];
  int to = findIndex(chessSq, toStr);
  int from = findIndex(chessSq, fromStr);
  MoveGeneration mg(position);
  mg.generateAllMoves();
  for (const Move &move : mg.getMoves()) {
    int tempTo = move.getTo();
    int tempFrom = move.getFrom();

    if (tempTo == to && tempFrom == from) {
      return move;
    }
  }
}