#pragma once
#include "Move.hpp"
#include "constants.hpp"
struct hashEntry {
  hashEntry(uint64_t z, int depth_, int score_, nodeType flag_, int ancient_,
            const Move &move_)
      : zobrist(z), depth(depth_), score(score_), flag(flag_),
        ancient(ancient_), move(move_){};
  uint64_t zobrist;
  int depth;
  int score;
  nodeType flag;
  int ancient;
  Move move;
};
class TranspositionTable {
public:
  TranspositionTable() = default;
  void add(const hashEntry &entry);
  void replaceByDepth(const hashEntry &entry);
  hashEntry get(const hashEntry &entry);

private:
  static std::array<hashEntry, 100> tt;
};