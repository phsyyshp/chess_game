#include "position.hpp"
// Initilazers;
const std::vector<uint64_t> Position::rookMagicNumbers =
    readMagicNumbersToVec(rook);
const std::vector<std::vector<uint64_t>> Position::rookLookUpTables =
    readLookUpTables(rook);
const std::vector<uint64_t> Position::bishopMagicNumbers =
    readMagicNumbersToVec(bishop);
const std::vector<std::vector<uint64_t>> Position::bishopLookUpTables =
    readLookUpTables(bishop);
const std::vector<uint64_t> Position::knightLookUpTable =
    readKnightLookUpTable();
const std::vector<std::vector<uint64_t>> Position::pawnLookUpTable = {
    readWhitePawnLookUpTable(), readBlackPawnLookUpTable()};
const std::vector<uint64_t> Position::kingLookUpTable = readKingLookUpTable();
// Setters
void Position::setWhitePiecesToInitialConfiguration() {

  pieces[white][rook] = 0b1ULL << a1 | 0b1ULL << h1;
  pieces[white][knight] = 0b1ULL << g1 | 0b1ULL << b1;
  pieces[white][bishop] = 0b1ULL << f1 | 0b1ULL << c1;
  pieces[white][queen] = 0b1ULL << e1;
  pieces[white][king] = 0b1ULL << d1;
  pieces[white][pawn] = 0b11111111ULL << 8;
  pieces[white][all] = pieces[white][rook] | pieces[white][knight] |
                       pieces[white][bishop] | pieces[white][queen] |
                       pieces[white][king] | pieces[white][pawn];
}
void Position::setBlackPiecesToInitialConfiguration() {
  pieces[black][rook] = 0b1ULL << a8 | 0b1ULL << h8;
  pieces[black][knight] = 0b1ULL << g8 | 0b1ULL << b8;
  pieces[black][bishop] = 0b1ULL << f8 | 0b1ULL << c8;
  pieces[black][queen] = 0b1ULL << e8;
  pieces[black][king] = 0b1ULL << d8;
  pieces[black][pawn] = 0b11111111ULL << 6;
  pieces[black][all] = pieces[black][rook] | pieces[black][knight] |
                       pieces[black][bishop] | pieces[black][queen] |
                       pieces[black][king] | pieces[black][pawn];
}
void Position::setBoardToInitialConfiguration() {
  setWhitePiecesToInitialConfiguration();
  setBlackPiecesToInitialConfiguration();
  turn = color::white;
  canWhiteCastle.kingSide = true;
  canWhiteCastle.queenSide = true;
  canBlackCastle.kingSide = true;
  canBlackCastle.queenSide = true;
}
void Position::changeTurn() {
  if (turn == color::white) {
    turn = color::black;
  } else {
    turn = color::white;
  }
}
// Asuming; non-special moves(!pro|!cast) and valid(des =empt|opColOc) input,
// use it for temprory changes.
std::vector<std::vector<uint64_t>>
Position::makeSoftMove(const int &source, const int &destination) const {
  uint64_t sourceMask = 0b1ull << source;
  uint64_t destinationMask = 0b1ull << destination;
  std::vector<std::vector<uint64_t>> piecesTemp = pieces;
  piece pieceType = getPieceType(sourceMask);
  color pieceColor = getPieceColor(sourceMask);
  int oppositePieceColor = (pieceColor + 1) % 2;

  piecesTemp[pieceColor][pieceType] &= ~sourceMask;
  piecesTemp[pieceColor][all] &= ~sourceMask;

  if (!isSquareEmpty(destinationMask)) {
    piece pieceType = getPieceType(destinationMask);
    piecesTemp[oppositePieceColor][pieceType] &= ~destinationMask;
    piecesTemp[oppositePieceColor][all] &= ~destinationMask;
  }

  piecesTemp[pieceColor][pieceType] &= destinationMask;
  piecesTemp[pieceColor][all] &= destinationMask;
  return piecesTemp;
}
// Getters
std::vector<std::vector<uint64_t>> Position::getPieces() const {
  return pieces;
}
color Position ::getPieceColor(const uint64_t &positionMask) const {

  if (positionMask & pieces[black][all]) {
    return color::black;
  } else if (positionMask & pieces[white][all]) {
    return color::white;
  } else {
    std::cerr << "empty square";
  }
}
piece Position::getPieceType(const uint64_t &positionMask) const {
  for (int pieceInd = 0; pieceInd < 12; pieceInd++) {

    if (positionMask & (pieces[white][pieceInd] | pieces[black][pieceInd])) {

      return static_cast<piece>(pieceInd);
    }
  }
}
color Position::getTurn() const { return turn; }
// Misc
void Position::printBoard() const {
  uint64_t allPieces = pieces[white][all] | pieces[black][all];
  for (int i = 63; i >= 0; i--) {
    if (i % 8 == 7) {
      std::cout << "\n";
    } else {
      std::cout << ((allPieces >> i) & 1);
    }
  }
  std::cout << "\n";
}
// Move validation
bool Position::isSquareEmpty(const uint64_t &positionMask) const {
  uint64_t allPieces = pieces[white][all] | pieces[black][all];
  return ~(allPieces & positionMask);
}
// Checks wheter there is a piece on destination with same color of the piece at
// source square.
// Assuming, the piece in source square have same color with turn.
// Assuming, valid move i.e. not trying to move empty square, not out of turn
// move.
bool Position::isDestinationOccupiedBySameColor(
    const uint64_t &sourceMask, const uint64_t &destinationMask) const {
  return pieces[turn][all] & destinationMask;
}
bool Position::isSlidingMove(const piece &pieceType) const {
  return pieceType == piece::bishop || pieceType == piece::queen ||
         pieceType == piece::rook;
}
bool Position::isSlidingMoveLegal(const uint64_t &sourceMask,
                                  const uint64_t &destinationMask,
                                  const uint64_t &allPieces,
                                  const piece &pieceType) const {
  switch (pieceType) {
  case piece::bishop:
    return destinationMask & getAttackMask(sourceMask, allPieces,
                                           bishopMagicNumbers,
                                           bishopLookUpTables, pieceType);
    break;
  case piece::rook:
    return destinationMask & getAttackMask(sourceMask, allPieces,
                                           rookMagicNumbers, rookLookUpTables,
                                           pieceType);
    break;
  case piece::queen:
    return destinationMask &
           (getAttackMask(sourceMask, allPieces, bishopMagicNumbers,
                          bishopLookUpTables, pieceType) |
            getAttackMask(sourceMask, allPieces, rookMagicNumbers,
                          rookLookUpTables, pieceType));

    break;
  default:
    std::cerr << "it's not a sliding move";
    return false;

    break;
  }
}
bool Position::isKnightMoveLegal(const int &source,
                                 const uint64_t &destinationMask) const {

  return destinationMask & knightLookUpTable[source];
};
// bool Position::isEnPassant(const uint64_t &sourceMask,
//                              const uint64_t &destinationMask) const {
// };
bool Position::isLegalPawnCapture(const uint64_t &sourceMask,
                                  const uint64_t &destinationMask) const {
  bool oppositeTurn = (turn + 1) % 2;
  return pieces[oppositeTurn][all] &
         pawnLookUpTable[turn][__builtin_ctzll(sourceMask)];
}
bool Position::isSinglePawnMoveLegal(const uint64_t &sourceMask,
                                     const uint64_t &destinationMask) const {
  bool isLegalPush;
  uint64_t legalCaptures; // TODO: there may be potential bugs regarding out of
                          // bound;
  switch (turn) {
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
bool Position::isDoublePawnMoveLegal(const uint64_t &sourceMask,
                                     const uint64_t &destinationMask) const {
  // TODO: there may be potential bugs regarding out of bound
  bool isLegalDoublePush;
  if (turn == color::white) {
    isLegalDoublePush = isSquareEmpty(destinationMask << 8) &&
                        (sourceMask == (destinationMask >> 16)) &&
                        (sourceMask / 8 == 1);
  } else if (turn == color::black) {
    isLegalDoublePush = (sourceMask == (destinationMask << 16)) &&
                        (sourceMask / 8 == 6) &&
                        isSquareEmpty(destinationMask >> 8);
  }
  return isLegalDoublePush && isSquareEmpty(destinationMask);
};
bool Position::isPawnMoveLegal(const uint64_t &sourceMask,
                               const uint64_t &destinationMask) const {
  // if (isEnPassant(sourceMask, destinationMask)) {
  //   return isEnPassantLegal(sourceMask, destinationMask, allPieces);
  // }
  return isSinglePawnMoveLegal(sourceMask, destinationMask) ||
         isDoublePawnMoveLegal(sourceMask, destinationMask) ||
         isLegalPawnCapture(sourceMask, destinationMask);
}
bool Position::isKingMoveLegal(const int &source,
                               const uint64_t &destinationMask) const {
  return destinationMask & kingLookUpTable[source];
};
bool Position::isAttackedBySlider(const uint64_t &sourceMask) const {

  int oppositeTurn = (turn + 1) % 2;
  uint64_t allPieces = pieces[black][all] | pieces[white][all];
  uint64_t bishopAttackMask = getAttackMask(
      sourceMask, allPieces, bishopMagicNumbers, bishopLookUpTables, bishop);
  uint64_t rookAttackMask = getAttackMask(
      sourceMask, allPieces, rookMagicNumbers, rookLookUpTables, bishop);

  return (pieces[oppositeTurn][bishop] & bishopAttackMask) |
         (pieces[oppositeTurn][rook] & rookAttackMask) |
         (pieces[oppositeTurn][queen] & (bishopAttackMask | rookAttackMask));
}
bool Position::isAttackedByPawn(const int &source) const {
  int oppositeTurn = (turn + 1) % 2;
  return pieces[oppositeTurn][pawn] & pawnLookUpTable[turn][source];
}
bool Position::isAttackedByKnight(const int &source) const {
  int oppositeTurn = (turn + 1) % 2;
  return pieces[oppositeTurn][knight] & knightLookUpTable[source];
}
bool Position::isAttackedByKing(const int &source) const {
  int oppositeTurn = (turn + 1) % 2;
  return pieces[oppositeTurn][king] & kingLookUpTable[source];
}
bool Position::isCheck() const {
  uint64_t kingPositionMask = pieces[turn][king];
  uint64_t kingPosition = __builtin_ctzll(kingPositionMask);
  return isAttackedBySlider(kingPositionMask) ||
         isAttackedByPawn(kingPosition) || isAttackedByKing(kingPosition) ||
         isAttackedByKnight(kingPosition);
};
bool Position::isPseudoLegalMove(const int &source,
                                 const int &destination) const {
  uint64_t sourceMask = 0b1uLL << source;
  if (isSquareEmpty(sourceMask)) {
    return false;
  };
  uint64_t destinationMask = 0b1uLL << destination;
  piece pieceType = getPieceType(sourceMask);
  color pieceColor = getPieceColor(sourceMask);
  uint64_t allPieces = pieces[white][all] | pieces[black][all];
  if (turn != pieceColor) {
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
bool Position::isLegalMove(const int &source, const int &destination) const {
  std::vector<std::vector<uint64_t>> piecesTemp =
      makeSoftMove(source, destination);
  Position tempPosition = Position(piecesTemp, turn);
  if (tempPosition.isCheck()) {
    return false;
  }
  return isPseudoLegalMove(source, destination);
}
