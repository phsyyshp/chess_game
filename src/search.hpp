#pragma once
#include "evaluation.hpp"
#include "tt.hpp"
#include <chrono>
class Search {
public:
  // constructors;
  Search();
  // getters;
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES>
  getKillerMoves() const;
  int getPly() const;
  void clear();
  // searchers;
  Move getBestMove(const Position &position, int maxDepth, int wtime, int winc,
                   int btime, int binc, bool isInfoOn);
  Move searchIt(const Position &position);
  int negaMax(int depth, const Position &position);
  Move search(int depth, const Position &position);
  Move searchAB(int depth, std::chrono::high_resolution_clock::time_point start,
                int remainingTime, int timeIncrement, const Position &position);
  int quiesce(int alpha, int beta, const Position &position);
  int alphaBeta(int alpha, int beta, int depthLeft, const Position &position);
  // move ordering;
  void scoreMoves(MoveList &moveList_, const Position &position) const;
  void pickMove(MoveList &moveList_, int startingIdx) const;
  void orderMoves(MoveList &movelist_);
  void storeKillerMove(const Move &move_, int ply);
  void flipGlobalAncientFlag();
  // timing
  int countTime(std::chrono::high_resolution_clock::time_point);
  int getMaxMoveDuration(color turn, int wtime, int winc, int btime,
                         int binc) const;

private:
  // std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES> killerMoves;
  int ply = 0;
  std::chrono::high_resolution_clock::time_point start;
  int maxMoveDuration;
  int maxDepth;
  bool isInfoOn;
  uint64_t nodes;
  Move pv;
  bool isTimeExeeded;
  // TranspositionTable tt;
  // bool globalAncientFlag = false;
};
