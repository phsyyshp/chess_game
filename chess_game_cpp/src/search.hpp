#include "evaluation.hpp"
class Search {
public:
  Search() = default;
  Search(const Position &p) : position(p){};

  int negaMax(int depth);
  Move search(int depth);
  int quiesce(int alpha, int beta);

  void scoreMoves(MoveList &moveList_);

  void orderMoves(MoveList &movelist_);
  int alphaBeta(int alpha, int beta, int depthLeft);

private:
  Position position;
};