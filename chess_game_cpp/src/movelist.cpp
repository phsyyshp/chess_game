#include "movelist.hpp"

void MoveList::push_back(Move move) { elements[lastSetIdx + 1] = move; }
int MoveList::size() const { return lastSetIdx; }
