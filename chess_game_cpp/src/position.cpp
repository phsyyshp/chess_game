#include "position.hpp"
// Constructors;
Position::Position(std::array<std::array<uint64_t, 6>, 2> pieces_, color turn_)
    : pieces(pieces_), gameState(turn_) {
  for (int j = 0; j < 64; j++) {
    mailbox[j] = noPiece;
  }
  for (auto coloredPieces : pieces_) {
    int i = 0;
    for (uint64_t piece_ : coloredPieces) {
      while (piece_) {
        int sq = __builtin_ctzll(piece_);
        mailbox[sq] = static_cast<piece>(i);
        piece_ ^= (0b1ull << sq);
      }
      i++;
    }
  }
}
// Setters;
void Position::setWhitePiecesToInitialConfiguration() {
  pieces[white][rook] = 0b1ULL << a1 | 0b1ULL << h1;
  pieces[white][knight] = 0b1ULL << g1 | 0b1ULL << b1;
  pieces[white][bishop] = 0b1ULL << f1 | 0b1ULL << c1;
  pieces[white][queen] = 0b1ULL << d1;
  pieces[white][king] = 0b1ULL << e1;
  pieces[white][pawn] = 0b11111111ULL << 8;
  mailbox[a1] = rook;
  mailbox[h1] = rook;
  mailbox[g1] = knight;
  mailbox[b1] = knight;
  mailbox[f1] = bishop;
  mailbox[c1] = bishop;
  mailbox[d1] = queen;
  mailbox[e1] = king;
  mailbox[a2] = pawn;
  mailbox[b2] = pawn;
  mailbox[c2] = pawn;
  mailbox[d2] = pawn;
  mailbox[e2] = pawn;
  mailbox[f2] = pawn;
  mailbox[g2] = pawn;
  mailbox[h2] = pawn;
}
void Position::setBlackPiecesToInitialConfiguration() {
  pieces[black][rook] = 0b1ULL << a8 | 0b1ULL << h8;
  pieces[black][knight] = 0b1ULL << g8 | 0b1ULL << b8;
  pieces[black][bishop] = 0b1ULL << f8 | 0b1ULL << c8;
  pieces[black][queen] = 0b1ULL << d8;
  pieces[black][king] = 0b1ULL << e8;
  pieces[black][pawn] = 0b11111111ULL << 6 * 8;
  mailbox[a8] = rook;
  mailbox[h8] = rook;
  mailbox[g8] = knight;
  mailbox[b8] = knight;
  mailbox[f8] = bishop;
  mailbox[c8] = bishop;
  mailbox[d8] = queen;
  mailbox[e8] = king;
  mailbox[a7] = pawn;
  mailbox[b7] = pawn;
  mailbox[c7] = pawn;
  mailbox[d7] = pawn;
  mailbox[e7] = pawn;
  mailbox[f7] = pawn;
  mailbox[g7] = pawn;
  mailbox[h7] = pawn;
}

void Position::setBoardToInitialConfiguration() {
  for (int i = 0; i < 64; i++) {
    mailbox[i] = noPiece;
  }
  setWhitePiecesToInitialConfiguration();
  setBlackPiecesToInitialConfiguration();
}
void Position::changeTurn() { gameState.changeTurn(); }
// Operators;
Position &Position::operator=(const Position &rhs) {
  pieces = rhs.pieces;
  mailbox = rhs.mailbox;
  gameState = rhs.gameState;
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
piece Position::getPiece(int square) const { return mailbox[square]; }
// TODO: there maybe bugs here! Especially when there is not any pieace of that
// type is left;
piece Position::getPieceType(const uint64_t &sqMask) const {
  for (int pieceIdx = 0; pieceIdx < 6; pieceIdx++) {
    if (sqMask & (pieces[white][pieceIdx] | pieces[black][pieceIdx])) {
      return static_cast<piece>(pieceIdx);
    }
  }
  return noPiece;
}
color Position::getTurn() const { return gameState.getTurn(); }
color Position::getOppositeTurn() const {
  return oppositeColor[gameState.getTurn()];
}

GameState Position::getGameState() const { return gameState; }
uint64_t Position::getAllPieces(const color &color_) const {
  return pieces[color_][rook] | pieces[color_][knight] |
         pieces[color_][bishop] | pieces[color_][queen] | pieces[color_][king] |
         pieces[color_][pawn];
}
piece Position::getCapturedInLastMove() const { return capturedInLastMove; }
uint64_t Position::getAttacksToKing() const {

  uint64_t allPieces = getAllPieces(black) | getAllPieces(white);
  color colorOfKing = gameState.getTurn();
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

// bools
bool Position::isInCheck() const { return (getAttacksToKing()) != 0; }
bool Position::isEmpty(int square_) const {
  return mailbox[square_] == noPiece;
}
// Asuming; non-special moves(!pro||!cast) and valid(des =empt||opColOc) input,
// Assuming pseudo-legal move as an input;
// Assuming kings are present, other wise segmentation error;
// returns true if pseudo legal input is also legal;
// Use it for temprory changes.
// It changes turns automatically for now.
bool Position::makeMove(const Move &move) {
  uint flag = move.getFlags();
  switch (flag) {
  case MoveType::quietMoves:
    makeQuietMove(move);
    break;
  case MoveType::captures:
    capture(move);
    break;
  case MoveType::doublePawnPush:
    makeDoublePawnPush(move);
    break;
  case MoveType::ePCapture:
    makeEPCapture(move);
    break;

  default:
    break;
  }
  // legality
  bool isLegal = !isInCheck();
  // gameState
  changeTurn();
  return isLegal;
}

// it does not change turn automatically;
void Position::makeQuietMove(const Move &move) {

  // decoding move
  uint from = move.getFrom();
  uint to = move.getTo();
  int movingPiece = mailbox[from];
  int oppositePieceColor = getOppositeTurn();

  // bit masks
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);

  // moving
  pieces[gameState.getTurn()][movingPiece] &= ~fromMask;
  capturedInLastMove = noPiece;

  pieces[gameState.getTurn()][movingPiece] |= toMask;
  // Mailbox operations;
  mailbox[to] = mailbox[from];
  mailbox[from] = noPiece;
  gameState.setEnPassant(NO_EP);
}
void Position::capture(const Move &move) {
  // decoding move
  uint from = move.getFrom();
  uint to = move.getTo();
  int movingPiece = mailbox[from];
  int oppositePieceColor = getOppositeTurn();

  // bit masks
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);

  // moving
  pieces[gameState.getTurn()][movingPiece] &= ~fromMask;
  piece capturedPieceType = mailbox[to];
  pieces[oppositePieceColor][capturedPieceType] &= (~toMask);
  capturedInLastMove = capturedPieceType;
  pieces[gameState.getTurn()][movingPiece] |= toMask;
  // Mailbox operations;
  mailbox[to] = mailbox[from];
  mailbox[from] = noPiece;
  gameState.setEnPassant(NO_EP);
}
void Position::makeDoublePawnPush(const Move &move) {
  makeQuietMove(move);
  uint file = squareTofile[move.getFrom()];
  gameState.setEnPassant(file);
}
void Position::makeEPCapture(const Move &move) {
  color turn = gameState.getTurn();
  uint fileEP = getGameState().getEnPassant();

  uint64_t victimMask;
  switch (turn) {

  case white:
    victimMask = 0b1ull << (fileEP + 8 * 4);
    pieces[black][pawn] ^= victimMask;
    mailbox[fileEP + 8 * 4] = noPiece;
    break;

  case black:
    victimMask = 0b1ull << (fileEP + 8 * 3);
    pieces[white][pawn] ^= victimMask;
    mailbox[fileEP + 8 * 3] = noPiece;
    break;
  default:
    break;
  }
  makeQuietMove(move);
}

// FIX IT: sth is wrong here fix me!
// void Position::undoMove(Move move) {
//   int from = move.getFrom();
//   int to = move.getTo();
//   int piece_ = move.getPiece();
//   int color_ = move.getColor();
//   bool isCapture = move.checkIsCapture();
//   int oppositePieceColor = (color_ + 1) % 2;
//   uint64_t toMask = (0b1ull << to);
//   uint64_t notFromMask = ~(0b1ull << from);
//   changeTurn();
//   pieces[color_][piece_] &= ~toMask;
//   pieces[color_][piece_] |= ~notFromMask;

//   if (isCapture) {
//     pieces[oppositePieceColor][capturedInLastMove] |= (toMask);
//   }
// }
std::array<piece, 64> Position::getMailbox() const { return mailbox; }
std::array<std::array<uint64_t, 6>, 2> Position::getPieces() const {
  return pieces;
}
// Misc
// TODO: this function is hideous refactor it;
void Position::printBoard() const {
  uint64_t allPieces = getAllPieces(white) | getAllPieces(black);
  piece piece_;
  color color_;
  std::string pieceIcon;
  for (int i = 7; i >= -1; i--) {
    for (int j = -1; j < 8; j++) {
      if (i != -1 && j != -1) {

        piece_ = getPieceType(0b1ull << (j + i * 8));
        color_ = getPieceColor(0b1ull << (j + i * 8));
        pieceIcon = getPieceIcon(piece_, color_);
        if ((i + j + 1) % 2 == 0) {
          pieceIcon = colorizeString(pieceIcon, "30", "46");
        } else {
          pieceIcon = colorizeString(pieceIcon, "30", "40");
        }
      } else if (j != -1 && i == -1) {
        pieceIcon = j + 'a';
        pieceIcon = colorizeString(pieceIcon, "30", "46");
      } else if (j == -1; i != -1) {
        pieceIcon = i + '1';
        pieceIcon = colorizeString(pieceIcon, "30", "46");
      } else {
        pieceIcon = ' ';
        pieceIcon = colorizeString(pieceIcon, "30", "46");
      }
      std::cout << pieceIcon;
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}
