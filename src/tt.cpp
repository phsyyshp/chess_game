#include "tt.hpp"

void TranspositionTable::add(const hashEntry &entry) {
  tt[entry.zobristKey % (TT_SIZE - 1)] = entry;
}
Move TranspositionTable::getMove(uint64_t zobristKey) const {
  Move move_ = tt[zobristKey % (TT_SIZE - 1)].move;
  move_.setScore(0);
  return move_;
}

hashEntry TranspositionTable::get(uint64_t zobristKey) const {

  return tt[zobristKey % (TT_SIZE - 1)];
}

void TranspositionTable::clear() {
  for (hashEntry &entry : tt) {
    entry = nullEntry;
  }
}