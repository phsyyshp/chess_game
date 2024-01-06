#include "loader.hpp"
#include <cstdint>
#include <iostream>
#include <string>

class Position {
public:
  struct canCastle {
    bool kingSide;
    bool queenSide;
  };

  Position() = default;

  void setBoardToInitialConfiguration();
  std::vector<std::vector<uint64_t>> getPieces() const;
  color getPieceColor(const uint64_t &position) const;
  piece getPieceType(const uint64_t &position) const;

  void changeTurn();
  void printBoard() const;

  bool isSquareEmpty(const uint64_t &squareMask) const;
  bool isDestinationOccupiedBySameColor(const uint64_t &sourceMask,
                                        const uint64_t &destinationMask) const;
  bool isSlidingMove(const piece &pieceType) const;

  bool isKnightMoveLegal(const int &source,
                         const uint64_t &destinationMask) const;
  bool isLegalPawnCapture(const uint64_t &sourceMask,
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
                          const piece &pieceType) const;
  bool isKingMoveLegal(const int &source,
                       const uint64_t &destinationMask) const;
  bool isAttackedBySlider(const uint64_t &sourceMask) const;
  bool isAttackedByPawn(const int &source) const;
  bool isAttackedByKing(const int &source) const;
  bool isAttackedByKnight(const int &source) const;
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
  static const std::vector<std::vector<uint64_t>> pawnLookUpTable;
  static const std::vector<uint64_t> kingLookUpTable;
  color turn;
  std::vector<std::vector<uint64_t>> pieces{2, std::vector<uint64_t>{13, 0}};
  canCastle canWhiteCastle;
  canCastle canBlackCastle;
};
