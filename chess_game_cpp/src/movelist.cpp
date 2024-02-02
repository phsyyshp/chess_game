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