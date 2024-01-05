#include "position.hpp"
// #include "loader.hpp"
// #include <cstdint>
// #include <iostream>

// Initilazers;
const std::vector<uint64_t> Position::rookMagicNumbers =
    readMagicNumbersToVec("rook");
const std::vector<std::vector<uint64_t>> Position::rookLookUpTables =
    readLookUpTables("rook");

const std::vector<uint64_t> Position::bishopMagicNumbers =
    readMagicNumbersToVec("bishop");
const std::vector<std::vector<uint64_t>> Position::bishopLookUpTables =
    readLookUpTables("bishop");
const std::vector<uint64_t> Position::knightLookUpTable =
    readKnightLookUpTable();
const std::vector<uint64_t> Position::whitePawnLookUpTable =
    readWhitePawnLookUpTable();

const std::vector<uint64_t> Position::blackPawnLookUpTable =
    readWhitePawnLookUpTable();

const std::vector<uint64_t> Position::kingLookUpTable = readKingLookUpTable();

// Setters
void Position::setWhitePiecesToInitialConfiguration() {
  pieces.rooks.white = 0b1ULL | 0b1ULL << 7;
  pieces.knights.white = 0b1ULL << 6 | 0b1ULL << 1;
  pieces.bishops.white = 0b1ULL << 5 | 0b1ULL << 2;
  pieces.queens.white = 0b1ULL << 4;
  pieces.king.white = 0b1ULL << 3;
  pieces.pawns.white = 0b11111111ULL << 8;
  pieces.all.white = pieces.rooks.white | pieces.knights.white |
                     pieces.bishops.white | pieces.queens.white |
                     pieces.king.white | pieces.pawns.white;
}
void Position::setBlackPiecesToInitialConfiguration() {
  pieces.rooks.black = 0b1ULL << (8 * 7) | 0b1ULL << (7 + 8 * 7);
  pieces.knights.black = 0b1ULL << (6 + 8 * 7) | 0b1ULL << (1 + 8 * 7);
  pieces.bishops.black = 0b1ULL << (5 + 8 * 7) | 0b1ULL << (2 + 8 * 7);
  pieces.queens.black = 0b1ULL << (4 + 8 * 7);
  pieces.king.black = 0b1ULL << (3 + 8 * 7);
  pieces.pawns.black = 0b11111111ULL << (8 * 6);
  pieces.all.black = pieces.rooks.black | pieces.knights.black |
                     pieces.bishops.black | pieces.queens.black |
                     pieces.king.black | pieces.pawns.black;
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
// Getters
Position::Pieces Position::getPieces() const { return pieces; }
// Position::Pieces Position::getBlackPieces() const { return pieces; }.black

color Position ::getPieceColor(const uint64_t &positionMask) const {

  if (positionMask | pieces.all.black) {
    return color::black;
  } else if (positionMask | pieces.all.white) {
    return color::white;
  } else {
    std::cerr << "empty square";
  }
}
std::string Position::getPieceType(const uint64_t &positionMask) const {
  std::string pieceType;
  if (positionMask & (pieces.rooks.white | pieces.rooks.black)) {
    pieceType = "rook";
  } else if (positionMask & (pieces.pawns.white | pieces.pawns.black)) {
    pieceType = "pawn";
  } else if (positionMask & (pieces.queens.white | pieces.queens.black)) {

    pieceType = "queen";
  } else if (positionMask & (pieces.knights.white | pieces.knights.black)) {

    pieceType = "knight";
  } else if (positionMask & (pieces.king.white | pieces.king.black)) {

    pieceType = "king";
  } else if (positionMask & (pieces.bishops.white | pieces.bishops.black)) {

    pieceType = "bishop";
  }
  return pieceType;
}
// misc
void Position::printBoard() const {
  uint64_t n = pieces.all.white | pieces.all.black;
  for (int i = 63; i >= 0; i--) {
    if (i % 8 == 7) {
      std::cout << "\n";
    } else {
      std::cout << ((n >> i) & 1);
    }
  }
  std::cout << "\n";
}
void Position::changeTurn() {
  if (turn == color::white) {
    turn = color::black;
  } else {
    turn = color::white;
  }
}
bool Position::isSquareEmpty(const uint64_t &squareMask) const {
  uint64_t allPieces = pieces.all.white | pieces.all.black;
  return allPieces & squareMask == 0;
}
bool Position::isDestinationOccupiedBySameColor(const int &source,
                                                const int &destination) const {
  if (turn == color::white) {
    return (pieces.all.white & (0b1ULL << destination)) != 0;
  } else {
    return (pieces.all.black & (1ULL << destination)) != 0;
  }
}
bool Position::isSlidingMove(const std::string &piecetype) const {
  return piecetype == "bishop" || piecetype == "queen" || piecetype == "rook";
}
bool Position::isSlidingMoveLegal(const uint64_t &sourceMask,
                                  const uint64_t &destinationMask,
                                  const uint64_t &allPieces,
                                  const std::string &pieceType) const {

  if (pieceType == "bishop") {
    return destinationMask & getAttackMask(sourceMask, allPieces,
                                           bishopMagicNumbers,
                                           bishopLookUpTables, pieceType);
  } else if (pieceType == "rook") {

    return destinationMask & getAttackMask(sourceMask, allPieces,
                                           rookMagicNumbers, rookLookUpTables,
                                           pieceType);
  } else if (pieceType == "queen") {
    return destinationMask &
           (getAttackMask(sourceMask, allPieces, bishopMagicNumbers,
                          bishopLookUpTables, pieceType) |
            getAttackMask(sourceMask, allPieces, rookMagicNumbers,
                          rookLookUpTables, pieceType));
  } else {
    std::cerr << "it's not a sliding move";
    return false;
  }
}
bool Position::isKnightMoveLegal(const int &source,
                                 const uint64_t &destinationMask) const {

  return destinationMask & knightLookUpTable[source];
};
// bool Position::isEnPassant(const uint64_t &sourceMask,
//                              const uint64_t &destinationMask) const {

// };
bool Position::isSinglePawnMoveLegal(const uint64_t &sourceMask,
                                     const uint64_t &destinationMask) const {
  bool isLegalPush;
  uint64_t legalCaptures; // TODO: there may be potential bugs regarding out of
                          // bound;
  if (turn == color::white) {
    if (pieces.pawns.black &
        whitePawnLookUpTable[__builtin_ctzll(sourceMask)]) {
      return true;
    }
    isLegalPush = (sourceMask == (destinationMask >> 8));
  } else if (turn == color::black) {
    if (pieces.pawns.white &
        blackPawnLookUpTable[__builtin_ctzll(sourceMask)]) {
      return true;
    }

    isLegalPush = (sourceMask == (destinationMask << 8));
  }
  return isLegalPush && isSquareEmpty(destinationMask);
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
         isDoublePawnMoveLegal(sourceMask, destinationMask);
}
bool Position::isKingMoveLegal(const int &source,
                               const uint64_t &destinationMask) const {
  return destinationMask & kingLookUpTable[source];
};

bool Position::isAttackedBySlider(const uint64_t &sourceMask) const {

  uint64_t allPieces = pieces.all.black | pieces.all.white;
  if (turn == color::white) {
    return (pieces.bishops.black &
            getAttackMask(sourceMask, allPieces, bishopMagicNumbers,
                          bishopLookUpTables, "bishop")) |
           (pieces.rooks.black & getAttackMask(sourceMask, allPieces,
                                               rookMagicNumbers,
                                               rookLookUpTables, "rook")) |
           (pieces.queens.black &
            (getAttackMask(sourceMask, allPieces, bishopMagicNumbers,
                           bishopLookUpTables, "bishop") |
             getAttackMask(sourceMask, allPieces, rookMagicNumbers,
                           rookLookUpTables, "rook")));
  } else if (turn == color::black) {

    return (pieces.bishops.white &
            getAttackMask(sourceMask, allPieces, bishopMagicNumbers,
                          bishopLookUpTables, "bishop")) |
           (pieces.rooks.white & getAttackMask(sourceMask, allPieces,
                                               rookMagicNumbers,
                                               rookLookUpTables, "rook")) |
           (pieces.queens.white &
            (getAttackMask(sourceMask, allPieces, bishopMagicNumbers,
                           bishopLookUpTables, "bishop") |
             getAttackMask(sourceMask, allPieces, rookMagicNumbers,
                           rookLookUpTables, "rook")));
  }
}
bool Position::isAttackedByPawn(const int &source) const {

  if (turn == color::white) {
    return pieces.pawns.black & whitePawnLookUpTable[source];
  } else if (turn == color::black) {
    return pieces.pawns.white & blackPawnLookUpTable[source];
  }
}
bool Position::isAttackedByKnight(const int &source) const {
  if (turn == color::white) {
    return pieces.knights.black & knightLookUpTable[source];
  } else if (turn == color::black) {

    return pieces.knights.white & knightLookUpTable[source];
  }
}
bool Position::isAttackedByKing(const int &source) const {
  if (turn == color::white) {
    return pieces.king.black & kingLookUpTable[source];
  } else if (turn == color::black) {

    return pieces.king.white & kingLookUpTable[source];
  }
}
bool Position::isCheck() const {
  uint64_t kingPositionMask;
  uint64_t kingPosition;

  if (turn == color::white) {

    kingPositionMask = pieces.king.white;
    kingPosition = __builtin_ctzll(kingPositionMask);
  } else {

    kingPositionMask = pieces.king.black;
    kingPosition = __builtin_ctzll(kingPositionMask);
  }
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
  std::string pieceType = getPieceType(sourceMask);
  color pieceColor = getPieceColor(sourceMask);
  uint64_t allPieces = pieces.all.white | pieces.all.black;

  if (turn != pieceColor) {
    return false;
  };
  if (pieceType == "pawn") {
    return isPawnMoveLegal(sourceMask, destinationMask);
  }
  if (isDestinationOccupiedBySameColor(source, destination)) {
    return false;
  };
  if (isSlidingMove(pieceType)) {
    return isSlidingMoveLegal(sourceMask, destinationMask, allPieces,
                              pieceType);
  };
  if (pieceType == "knight") {
    return isKnightMoveLegal(sourceMask, destinationMask);
  };
  if (pieceType == "king") {
    return isKingMoveLegal(source, destinationMask);
  }
  // } else if (isPawnMove(pieceType)) {
  // return isPawnMoveLegal(pieceType);
  // } else if (isKingMove())
}
