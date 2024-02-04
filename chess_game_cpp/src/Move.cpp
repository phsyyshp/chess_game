#include "Move.hpp"
#include "position.hpp"
// getters
uint32_t Move::getMoveInt() const { return moveNum; }
int Move::getFrom() const { return moveNum & 0x3f; }
int Move::getTo() const { return (moveNum >> 6) & 0x3f; }
int Move::getColor() const { return (moveNum >> 15) & 0b1; }
int Move::getPiece() const { return (moveNum >> 12) & 0b111; }
bool Move::checkIsCapture() const { return (moveNum >> 16) & 0x1; }
int Move::getScore() const { return (moveNum) >> 17; }
piece Move::getCaptured(const Position &position) const {
  return position.getMailbox()[getTo()];
}
// setters
// Warning! this does not override the already set score of a move. Must be used
// only if the score of the move is not yet given.
void Move::setScore(const int &score) { moveNum |= score << 17; }
// visualizers
void Move::print() const {
  std::cout << "move: " << chessSq[getFrom()] << chessSq[getTo()]
            << "color: " << getColor()
            << "piece: " << pieceToStr(static_cast<piece>(getPiece()))
            << "is capture: " << checkIsCapture() << "score: " << getScore()
            << std::endl;
}