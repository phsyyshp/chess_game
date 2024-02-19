#pragma once
#include "Move.hpp"
#include "constants.hpp"
struct hashEntry {
  hashEntry()
      : zobrist(0), depth(0), score(0), flag(nodeType::EXACT), ancient(true),
        move(a1, a1, false){};

  hashEntry(uint64_t z, int depth_, int score_, nodeType flag_, bool ancient_,
            const Move &move_)
      : zobrist(z), depth(depth_), score(score_), flag(flag_),
        ancient(ancient_), move(move_){};
  uint64_t zobrist;
  int depth;
  int score;
  nodeType flag;
  bool ancient;
  Move move;
};
class TranspositionTable {
public:
  TranspositionTable() { clear(); }
  void add(const hashEntry &entry);
  void replaceByDepth(const hashEntry &entry, bool globalAncientFlag);
  hashEntry get(const hashEntry &entry) const;
  Move getMove(uint64_t zobristKey) const;

  void clear();

private:
  std::array<hashEntry, 100> tt;
};