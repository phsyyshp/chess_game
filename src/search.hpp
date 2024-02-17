#pragma once
#include "evaluation.hpp"
#include <chrono>
class Search {
public:
  // constructors;
  Search();
  Search(int timeLeftWhite_, int timeIncrementWhite_, int timeLeftBlack_,
         int timeIncrementBlack_);

  // getters;
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES>
  getKillerMoves() const;
  int getPly() const;

  // searchers;
  int negaMax(int depth, const Position &position);
  Move search(int depth, const Position &position);
  Move searchIt(int maxDepth, bool isInfoOn, const Position &position);
  Move searchAB(int depth, std::chrono::high_resolution_clock::time_point start,
                int remainingTime, int timeIncrement, const Position &position);
  int quiesce(int alpha, int beta, const Position &position);
  int alphaBeta(int alpha, int beta, int depthLeft, const Position &position);
  // move ordering;
  void scoreMoves(MoveList &moveList_, const Position &position) const;
  void pickMove(MoveList &moveList_, int startingIdx) const;
  void orderMoves(MoveList &movelist_);
  void storeKillerMove(const Move &move_, int ply);

  // timing
  int countTime(std::chrono::high_resolution_clock::time_point);

private:
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES> killerMoves;
  int ply = 0;
  int timeLeftWhite;
  int timeIncrementWhite;
  int timeLeftBlack;
  int timeIncrementBlack;
  Move pv;
};
