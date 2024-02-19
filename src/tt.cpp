#include "tt.hpp"

TranspositionTable::TranspositionTable() {
  for (hashEntry entry : tt) {
    entry = hashEntry{0ull, 0, 0, nodeType::EXACT, true, Move{a1, a1, false}};
  }
}
void TranspositionTable::add(const hashEntry &entry) {
  tt[entry.zobrist % 100] = entry;
}
hashEntry TranspositionTable::get(const hashEntry &entry) {
  return tt[entry.zobrist % 100];
}
Move TranspositionTable::getMove(uint64_t zobristKey) const {
  return tt[zobristKey % 100].move;
}
void TranspositionTable::replaceByDepth(const hashEntry &entry,
                                        bool globalAncientFlag) {
  if ((get(entry).depth < entry.depth) &&
      (globalAncientFlag != get(entry).ancient)) {
    add(entry);
  }
}
void TranspositionTable::clear() {

  for (hashEntry entry : tt) {
    entry = hashEntry{0ull, 0, 0, nodeType::EXACT, true, Move{a1, a1, false}};
  }
}