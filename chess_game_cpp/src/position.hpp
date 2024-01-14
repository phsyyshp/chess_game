#include "Move.hpp"
#include "loader.hpp"
#include "tables.hpp"
class Position {
public:
  struct canCastle {
    bool kingSide;
    bool queenSide;
  };
  // constructors:
  Position() : pieces(2, std::vector<uint64_t>(6, 0)) {}
  Position(std::vector<std::vector<uint64_t>> piecesInput, color turnInput)
      : pieces(piecesInput), turn(turnInput) {}
  // setters;
  void setBoardToInitialConfiguration();
  void changeTurn();
  // getters:
  std::vector<std::vector<uint64_t>> getPieces() const;
  color getTurn() const;
  color getPieceColor(const uint64_t &position) const;
  piece getPieceType(const uint64_t &position) const;
  uint64_t getAllPieces(const color &pieceColor) const;

  // std::vector<std::vector<uint64_t>> makeSoftMove(const int &source,
  //                                                 const int &destination)
  //                                                 const;
  void makeMove(Move move);
  void undoMove(Move move);
  void printBoard() const;

private:
  void setWhitePiecesToInitialConfiguration();
  void setBlackPiecesToInitialConfiguration();
  color turn;
  std::vector<std::vector<uint64_t>> pieces;

  piece capturedInLastMove;
  canCastle canWhiteCastle;
  canCastle canBlackCastle;
};
