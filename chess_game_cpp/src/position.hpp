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
  Position(std::array<std::array<uint64_t, 6>, 2> pieces_, color turn_)
      : pieces(pieces_), turn(turn_) {}
  // operators:
  Position &operator=(const Position &);
  // setters;
  void setBoardToInitialConfiguration();
  void changeTurn();
  // getters:
  std::array<std::array<uint64_t, 6>, 2> getPieces() const;
  color getTurn() const;
  color getOppositeTurn() const;
  color getPieceColor(const uint64_t &sqMask) const;
  piece getPieceType(const uint64_t &sqMask) const;
  uint64_t getAllPieces(const color &color_) const;
  uint64_t getAttacksToKing() const;
  // bools:
  bool isInCheck() const;
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
  std::array<piece, 64> mailbox;
};
#endif