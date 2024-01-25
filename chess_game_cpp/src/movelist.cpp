#include "movelist.hpp"

void MoveList::pushBack(Move move) { elements[lastSetIdx + 1] = move; }
