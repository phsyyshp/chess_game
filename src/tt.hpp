#pragma once
#include "constants.hpp"
#include "move.hpp"
struct hashEntry {
  hashEntry()
      : zobristKey(0), depth(0), score(0), flag(nodeType::EXACT),
        move(a1, a1, false){};

  hashEntry(uint64_t z, int depth_, int score_, nodeType flag_,
            const Move &move_)
      : zobristKey(z), depth(depth_), score(score_), flag(flag_), move(move_){};
  bool operator==(const hashEntry &other) const {
    return zobristKey == other.zobristKey && depth == other.depth &&
           score == other.score && flag == other.flag &&
           move.getMoveInt() == other.move.getMoveInt();
  }

  uint64_t zobristKey;
  int8_t depth;
  int16_t score;
  nodeType flag;
  Move move;
};
const hashEntry nullEntry;
class TranspositionTable {
public:
  TranspositionTable() {
    tt.resize(TT_SIZE);
    clear();
  }
  void add(const hashEntry &entry);
  hashEntry get(uint64_t zobristKey) const;
  Move getMove(uint64_t zobristKey) const;
  void clear();
  int fullness();

private:
  std::vector<hashEntry> tt;
  int numel = 0;
};