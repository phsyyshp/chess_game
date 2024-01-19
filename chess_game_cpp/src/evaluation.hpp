#include "bitoperations.hpp"
#include "move_generation.hpp"
#include "position.hpp"
class Evaluation {
public:
  // setters;
  Evaluation() = default;
  Evaluation(Position pos) : position(pos) {}
  // getters;
  Position getPosition() const;
  int getPieceCount(const piece &, const color &) const;
  // pawn operations;
  int getDoubledPawnCount(const color &) const;
  int getBlockedPawnCount(const color &) const;
  int getIsolatedPawnCount(const color &) const;

  size_t getMobility(const color &) const;
  float evaluate() const;

private:
  Position position;
};