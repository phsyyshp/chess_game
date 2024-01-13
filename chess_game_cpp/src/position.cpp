#include "position.hpp"
// Initilazers;
// Setters
void Position::setWhitePiecesToInitialConfiguration() {

  pieces[white][rook] = 0b1ULL << a1 | 0b1ULL << h1;
  pieces[white][knight] = 0b1ULL << g1 | 0b1ULL << b1;
  pieces[white][bishop] = 0b1ULL << f1 | 0b1ULL << c1;
  pieces[white][queen] = 0b1ULL << d1;
  pieces[white][king] = 0b1ULL << e1;
  pieces[white][pawn] = 0b11111111ULL << 8;
  pieces[white][all] = pieces[white][rook] | pieces[white][knight] |
                       pieces[white][bishop] | pieces[white][queen] |
                       pieces[white][king] | pieces[white][pawn];
}
void Position::setBlackPiecesToInitialConfiguration() {
  pieces[black][rook] = 0b1ULL << a8 | 0b1ULL << h8;
  pieces[black][knight] = 0b1ULL << g8 | 0b1ULL << b8;
  pieces[black][bishop] = 0b1ULL << f8 | 0b1ULL << c8;
  pieces[black][queen] = 0b1ULL << d8;
  pieces[black][king] = 0b1ULL << e8;
  pieces[black][pawn] = 0b11111111ULL << 6 * 8;
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
// CAUTION: it does not change the turn automatically!
void Position::makeMove(Move move) {
  int from = move.getFrom();
  int to = move.getTo();
  int pieceType = move.getPiece();
  int pieceColor = move.getColor();
  bool isCapture = move.checkIsCapture();
  int oppositePieceColor = (pieceColor + 1) % 2;
  uint64_t toMask = (0b1ull << to);
  uint64_t notFromMask = ~(0b1ull << from);
  pieces[pieceColor][pieceType] &= notFromMask;
  pieces[pieceColor][pieceType] |= toMask;

  if (isCapture) {
    piece capturedPieceType = getPieceType(toMask);
    pieces[oppositePieceColor][capturedPieceType] &= (~toMask);
  }
  pieces[white][all] = pieces[white][rook] | pieces[white][knight] |
                       pieces[white][bishop] | pieces[white][queen] |
                       pieces[white][king] | pieces[white][pawn];
  pieces[black][all] = pieces[black][rook] | pieces[black][knight] |
                       pieces[black][bishop] | pieces[black][queen] |
                       pieces[black][king] | pieces[black][pawn];
};
std::vector<std::vector<uint64_t>> Position::getPieces() const {
  return pieces;
}
color Position ::getPieceColor(const uint64_t &sqMask) const {

  if (sqMask & pieces[black][all]) {
    return color::black;
  } else if (sqMask & pieces[white][all]) {
    return color::white;
  } else {
    std::cerr << "empty square";
  }
}
piece Position::getPieceType(const uint64_t &sqMask) const {
  for (int pieceInd = 0; pieceInd < 6; pieceInd++) {

    if (sqMask & (pieces[white][pieceInd] | pieces[black][pieceInd])) {

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
