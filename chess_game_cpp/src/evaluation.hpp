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
  size_t getMobility(const color &pieceColor) const;

  // pawn operations;
  int getDoubledPawnCount(const color &) const;
  int getBlockedPawnCount(const color &) const;
  int getIsolatedPawnCount(const color &) const;

  int pawnSquareTables(const color &pieceColor) const;
  // other piece operations

  int knightSquareTables(const color &pieceColor) const;

  int queenSquareTables(const color &pieceColor) const;

  int rookSquareTables(const color &pieceColor) const;

  int bishopSquareTables(const color &pieceColor) const;

  float pieceSquareTables() const;

  float evaluate() const;

private:
  Position position;
};