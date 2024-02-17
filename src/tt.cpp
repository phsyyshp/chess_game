#include "tt.hpp"
void TranspositionTable::add(const hashEntry &entry) {
  tt[entry.zobrist % 100] = entry;
}
hashEntry TranspositionTable::get(const hashEntry &entry) {
  return tt[entry.zobrist % 100];
}
void TranspositionTable::replaceByDepth(const hashEntry &entry) {
  if (get(entry).depth < entry.depth) {
    add(entry);
  }
}