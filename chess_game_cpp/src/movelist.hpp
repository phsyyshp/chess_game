#include "Move.hpp"
#include "constants.hpp"
class MoveList {
public:
  MoveList() : lastSetIdx(-1) {}

  void pushBack(Move move);
  Move operator[](int n) { return elements[n]; }

private:
  int lastSetIdx;

  std::array<Move, 256> elements;
};