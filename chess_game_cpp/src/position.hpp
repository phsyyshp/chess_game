#include "loader.hpp"
#include <cstdint>
#include <iostream>
#include <string>

enum class color {
  white,
  black

};
enum square {
  a1,
  b1,
  c1,
  d1,
  e1,
  f1,
  g1,
  h1,
  a2,
  b2,
  c2,
  d2,
  e2,
  f2,
  g2,
  h2,
  a3,
  b3,
  c3,
  d3,
  e3,
  f3,
  g3,
  h3,
  a4,
  b4,
  c4,
  d4,
  e4,
  f4,
  g4,
  h4,
  a5,
  b5,
  c5,
  d5,
  e5,
  f5,
  g5,
  h5,
  a6,
  b6,
  c6,
  d6,
  e6,
  f6,
  g6,
  h6,
  a7,
  b7,
  c7,
  d7,
  e7,
  f7,
  g7,
  h7,
  a8,
  b8,
  c8,
  d8,
  e8,
  f8,
  g8,
  h8
};
class Position {
public:
  struct colored {
    uint64_t white;
    uint64_t black;
  };
  struct Pieces {
    colored queens;
    colored bishops;
    colored knights;
    colored rooks;
    colored pawns;
    colored king;
    colored all;
  };
  struct canCastle {
    bool kingSide;
    bool queenSide;
  };

  Position() = default;

  void setBoardToInitialConfiguration();
  Pieces getPieces() const;
  color getPieceColor(const uint64_t &position) const;
  std::string getPieceType(const uint64_t &position) const;

  void changeTurn();
  void printBoard() const;

  bool isSquareEmpty(const uint64_t &squareMask) const;
  bool isDestinationOccupiedBySameColor(const int &source,
                                        const int &destination) const;
  bool isSlidingMove(const std::string &piecetype) const;

  bool isKnightMoveLegal(const int &source,
                         const uint64_t &destinationMask) const;
  bool isSinglePawnMoveLegal(const uint64_t &sourceMask,
                             const uint64_t &destinationMask) const;
  bool isDoublePawnMoveLegal(const uint64_t &sourceMask,
                             const uint64_t &destinationMask) const;
  bool isPawnMoveLegal(const uint64_t &sourceMask,
                       const uint64_t &destinationMask) const;
  bool isPseudoLegalMove(const int &source, const int &destination) const;

  bool isSlidingMoveLegal(const uint64_t &sourceMask,
                          const uint64_t &destinationMask,
                          const uint64_t &allPieces,
                          const std::string &pieceType) const;
  bool isKingMoveLegal(const int &source,
                       const uint64_t &destinationMask) const;
  bool isAttackedBySlider(const uint64_t &sourceMask) const;
  bool isAttackedByPawn(const int &source) const;
  bool isAttackedByKing(const int &source) const;
  bool isAttackedByKnight(const int &source) const;
  // Pieces getBlackPieces() const;
  bool isCheck() const;

private:
  void setWhitePiecesToInitialConfiguration();
  void setBlackPiecesToInitialConfiguration();
  // slider cache
  static const std::vector<uint64_t> rookMagicNumbers;
  static const std::vector<std::vector<uint64_t>> rookLookUpTables;
  static const std::vector<uint64_t> bishopMagicNumbers;
  static const std::vector<std::vector<uint64_t>> bishopLookUpTables;
  // rest cache
  static const std::vector<uint64_t> knightLookUpTable;
  static const std::vector<uint64_t> whitePawnLookUpTable;
  static const std::vector<uint64_t> blackPawnLookUpTable;
  static const std::vector<uint64_t> kingLookUpTable;
  color turn;
  Pieces pieces;
  canCastle canWhiteCastle;
  canCastle canBlackCastle;
};
