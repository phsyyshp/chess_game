#include "bitoperations.hpp"
#include "move_generation.hpp"
#include "position.hpp"
class Evaluation {
public:
  // setters;
  Evaluation() = default;
  Evaluation(const Position &position_) : position(position_) {}
  // getters;
  Position getPosition() const;
  int getPieceCount(const piece &, const color &) const;
  size_t getMobility(const color &color_) const;
  // pawn operations;
  int getDoubledPawnCount(const color &) const;
  // int getBlockedPawnCount(const color &) const;
  int getIsolatedPawnCount(const color &) const;
  int pawnSquareTables(const color &color_) const;
  // other piece operations
  int knightSquareTables(const color &color_) const;
  int queenSquareTables(const color &color_) const;
  int rookSquareTables(const color &color_) const;
  int bishopSquareTables(const color &color_) const;
  int kingSquareTables(const color &color_) const;
  int pieceSquareTables() const;
  int evaluate() const;

private:
  Position position;
};