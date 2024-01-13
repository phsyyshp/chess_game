#include "moveValidator.hpp"
// Move validation
bool MoveValidator::isSquareEmpty(const uint64_t &positionMask) const {
  uint64_t allPieces =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  return ~(allPieces & positionMask);
}
// Checks wheter there is a piece on destination with same color of the piece at
// source square.
// Assuming, the piece in source square have same color with turn.
// Assuming, valid move i.e. not trying to move empty square, not out of turn
// move.
bool MoveValidator::isDestinationOccupiedBySameColor(
    const uint64_t &sourceMask, const uint64_t &destinationMask) const {
  return position.getPieces()[position.getTurn()][all] & destinationMask;
}
bool MoveValidator::isSlidingMove(const piece &pieceType) const {
  return pieceType == piece::bishop || pieceType == piece::queen ||
         pieceType == piece::rook;
}
bool MoveValidator::isSlidingMoveLegal(const uint64_t &sourceMask,
                                       const uint64_t &destinationMask,
                                       const uint64_t &allPieces,
                                       const piece &pieceType) const {
  square from = static_cast<square>(__builtin_ctzll(sourceMask));
  switch (pieceType) {
  case piece::bishop:
    return destinationMask & getBishopAttackMask(from, allPieces);
    break;
  case piece::rook:
    return destinationMask & getRookAttackMask(from, allPieces);
    break;
  case piece::queen:
    return destinationMask & (getRookAttackMask(from, allPieces) |
                              getBishopAttackMask(from, allPieces));

    break;
  default:
    std::cerr << "it's not a sliding move";
    return false;

    break;
  }
}
bool MoveValidator::isKnightMoveLegal(const int &source,
                                      const uint64_t &destinationMask) const {

  return destinationMask & knightLookUpTable[source];
};
// bool MoveValidator::isEnPassant(const uint64_t &sourceMask,
//                              const uint64_t &destinationMask) const {
// };
bool MoveValidator::isLegalPawnCapture(const uint64_t &sourceMask,
                                       const uint64_t &destinationMask) const {
  bool oppositeTurn = (position.getTurn() + 1) % 2;
  return position.getPieces()[oppositeTurn][all] &
         pawnLookUpTable[position.getTurn()][__builtin_ctzll(sourceMask)];
}
bool MoveValidator::isSinglePawnMoveLegal(
    const uint64_t &sourceMask, const uint64_t &destinationMask) const {
  bool isLegalPush;
  uint64_t legalCaptures; // TODO: there may be potential bugs regarding out of
                          // bound;
  switch (position.getTurn()) {
  case color::white:
    isLegalPush = (sourceMask == (destinationMask >> 8));
    return isLegalPush && isSquareEmpty(destinationMask);
    break;
  case color::black:
    isLegalPush = (sourceMask == (destinationMask << 8));
    return isLegalPush && isSquareEmpty(destinationMask);
    break;

  default:
    break;
  }
};
bool MoveValidator::isDoublePawnMoveLegal(
    const uint64_t &sourceMask, const uint64_t &destinationMask) const {
  // TODO: there may be potential bugs regarding out of bound
  bool isLegalDoublePush;
  if (position.getTurn() == color::white) {
    isLegalDoublePush = isSquareEmpty(destinationMask << 8) &&
                        (sourceMask == (destinationMask >> 16)) &&
                        (sourceMask / 8 == 1);
  } else if (position.getTurn() == color::black) {
    isLegalDoublePush = (sourceMask == (destinationMask << 16)) &&
                        (sourceMask / 8 == 6) &&
                        isSquareEmpty(destinationMask >> 8);
  }
  return isLegalDoublePush && isSquareEmpty(destinationMask);
};
bool MoveValidator::isPawnMoveLegal(const uint64_t &sourceMask,
                                    const uint64_t &destinationMask) const {
  // if (isEnPassant(sourceMask, destinationMask)) {
  //   return isEnPassantLegal(sourceMask, destinationMask, allPieces);
  // }
  return isSinglePawnMoveLegal(sourceMask, destinationMask) ||
         isDoublePawnMoveLegal(sourceMask, destinationMask) ||
         isLegalPawnCapture(sourceMask, destinationMask);
}
bool MoveValidator::isKingMoveLegal(const int &source,
                                    const uint64_t &destinationMask) const {
  return destinationMask & kingLookUpTable[source];
};
bool MoveValidator::isAttackedBySlider(const uint64_t &sourceMask) const {

  int oppositeTurn = (position.getTurn() + 1) % 2;
  auto source = static_cast<square>(__builtin_ctzll(sourceMask));
  uint64_t allPieces =
      position.getPieces()[black][all] | position.getPieces()[white][all];
  uint64_t bishopAttackMask = getBishopAttackMask(source, allPieces);
  uint64_t rookAttackMask = getRookAttackMask(source, allPieces);

  return (position.getPieces()[oppositeTurn][bishop] & bishopAttackMask) |
         (position.getPieces()[oppositeTurn][rook] & rookAttackMask) |
         (position.getPieces()[oppositeTurn][queen] &
          (bishopAttackMask | rookAttackMask));
}
bool MoveValidator::isAttackedByPawn(const int &source) const {
  int oppositeTurn = (position.getTurn() + 1) % 2;
  return position.getPieces()[oppositeTurn][pawn] &
         pawnLookUpTable[position.getTurn()][source];
}
bool MoveValidator::isAttackedByKnight(const int &source) const {
  int oppositeTurn = (position.getTurn() + 1) % 2;
  return position.getPieces()[oppositeTurn][knight] & knightLookUpTable[source];
}
bool MoveValidator::isAttackedByKing(const int &source) const {
  int oppositeTurn = (position.getTurn() + 1) % 2;
  return position.getPieces()[oppositeTurn][king] & kingLookUpTable[source];
}
bool MoveValidator::isCheck() const {
  uint64_t kingPositionMask = position.getPieces()[position.getTurn()][king];
  uint64_t kingPosition = __builtin_ctzll(kingPositionMask);
  return isAttackedBySlider(kingPositionMask) ||
         isAttackedByPawn(kingPosition) || isAttackedByKing(kingPosition) ||
         isAttackedByKnight(kingPosition);
};
bool MoveValidator::isPseudoLegalMove(const int &source,
                                      const int &destination) const {
  uint64_t sourceMask = 0b1uLL << source;
  if (isSquareEmpty(sourceMask)) {
    return false;
  };
  uint64_t destinationMask = 0b1uLL << destination;
  piece pieceType = position.getPieceType(sourceMask);
  color pieceColor = position.getPieceColor(sourceMask);
  uint64_t allPieces =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  if (position.getTurn() != pieceColor) {
    return false;
  };
  if (pieceType == pawn) {
    return isPawnMoveLegal(sourceMask, destinationMask);
  }
  if (isDestinationOccupiedBySameColor(source, destination)) {
    return false;
  };
  if (isSlidingMove(pieceType)) {
    return isSlidingMoveLegal(sourceMask, destinationMask, allPieces,
                              pieceType);
  };
  if (pieceType == knight) {
    return isKnightMoveLegal(sourceMask, destinationMask);
  };
  if (pieceType == king) {
    return isKingMoveLegal(source, destinationMask);
  }
}
bool MoveValidator::isLegalMove(const int &source,
                                const int &destination) const {
  std::vector<std::vector<uint64_t>> piecesTemp =
      position.makeSoftMove(source, destination);
  Position tempPosition = Position(piecesTemp, position.getTurn());
  // FIX IT: first fix then uncoment the following section.
  //    if (tempPosition.isCheck()) {
  //      return false;
  //    }
  return isPseudoLegalMove(source, destination);
}
