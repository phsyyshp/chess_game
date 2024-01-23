#include "evaluation.hpp"

class Search {
public:
  Search() = default;
  Search(const Position &p) : position(p){};

  int negaMax(int depth);
  Move search(int depth);

private:
  Position position;
};