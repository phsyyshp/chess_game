#include "evaluation.hpp"

class Search {
public:
  Search() = default;
  Search(const Position &p) : position(p){};

  int negaMax(int depth);
  Move getBestMove() const;

private:
  Position position;
  Move bestMove;
};