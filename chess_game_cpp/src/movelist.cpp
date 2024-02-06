#include "movelist.hpp"

void MoveList::push_back(Move move) {
  elements[lastSetIdx + 1] = move;
  lastSetIdx++;
}
int MoveList::size() const { return lastSetIdx + 1; }
// Warning! Assuming valid i, j.
void MoveList::swap(int i, int j) {
  auto temp = elements[j];
  elements[j] = elements[i];
  elements[i] = temp;
}
// TODO: possible optimizations here.
MoveList MoveList::getCapturedMoves() const {
  MoveList captureMoves;
  for (int i = 0; i < (lastSetIdx + 1); i++) {
    if (elements[i].isCapture()) {
      captureMoves.push_back(elements[i]);
    }
  }
  return captureMoves;
}