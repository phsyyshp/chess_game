#pragma once
#include "Move.hpp"
#include "constants.hpp"
struct hashEntry {
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
  TranspositionTable();
  void add(const hashEntry &entry);
  void replaceByDepth(const hashEntry &entry, bool globalAncientFlag);
  hashEntry get(const hashEntry &entry);
  Move getMove(uint64_t zobristKey) const;

  void clear();

private:
  static std::array<hashEntry, 100> tt;
};