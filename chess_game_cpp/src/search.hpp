#include "evaluation.hpp"

class Search {
public:
  Search() = default;
  Search(const Position &p) : position(p){};

  int negaMax(int depth);
  Move search(int depth);
  int quiesce(int alpha, int beta);

private:
  Position position;
};