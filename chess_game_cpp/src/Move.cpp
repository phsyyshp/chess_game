#include "Move.hpp"
#include "position.hpp"
// getters
uint32_t Move::getMoveInt() const { return moveNum; }
int Move::getFrom() const { return moveNum & 0x3f; }
int Move::getTo() const { return (moveNum >> 6) & 0x3f; }
bool Move::isCapture() const { return (moveNum >> 16) & 0x1; }
int Move::getScore() const { return (moveNum) >> 17; }
// setters
// Warning! this does not override the already set score of a move. Must be used
// only if the score of the move is not yet given.
void Move::setScore(const int &score) { moveNum |= score << 17; }
// visualizers
void Move::print() const {
  std::cout << "move: " << chessSq[getFrom()] << chessSq[getTo()]
            << "is capture: " << isCheck() << "score: " << getScore()
            << std::endl;
}