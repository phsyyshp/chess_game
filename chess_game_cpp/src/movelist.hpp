#include "Move.hpp"
#include "constants.hpp"
class MoveList {
public:
  MoveList() : lastSetIdx(-1) {}

  void pushBack(Move move);

private:
  int lastSetIdx;

  std::array<Move, 256> elements;
};