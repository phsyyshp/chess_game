#include "Move.hpp"
#include "loader.hpp"
#include "tables.hpp"
#include "utilities.hpp"
class Position {
public:
  struct canCastle {
    bool kingSide;
    bool queenSide;
  };
  // constructors:
  Position() = default;
  Position(std::array<std::array<uint64_t, 6>, 2> piecesInput, color turnInput)
      : pieces(piecesInput), turn(turnInput) {}
  // setters;
  void setBoardToInitialConfiguration();
  void changeTurn();
  // getters:
  std::array<std::array<uint64_t, 6>, 2> getPieces() const;
  color getTurn() const;
  color getPieceColor(const uint64_t &position) const;
  piece getPieceType(const uint64_t &position) const;
  uint64_t getAllPieces(const color &pieceColor) const;
  // movers:
  void makeMove(Move move);
  void undoMove(Move move);
  // visualizers;
  void printBoard() const;

private:
  void setWhitePiecesToInitialConfiguration();
  void setBlackPiecesToInitialConfiguration();
  color turn;
  std::array<std::array<uint64_t, 6>, 2> pieces;

public:
  piece capturedInLastMove;
  canCastle canWhiteCastle;
  canCastle canBlackCastle;
};
