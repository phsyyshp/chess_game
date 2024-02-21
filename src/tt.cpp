#include "tt.hpp"

void TranspositionTable::add(const hashEntry &entry) {
  tt[entry.zobristKey % TT_SIZE] = entry;
}
Move TranspositionTable::getMove(uint64_t zobristKey) const {
  return tt[zobristKey % TT_SIZE].move;
}

hashEntry TranspositionTable::get(uint64_t zobristKey) const {

  return tt[zobristKey % TT_SIZE];
}

void TranspositionTable::clear() {
  for (hashEntry &entry : tt) {
    entry = nullEntry;
  }
}