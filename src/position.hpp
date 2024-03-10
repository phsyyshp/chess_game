#pragma once
#include "gameState.hpp"
#include "loader.hpp"
#include "move.hpp"
#include "tables.hpp"
#include "utilities.hpp"
#include "zobrist.hpp"
class Position {
public:
  // constructors:
  Position() = default;
  Position(const std::array<std::array<uint64_t, 6>, 2> &pieces_, Color turn_);
  Position(const std::string &FENstr);

  // operators:
  Position &operator=(const Position &);
  // setters;
  void setBoardToInitialConfiguration();
  void changeTurn();
  void setPly(int ply_);
  void clear();
  // getters:
  const std::array<std::array<uint64_t, 6>, 2> &getPieces() const;
  const std::array<Piece, 64> &getMailbox() const;
  const Color &getTurn() const;
  uint64_t getZobrist() const;
  int getPly() const;
  const GameState &getGameState() const;
  Color getOppositeTurn() const;
  Color getPieceColor(const uint64_t &sqMask) const;
  Piece getPieceType(const uint64_t &sqMask) const;
  Piece getPiece(int square) const;
  const uint64_t &getOccupancy() const;
  const uint64_t &getAllPieces(const Color &color_) const;
  uint64_t getAttacksToKing() const;
  Piece getCapturedInLastMove() const;
  uint64_t getAttacksToSquare(Square square_) const;

  // bools:
  bool isInCheck() const;
  // TODO: refactor this
  bool isInCheck(Square square_) const;
  bool isEmpty(int square_) const;
  bool isThreeFoldRep(const std::array<uint64_t, 100> &positionHistory) const;

  // movers:
  // assuming the move is pseudolegal,
  // returns true if the is legal.
  bool makeMove(const Move &move, std::array<uint64_t, 100> &positionHistory);
  bool makeMove(const Move &move);

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

  void makePromotion(const Move &move, Piece piece_);
  void makePromoCapture(const Move &move, Piece piece_);

  void updateCastlingRights(int from, int movingPiece);
  // members
  std::array<std::array<uint64_t, 6>, 2> pieces;
  std::array<Piece, 64> mailbox;
  Piece capturedInLastMove;
  GameState gameState;
  uint64_t zobristHash;
  std::array<uint64_t, 2> occupanciesOfColor;
  uint64_t occupancy;
  Color turn;
  int ply = 0;
};