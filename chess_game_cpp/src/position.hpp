#include "loader.hpp"

#include "tables.hpp"
class Position {
public:
  struct canCastle {
    bool kingSide;
    bool queenSide;
  };

  // constructors:
  Position() = default;
  Position(std::vector<std::vector<uint64_t>> piecesInput, color turnInput)
      : pieces(piecesInput), turn(turnInput) {}
  // setters;
  void setBoardToInitialConfiguration();
  void changeTurn();
  // getters:
  std::vector<std::vector<uint64_t>> getPieces() const;
  color getPieceColor(const uint64_t &position) const;
  piece getPieceType(const uint64_t &position) const;
  std::vector<std::vector<uint64_t>> makeSoftMove(const int &source,
                                                  const int &destination) const;
  uint64_t getRookAttackMask(const square &sq, const uint64_t &occupancy) const;
  uint64_t getBishopAttackMask(const square &sq,
                               const uint64_t &occupancy) const;

  color getTurn() const;
  void printBoard() const;
  // validators:
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
  bool isLegalMove(const int &source, const int &destination) const;

private:
  void setWhitePiecesToInitialConfiguration();
  void setBlackPiecesToInitialConfiguration();
  color turn;
  std::vector<std::vector<uint64_t>> pieces{2, std::vector<uint64_t>{13, 0}};
  canCastle canWhiteCastle;
  canCastle canBlackCastle;
};
