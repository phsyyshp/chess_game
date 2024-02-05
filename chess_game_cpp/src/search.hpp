#include "evaluation.hpp"
class Search {
public:
  // constructors;
  Search() = default;
  Search(const Position &p) : position(p){};
  // getters;
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES>
  getKillerMoves() const;
  int getPly() const;
  // searchers;
  int negaMax(int depth);
  Move search(int depth);
  Move searchAB(int depth);
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
};