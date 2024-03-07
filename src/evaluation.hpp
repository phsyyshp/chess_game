#include "bitoperations.hpp"
#include "moveGeneration.hpp"
#include "position.hpp"
class Evaluation {
public:
  // setters;
  Evaluation() = default;
  Evaluation(const Position &position_) : position(position_) {}
  // getters;
  Position getPosition() const;
  int getPieceCount(const Piece &, const Color &) const;
  size_t getMobility(const Color &color_) const;
  // pawn operations;
  int getDoubledPawnCount(const Color &) const;
  // int getBlockedPawnCount(const color &) const;
  int getIsolatedPawnCount(const Color &) const;
  int pawnSquareTables(const Color &color_) const;
  // other piece operations
  int knightSquareTables(const Color &color_) const;
  int queenSquareTables(const Color &color_) const;
  int rookSquareTables(const Color &color_) const;
  int bishopSquareTables(const Color &color_) const;
  int kingSquareTables(const Color &color_) const;
  int pieceSquareTables() const;
  int16_t evaluate() const;

private:
  Position position;
};