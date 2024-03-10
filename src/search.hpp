#pragma once
#include "evaluation.hpp"
#include "tt.hpp"
#include <algorithm>
#include <chrono>
class Search {
public:
  // constructors;
  Search(std::array<uint64_t, 100> &pH);
  // setter;
  void setPositionHistory(std::array<uint64_t, 100> &positionHistory_);
  // getters;
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES>
  getKillerMoves() const;
  int getPly() const;
  void clear();
  void initSearch();
  void clearKillerMoves();
  // searchers;
  Move getBestMove(const Position &position, int maxDepth, int wtime, int winc,
                   int btime, int binc, bool isInfoOn);
  void iterativeDeepening(const Position &position);
  Move searchRoot(int depth, const Position &position);
  int16_t quiesce(int16_t alpha, int16_t beta, const Position &position);
  int16_t search(int16_t alpha, int16_t beta, int depthLeft,
                 const Position &position, bool isRoot);
  // move ordering;
  void scoreMoves(MoveList &moveList_, const Position &position);
  void pickMove(MoveList &moveList_, int startingIdx) const;
  void orderMoves(MoveList &movelist_);
  void storeKillerMove(const Move &move_, int ply);
  void flipGlobalAncientFlag();
  // timing
  int countTime(std::chrono::high_resolution_clock::time_point);
  int getMaxMoveDuration(Color turn, int wtime, int winc, int btime,
                         int binc) const;

private:
  int ply = 0;
  int gamePly = 0;
  std::chrono::high_resolution_clock::time_point start;
  int maxMoveDuration;
  int maxDepth;
  bool isInfoOn;
  uint64_t nodes;
  bool isTimeExeeded;
  int hits = 0;
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES> killerMoves;
  TranspositionTable tt;
  Move bestMove;
  int16_t pvScore;
  std::array<uint64_t, 100> &positionHistory;
  std::array<std::array<int, 64>, 64> LMRtable;
  std::array<std::array<int, 11>, 2> LateMovePruningCounts;
};
