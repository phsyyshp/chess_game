#include "tt.hpp"

void TranspositionTable::add(const hashEntry &entry) {
  tt[entry.zobrist % TT_SIZE] = entry;
}
hashEntry TranspositionTable::get(const hashEntry &entry) const {
  return tt[entry.zobrist % TT_SIZE];
}
Move TranspositionTable::getMove(uint64_t zobristKey) const {
  return tt[zobristKey % TT_SIZE].move;
}
void TranspositionTable::replaceByDepth(const hashEntry &entry,
                                        bool globalAncientFlag) {
  if ((get(entry).depth < entry.depth) &&
      (globalAncientFlag != get(entry).ancient)) {
    add(entry);
  }
}
void TranspositionTable::clear() {

  for (hashEntry &entry : tt) {
    entry = hashEntry{0ull, 0, 0, nodeType::EXACT, true, Move{a1, a1, false}};
  }
}