#include "position.hpp"
class MoveValidator {
public:
  MoveValidator() = default;
  MoveValidator(const Position &position_) : position(position_){};
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
  Position position;
};