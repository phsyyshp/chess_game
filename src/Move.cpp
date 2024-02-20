#include "Move.hpp"
#include "position.hpp"
// getters
Move &Move::operator=(const Move &rhs) {
  moveNum = rhs.moveNum;
  return *this;
}
uint32_t Move::getMoveInt() const { return moveNum; }
uint Move::getFrom() const { return moveNum & 0x3f; }
uint Move::getTo() const { return (moveNum >> 6) & 0x3f; }
uint Move::getFlags() const {
  /*
+------+---------+--------+----------+----------+----------------------+
| Code | Promo.  | Capt.  | Special1 | Special0 | Kind of Move         |
+------+---------+--------+----------+----------+----------------------+
| 0    | 0       | 0      | 0        | 0        | Quiet moves          |
| 1    | 0       | 0      | 0        | 1        | Double pawn push     |
| 2    | 0       | 0      | 1        | 0        | King castle          |
| 3    | 0       | 0      | 1        | 1        | Queen castle         |
| 4    | 0       | 1      | 0        | 0        | Captures             |
| 5    | 0       | 1      | 0        | 1        | EP-capture           |
| 8    | 1       | 0      | 0        | 0        | Knight-promotion     |
| 9    | 1       | 0      | 0        | 1        | Bishop-promotion     |
| 10   | 1       | 0      | 1        | 0        | Rook-promotion       |
| 11   | 1       | 0      | 1        | 1        | Queen-promotion      |
| 12   | 1       | 1      | 0        | 0        | Knight-promo capture |
| 13   | 1       | 1      | 0        | 1        | Bishop-promo capture |
| 14   | 1       | 1      | 1        | 0        | Rook-promo capture   |
| 15   | 1       | 1      | 1        | 1        | Queen-promo capture  |
+------+---------+--------+----------+----------+----------------------+
*/
  return (moveNum >> 12) & 0xf;
}
int Move::getScore() const { return (moveNum) >> 17; }

// bools
bool Move::isCapture() const { return (moveNum & CAPTURE_FLAG) != 0; }

// setters
// Warning! this does not override the already set score of a move. Must be used
// only if the score of the move is not yet given.
void Move::setScore(const int &score) { moveNum |= score << 16; }

// visualizers
void Move::print() const {
  std::cout << chessSq[getFrom()] << chessSq[getTo()];
  if (getFlags() >= 6) {

    std::cout << chessSq[getFrom()] << chessSq[getTo()]
              << PROMOTION_TYPE_TO_STR[getFlags() - 6];
  }
}
std::string Move::toStr() const {
  if (getFlags() >= 6) {
    return chessSq[getFrom()] + chessSq[getTo()] +
           PROMOTION_TYPE_TO_STR[getFlags() - 6];
  }
  return chessSq[getFrom()] + chessSq[getTo()];
}
