#include "bitoperations.hpp"
#include "position.hpp"

class Evaluation {
public:
  // setters;
  Evaluation() = default;
  Evaluation(Position pos) : position(pos) {}
  // getters;
  Position getPosition() const;
  int getPieceCount(const piece &, const piece &) const;
  int getDoubledPawnCount(const color &) const;
  int getBlockedPawnCount(const color &) const;
  int getIsolatedPawnCount(const color &) const;
  int getMobility(const color &) const;
  // bools;
  bool isPawnDoubled(square sq) const;
  bool isPawnIsolated(square, const color &) const;
  bool isPawnBlocked(square, const color &) const;

private:
  Position position;
};