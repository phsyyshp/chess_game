#pragma once
#include "evaluation.hpp"
#include <chrono>
class Search {
public:
  // constructors;
  Search() = default;
  Search(const Position &p);
  Search(const Position &p, int timeLeftWhite_, int timeIncrementWhite_,
         int timeLeftBlack_, int timeIncrementBlack_);

  // getters;
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES>
  getKillerMoves() const;
  int getPly() const;

  // searchers;
  int negaMax(int depth);
  Move search(int depth);
  Move searchIt(int maxDepth);
  Move searchAB(int depth, auto start, int remainingTime, int timeIncrement);
  int quiesce(int alpha, int beta);
  int alphaBeta(int alpha, int beta, int depthLeft);
  // move ordering;
  void scoreMoves(MoveList &moveList_) const;
  void pickMove(MoveList &moveList_, int startingIdx) const;
  void orderMoves(MoveList &movelist_);
  void storeKillerMove(const Move &move_, int ply);

private:
  Position position;
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES> killerMoves;
  int ply = 0;
  int timeLeftWhite;
  int timeIncrementWhite;
  int timeLeftBlack;
  int timeIncrementBlack;
  Move pv;
};
