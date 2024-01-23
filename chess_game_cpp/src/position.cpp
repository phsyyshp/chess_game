#include "position.hpp"
// Setters
void Position::setWhitePiecesToInitialConfiguration() {

  pieces[white][rook] = 0b1ULL << a1 | 0b1ULL << h1;
  pieces[white][knight] = 0b1ULL << g1 | 0b1ULL << b1;
  pieces[white][bishop] = 0b1ULL << f1 | 0b1ULL << c1;
  pieces[white][queen] = 0b1ULL << d1;
  pieces[white][king] = 0b1ULL << e1;
  pieces[white][pawn] = 0b11111111ULL << 8;
}
void Position::setBlackPiecesToInitialConfiguration() {
  pieces[black][rook] = 0b1ULL << a8 | 0b1ULL << h8;
  pieces[black][knight] = 0b1ULL << g8 | 0b1ULL << b8;
  pieces[black][bishop] = 0b1ULL << f8 | 0b1ULL << c8;
  pieces[black][queen] = 0b1ULL << d8;
  pieces[black][king] = 0b1ULL << e8;
  pieces[black][pawn] = 0b11111111ULL << 6 * 8;
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
// Operators;
Position &Position::operator=(const Position &rhs) {
  turn = rhs.turn;
  pieces = rhs.pieces;
  canWhiteCastle = rhs.canWhiteCastle;
  canBlackCastle = rhs.canBlackCastle;
  capturedInLastMove = rhs.capturedInLastMove;
  return *this;
}
// Getters;
color Position ::getPieceColor(const uint64_t &sqMask) const {

  if (sqMask & getAllPieces(black)) {
    return color::black;
  } else if (sqMask & getAllPieces(white)) {
    return color::white;
  } else {

    return color::invalid;
  }
}
// TODO: there maybe bugs here!
piece Position::getPieceType(const uint64_t &sqMask) const {
  for (int pieceInd = 0; pieceInd < 6; pieceInd++) {

    if (sqMask & (pieces[white][pieceInd] | pieces[black][pieceInd])) {

      return static_cast<piece>(pieceInd);
    }
  }
  return noPiece;
}
color Position::getTurn() const { return turn; }
color Position::getOppositeTurn() const { return oppositeColor[turn]; }
uint64_t Position::getAllPieces(const color &pieceColor) const {
  return pieces[pieceColor][rook] | pieces[pieceColor][knight] |
         pieces[pieceColor][bishop] | pieces[pieceColor][queen] |
         pieces[pieceColor][king] | pieces[pieceColor][pawn];
}
uint64_t Position::getAttacksToKing() const {

  uint64_t allPieces = getAllPieces(black) | getAllPieces(white);
  color colorOfKing = turn;
  color oppositeColor = getOppositeTurn();
  square squareOfKing =
      static_cast<square>(__builtin_ctzll(pieces[colorOfKing][king]));
  uint64_t oppositePawns = pieces[oppositeColor][pawn];
  uint64_t oppositeKnights = pieces[oppositeColor][knight];
  uint64_t oppositeRooks = pieces[oppositeColor][rook];
  uint64_t oppositeBishops = pieces[oppositeColor][bishop];
  uint64_t oppositeQueens = pieces[oppositeColor][queen];
  uint64_t oppositeRookQueens = oppositeRooks | oppositeQueens;

  uint64_t oppositeBishopQueens = oppositeBishops | oppositeQueens;
  return (knightLookUpTable[squareOfKing] & oppositeKnights) |
         (pawnLookUpTable[colorOfKing][squareOfKing] & oppositePawns) |
         (getBishopAttackMask(squareOfKing, allPieces) & oppositeBishopQueens) |
         (getRookAttackMask(squareOfKing, allPieces) & oppositeRookQueens);
}

bool Position::isInCheck() const { return (getAttacksToKing()) != 0; }

// Asuming; non-special moves(!pro|!cast) and valid(des =empt|opColOc) input,
// use it for temprory changes.
// It changes turns automatically for now.
void Position::makeMove(Move move) {
  int from = move.getFrom();
  int to = move.getTo();
  int pieceType = move.getPiece();
  int pieceColor = move.getColor();
  bool isCapture = move.checkIsCapture();
  int oppositePieceColor = (pieceColor + 1) % 2;
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);
  pieces[pieceColor][pieceType] &= ~fromMask;
  if (isCapture) {
    piece capturedPieceType = getPieceType(toMask);
    pieces[oppositePieceColor][capturedPieceType] &= (~toMask);
    capturedInLastMove = capturedPieceType;
  } else {
    // capturedInLastMove = nulity;
    ;
  }
  pieces[pieceColor][pieceType] |= toMask;

  changeTurn();
}
// FIX IT: sth is wrong here fix me!
void Position::undoMove(Move move) {
  int from = move.getFrom();
  int to = move.getTo();
  int pieceType = move.getPiece();
  int pieceColor = move.getColor();
  bool isCapture = move.checkIsCapture();
  int oppositePieceColor = (pieceColor + 1) % 2;
  uint64_t toMask = (0b1ull << to);
  uint64_t notFromMask = ~(0b1ull << from);
  changeTurn();
  pieces[pieceColor][pieceType] &= ~toMask;
  pieces[pieceColor][pieceType] |= ~notFromMask;

  if (isCapture) {
    pieces[oppositePieceColor][capturedInLastMove] |= (toMask);
  }
}
std::array<std::array<uint64_t, 6>, 2> Position::getPieces() const {
  return pieces;
}
// Misc
void Position::printBoard() const {
  uint64_t allPieces = getAllPieces(white) | getAllPieces(black);
  piece pieceType;
  color pieceColor;
  std::string pieceIcon;
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      pieceType = getPieceType(0b1ull << (j + i * 8));
      pieceColor = getPieceColor(0b1ull << (j + i * 8));
      pieceIcon = getPieceIcon(pieceType, pieceColor);
      if ((i + j + 1) % 2 == 0) {
        pieceIcon = colorizeString(pieceIcon, "30", "46");
      } else {
        pieceIcon = colorizeString(pieceIcon, "30", "40");
      }
      std::cout << pieceIcon;
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}
