#ifndef POSITION_HPP
#define POSITION_HPP
#include "Move.hpp"
#include "gameState.hpp"
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
  Position(const std::array<std::array<uint64_t, 6>, 2> &pieces_, color turn_);
  Position(const std::string &FENstr);

  // operators:
  Position &operator=(const Position &);
  // setters;
  void setBoardToInitialConfiguration();
  void changeTurn();
  void clear();
  // getters:
  std::array<std::array<uint64_t, 6>, 2> getPieces() const;
  std::array<piece, 64> getMailbox() const;
  color getTurn() const;
  int getPly() const;
  GameState getGameState() const;
  color getOppositeTurn() const;
  color getPieceColor(const uint64_t &sqMask) const;
  piece getPieceType(const uint64_t &sqMask) const;
  piece getPiece(int square) const;

  uint64_t getAllPieces(const color &color_) const;
  uint64_t getAttacksToKing() const;
  piece getCapturedInLastMove() const;
  uint64_t getAttacksToSquare(square square_) const;

  // bools:
  bool isInCheck() const;
  // TODO: refactor this
  bool isInCheck(square square_) const;
  bool isEmpty(int square_) const;
  // movers:
  // assuming the move is pseudolegal,
  // returns true if the is legal.
  bool makeMove(const Move &move);
  void undoMove(const Move &move);
  // visualizers;
  void printBoard() const;

private:
  // functions
  void setWhitePiecesToInitialConfiguration();
  void setBlackPiecesToInitialConfiguration();
  // moveMakers
  void makeQuietMove(const Move &move);
  void capture(const Move &move);
  void makeDoublePawnPush(const Move &move);
  void makeEPCapture(const Move &move);
  void makeQueenCastle(const Move &move);
  void makeKingCastle(const Move &move);

  void makeKnightPromotion(const Move &move);
  void makeBishopPromotion(const Move &move);
  void makeRookPromotion(const Move &move);
  void makeQueenPromotion(const Move &move);
  void makeKnightPromoCapture(const Move &move);
  void makeBishopPromoCapture(const Move &move);
  void makeRookPromoCapture(const Move &move);
  void makeQueenPromoCapture(const Move &move);

  void updateCastlingRights(int from, int movingPiece);
  // members
  std::array<std::array<uint64_t, 6>, 2> pieces;
  std::array<piece, 64> mailbox;
  piece capturedInLastMove;
  GameState gameState;
  uint ply;
};
#endif