#ifndef POSITION_HPP
#define POSITION_HPP
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
  Position(std::array<std::array<uint64_t, 6>, 2> pieces_, color turn_);
  // operators:
  Position &operator=(const Position &);
  // setters;
  void setBoardToInitialConfiguration();
  void changeTurn();
  // getters:
  std::array<std::array<uint64_t, 6>, 2> getPieces() const;
  std::array<piece, 64> getMailbox() const;
  color getTurn() const;
  color getOppositeTurn() const;
  color getPieceColor(const uint64_t &sqMask) const;
  piece getPieceType(const uint64_t &sqMask) const;
  piece getPiece(int square);

  uint64_t getAllPieces(const color &color_) const;
  uint64_t getAttacksToKing() const;
  piece getCapturedInLastMove() const;
  // bools:
  bool isInCheck() const;
  bool isEmpty(int square_) const;
  // movers:
  // assuming the move is pseudolegal,
  // returns true if the is legal.
  bool makeMove(Move move);
  void undoMove(Move move);
  // visualizers;
  void printBoard() const;

private:
  void setWhitePiecesToInitialConfiguration();
  void setBlackPiecesToInitialConfiguration();
  color turn;
  std::array<std::array<uint64_t, 6>, 2> pieces;
  std::array<piece, 64> mailbox;

  piece capturedInLastMove;

public:
  canCastle canWhiteCastle;
  canCastle canBlackCastle;
};
#endif