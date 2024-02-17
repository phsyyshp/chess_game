#pragma once
#include "Move.hpp"
#include "constants.hpp"
struct hashEntry {
  uint64_t zobrist;
  int depth;
  int score;
  int flag;
  int ancient;
  Move move;
};
class TranspositionTable {
  static std::array<hashEntry, 100> tt;

public:
  TranspositionTable() = default;
  void add(const hashEntry &entry);
  void replaceByDepth(const hashEntry &entry);
  hashEntry get(const hashEntry &entry);
};