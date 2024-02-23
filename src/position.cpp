#include "position.hpp"
// Constructors;
Position::Position(const std::string &FENstr) {
  std::vector<std::string> fieldVec = FENtoFields(FENstr);
  pieces = FENtoPieces(fieldVec[0]);
  gameState = FENtoGameStateInt(FENstr);
  // ply = stoi(fieldVec[5]);
  for (int j = 0; j < 64; j++) {
    mailbox[j] = NO_PIECE;
  }
  for (auto coloredPieces : pieces) {
    int i = 0;
    for (uint64_t piece_ : coloredPieces) {
      while (piece_) {
        int sq = __builtin_ctzll(piece_);
        mailbox[sq] = static_cast<Piece>(i);
        piece_ ^= (0b1ull << sq);
      }
      i++;
    }
  }
  zobristHash = Zobrist::generateTotalZobristKey(*this);
}
Position::Position(const std::array<std::array<uint64_t, 6>, 2> &pieces_,
                   Color turn_)
    : pieces(pieces_), gameState(turn_) {
  for (int j = 0; j < 64; j++) {
    mailbox[j] = NO_PIECE;
  }
  for (auto coloredPieces : pieces_) {
    int i = 0;
    for (uint64_t piece_ : coloredPieces) {
      while (piece_) {
        int sq = __builtin_ctzll(piece_);
        mailbox[sq] = static_cast<Piece>(i);
        piece_ ^= (0b1ull << sq);
      }
      i++;
    }
  }
  zobristHash = Zobrist::generateTotalZobristKey(*this);
}
// Setters;
void Position::setWhitePiecesToInitialConfiguration() {
  pieces[WHITE][ROOK] = 0b1ULL << A1 | 0b1ULL << h1;
  pieces[WHITE][KNIGHT] = 0b1ULL << g1 | 0b1ULL << B1;
  pieces[WHITE][BISHOP] = 0b1ULL << f1 | 0b1ULL << C1;
  pieces[WHITE][QUEEN] = 0b1ULL << D1;
  pieces[WHITE][KING] = 0b1ULL << e1;
  pieces[WHITE][PAWN] = 0b11111111ULL << 8;
  mailbox[A1] = ROOK;
  mailbox[h1] = ROOK;
  mailbox[g1] = KNIGHT;
  mailbox[B1] = KNIGHT;
  mailbox[f1] = BISHOP;
  mailbox[C1] = BISHOP;
  mailbox[D1] = QUEEN;
  mailbox[e1] = KING;
  mailbox[a2] = PAWN;
  mailbox[b2] = PAWN;
  mailbox[c2] = PAWN;
  mailbox[d2] = PAWN;
  mailbox[e2] = PAWN;
  mailbox[f2] = PAWN;
  mailbox[g2] = PAWN;
  mailbox[h2] = PAWN;
}
void Position::setBlackPiecesToInitialConfiguration() {
  pieces[BLACK][ROOK] = 0b1ULL << a8 | 0b1ULL << h8;
  pieces[BLACK][KNIGHT] = 0b1ULL << g8 | 0b1ULL << b8;
  pieces[BLACK][BISHOP] = 0b1ULL << f8 | 0b1ULL << c8;
  pieces[BLACK][QUEEN] = 0b1ULL << d8;
  pieces[BLACK][KING] = 0b1ULL << e8;
  pieces[BLACK][PAWN] = 0b11111111ULL << 6 * 8;
  mailbox[a8] = ROOK;
  mailbox[h8] = ROOK;
  mailbox[g8] = KNIGHT;
  mailbox[b8] = KNIGHT;
  mailbox[f8] = BISHOP;
  mailbox[c8] = BISHOP;
  mailbox[d8] = QUEEN;
  mailbox[e8] = KING;
  mailbox[a7] = PAWN;
  mailbox[b7] = PAWN;
  mailbox[c7] = PAWN;
  mailbox[d7] = PAWN;
  mailbox[e7] = PAWN;
  mailbox[f7] = PAWN;
  mailbox[g7] = PAWN;
  mailbox[h7] = PAWN;
}
void Position::setBoardToInitialConfiguration() {
  clear();
  setWhitePiecesToInitialConfiguration();
  setBlackPiecesToInitialConfiguration();
  zobristHash = Zobrist::generateTotalZobristKey(*this);
}
void Position::clear() {
  for (int i = 0; i < 64; i++) {
    mailbox[i] = NO_PIECE;
  }
  for (int j = 0; j < 6; j++) {
    pieces[WHITE][j] = 0;
    pieces[BLACK][j] = 0;
  }
  ply = 0;
  gameState.clear();
  zobristHash = 0ull;
  capturedInLastMove = NO_PIECE;
}
void Position::changeTurn() {
  gameState.changeTurn();
  Zobrist::changeTurn(zobristHash);
}

void Position::setPly(int ply_) { ply = ply_; }

// Operators;
Position &Position::operator=(const Position &rhs) {
  pieces = rhs.pieces;
  mailbox = rhs.mailbox;
  gameState = rhs.gameState;
  capturedInLastMove = rhs.capturedInLastMove;
  ply = rhs.ply;
  zobristHash = rhs.zobristHash;
  return *this;
}
// Getters;
Color Position ::getPieceColor(const uint64_t &sqMask) const {

  if (sqMask & getAllPieces(BLACK)) {
    return Color::BLACK;
  } else if (sqMask & getAllPieces(WHITE)) {
    return Color::WHITE;
  } else {

    return Color::INVALID;
  }
}
uint64_t Position::getZobrist() const { return zobristHash; }
Piece Position::getPiece(int square) const { return mailbox[square]; }
int Position::getPly() const { return ply; }
// TODO: there maybe bugs here! Especially when there is not any pieace of that
// type is left;
Piece Position::getPieceType(const uint64_t &sqMask) const {
  for (int pieceIdx = 0; pieceIdx < 6; pieceIdx++) {
    if (sqMask & (pieces[WHITE][pieceIdx] | pieces[BLACK][pieceIdx])) {
      return static_cast<Piece>(pieceIdx);
    }
  }
  return NO_PIECE;
}
Color Position::getTurn() const { return gameState.getTurn(); }
Color Position::getOppositeTurn() const {
  return OPPOSITE_COLOR[gameState.getTurn()];
}

GameState Position::getGameState() const { return gameState; }
uint64_t Position::getAllPieces(const Color &color_) const {
  return pieces[color_][ROOK] | pieces[color_][KNIGHT] |
         pieces[color_][BISHOP] | pieces[color_][QUEEN] | pieces[color_][KING] |
         pieces[color_][PAWN];
}
Piece Position::getCapturedInLastMove() const { return capturedInLastMove; }
uint64_t Position::getAttacksToKing() const {

  uint64_t allPieces = getAllPieces(BLACK) | getAllPieces(WHITE);
  Color colorOfKing = gameState.getTurn();
  Color oppositeColor = getOppositeTurn();
  Square squareOfKing =
      static_cast<Square>(__builtin_ctzll(pieces[colorOfKing][KING]));
  uint64_t oppositePawns = pieces[oppositeColor][PAWN];
  uint64_t oppositeKnights = pieces[oppositeColor][KNIGHT];
  uint64_t oppositeRooks = pieces[oppositeColor][ROOK];
  uint64_t oppositeBishops = pieces[oppositeColor][BISHOP];
  uint64_t oppositeQueens = pieces[oppositeColor][QUEEN];
  uint64_t oppositeKing = pieces[oppositeColor][KING];
  uint64_t oppositeRookQueens = oppositeRooks | oppositeQueens;
  uint64_t oppositeBishopQueens = oppositeBishops | oppositeQueens;
  return (knightLookUpTable[squareOfKing] & oppositeKnights) |
         (pawnLookUpTable[colorOfKing][squareOfKing] & oppositePawns) |
         (getBishopAttackMask(squareOfKing, allPieces) & oppositeBishopQueens) |
         (getRookAttackMask(squareOfKing, allPieces) & oppositeRookQueens) |
         (kingLookUpTable[squareOfKing] & oppositeKing);
}

uint64_t Position::getAttacksToSquare(Square square_) const {

  uint64_t allPieces = getAllPieces(BLACK) | getAllPieces(WHITE);
  Color colorOfVictim = gameState.getTurn();
  Color oppositeColor = getOppositeTurn();
  Square victimSq = square_;
  uint64_t oppositePawns = pieces[oppositeColor][PAWN];
  uint64_t oppositeKnights = pieces[oppositeColor][KNIGHT];
  uint64_t oppositeRooks = pieces[oppositeColor][ROOK];
  uint64_t oppositeBishops = pieces[oppositeColor][BISHOP];
  uint64_t oppositeQueens = pieces[oppositeColor][QUEEN];
  uint64_t oppositeRookQueens = oppositeRooks | oppositeQueens;
  uint64_t oppositeKing = pieces[oppositeColor][KING];
  uint64_t oppositeBishopQueens = oppositeBishops | oppositeQueens;
  return (knightLookUpTable[victimSq] & oppositeKnights) |
         (pawnLookUpTable[colorOfVictim][victimSq] & oppositePawns) |
         (getBishopAttackMask(victimSq, allPieces) & oppositeBishopQueens) |
         (getRookAttackMask(victimSq, allPieces) & oppositeRookQueens) |
         (kingLookUpTable[victimSq] & oppositeKing);
}
// bools
// FIX IT: kings can become adjacent;
bool Position::isInCheck() const { return (getAttacksToKing()) != 0; }
bool Position::isInCheck(Square square_) const {
  return (getAttacksToSquare(square_) != 0);
}

bool Position::isEmpty(int square_) const {
  return mailbox[square_] == NO_PIECE;
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
  case MoveType::kingCastle:
    makeKingCastle(move);
    break;
  case MoveType::queenCastle:
    makeQueenCastle(move);
    break;
  case MoveType::rookPromotion:
    makePromotion(move, ROOK);
    break;
  case MoveType::rookPromoCapture:
    makePromoCapture(move, ROOK);
    break;
  case MoveType::queenPromotion:
    makePromotion(move, QUEEN);
    break;
  case MoveType::queenPromoCapture:
    makePromoCapture(move, QUEEN);
    break;
  case MoveType::bishopPromotion:
    makePromotion(move, BISHOP);
    break;
  case MoveType::bishopPromoCapture:
    makePromoCapture(move, BISHOP);
    break;
  case MoveType::knightPromotion:
    makePromotion(move, KNIGHT);
    break;
  case MoveType::knightPromoCapture:
    makePromoCapture(move, KNIGHT);
    break;
  default:
    break;
  }
  // legality
  bool isLegal = !isInCheck();
  // gameState
  changeTurn();
  ply++;
  return isLegal;
}

// it does not change turn automatically;
void Position::makeQuietMove(const Move &move) {

  // decoding move
  uint from = move.getFrom();
  uint to = move.getTo();
  Piece movingPiece = mailbox[from];
  int oppositePieceColor = getOppositeTurn();

  updateCastlingRights(from, movingPiece);
  // bit masks
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);

  // moving
  pieces[gameState.getTurn()][movingPiece] &= ~fromMask;
  Zobrist::removeAddPiece(zobristHash, from, movingPiece, getTurn());
  capturedInLastMove = NO_PIECE;

  pieces[gameState.getTurn()][movingPiece] |= toMask;
  Zobrist::removeAddPiece(zobristHash, to, movingPiece, getTurn());
  // Mailbox operations;
  mailbox[to] = mailbox[from];
  mailbox[from] = NO_PIECE;
  int epFile = gameState.getEnPassant();
  if (epFile != NO_EP) {
    Zobrist::flipEpStatus(zobristHash, epFile);
  }
  gameState.setEnPassant(NO_EP);
}
void Position::capture(const Move &move) {
  // decoding move
  uint from = move.getFrom();
  uint to = move.getTo();
  Piece movingPiece = mailbox[from];
  int oppositePieceColor = getOppositeTurn();
  updateCastlingRights(from, movingPiece);
  // bit masks
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);
  // moving
  pieces[gameState.getTurn()][movingPiece] &= ~fromMask;
  Zobrist::removeAddPiece(zobristHash, from, movingPiece, getTurn());
  Piece capturedPieceType = mailbox[to];
  if (capturedPieceType == ROOK) {
    updateCastlingRights(to, ROOK);
  }
  pieces[oppositePieceColor][capturedPieceType] &= (~toMask);
  Zobrist::removeAddPiece(zobristHash, to, capturedPieceType,
                          getOppositeTurn());
  capturedInLastMove = capturedPieceType;
  pieces[gameState.getTurn()][movingPiece] |= toMask;
  Zobrist::removeAddPiece(zobristHash, to, movingPiece, getTurn());

  // Mailbox operations;
  mailbox[to] = mailbox[from];
  mailbox[from] = NO_PIECE;
  int epFile = gameState.getEnPassant();
  if (epFile != NO_EP) {
    Zobrist::flipEpStatus(zobristHash, epFile);
  }
  gameState.setEnPassant(NO_EP);
}
void Position::makeDoublePawnPush(const Move &move) {
  makeQuietMove(move);
  uint file = squareTofile[move.getFrom()];
  Zobrist::flipEpStatus(zobristHash, file);
  gameState.setEnPassant(file);
}
void Position::makeEPCapture(const Move &move) {
  Color turn = gameState.getTurn();
  uint fileEP = getGameState().getEnPassant();

  uint64_t victimMask;
  switch (turn) {

  case WHITE:
    victimMask = 0b1ull << (fileEP + 8 * 4);
    pieces[BLACK][PAWN] ^= victimMask;
    Zobrist::removeAddPiece(zobristHash, fileEP + 8 * 4, PAWN, BLACK);
    mailbox[fileEP + 8 * 4] = NO_PIECE;
    break;

  case BLACK:
    victimMask = 0b1ull << (fileEP + 8 * 3);
    pieces[WHITE][PAWN] ^= victimMask;
    Zobrist::removeAddPiece(zobristHash, fileEP + 8 * 3, PAWN, WHITE);
    mailbox[fileEP + 8 * 3] = NO_PIECE;
    break;
  default:
    break;
  }
  makeQuietMove(move);
}
// doesnt check if castling is legal;
void Position::makeQueenCastle(const Move &move) {
  int turn = getTurn();
  uint castlingRigths = gameState.getCastlingRigths();

  switch (turn) {
  case WHITE:
    pieces[WHITE][ROOK] &= ~(0b1ull << A1);
    Zobrist::removeAddPiece(zobristHash, A1, ROOK, WHITE);
    pieces[WHITE][ROOK] |= (0b1ull << D1);
    Zobrist::removeAddPiece(zobristHash, D1, ROOK, WHITE);

    pieces[WHITE][KING] >>= 2;
    Zobrist::removeAddPiece(zobristHash, C1, KING, WHITE);
    Zobrist::removeAddPiece(zobristHash, e1, KING, WHITE);

    mailbox[A1] = NO_PIECE;
    mailbox[e1] = NO_PIECE;
    mailbox[C1] = KING;
    mailbox[D1] = ROOK;

    if ((castlingRigths & WHITE_KING_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::whiteKingSide);
    }
    if ((castlingRigths & WHITE_QUEEN_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::whiteQueenSide);
    }

    break;

  case BLACK:
    pieces[BLACK][ROOK] &= ~(0b1ull << a8);
    Zobrist::removeAddPiece(zobristHash, a8, ROOK, BLACK);
    pieces[BLACK][ROOK] |= (0b1ull << d8);
    Zobrist::removeAddPiece(zobristHash, d8, ROOK, BLACK);
    pieces[BLACK][KING] >>= 2;
    Zobrist::removeAddPiece(zobristHash, c8, KING, BLACK);
    Zobrist::removeAddPiece(zobristHash, e8, KING, BLACK);
    mailbox[a8] = NO_PIECE;
    mailbox[e8] = NO_PIECE;
    mailbox[c8] = KING;
    mailbox[d8] = ROOK;

    if ((castlingRigths & BLACK_KING_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::blackKingSide);
    }
    if ((castlingRigths & BLACK_QUEEN_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::blackQueenSide);
    }
    break;

  default:
    break;
  }
  int epFile = gameState.getEnPassant();
  if (epFile != NO_EP) {
    Zobrist::flipEpStatus(zobristHash, epFile);
  }
  gameState.setEnPassant(NO_EP);
  gameState.setCastlingRigths(
      castlingRigths & NO_CASTLING_COLOR_MASK_LOOK_UP[gameState.getTurn()]);
}
// doesnt check if castling is legal;
void Position::makeKingCastle(const Move &move) {

  uint castlingRigths = gameState.getCastlingRigths();
  int turn = getTurn();
  switch (turn) {
  case WHITE:
    pieces[WHITE][ROOK] &= ~(0b1ull << h1);
    Zobrist::removeAddPiece(zobristHash, h1, ROOK, WHITE);
    pieces[WHITE][ROOK] |= (0b1ull << f1);
    Zobrist::removeAddPiece(zobristHash, f1, ROOK, WHITE);
    pieces[WHITE][KING] <<= 2;
    Zobrist::removeAddPiece(zobristHash, g1, KING, WHITE);
    Zobrist::removeAddPiece(zobristHash, e1, KING, WHITE);
    mailbox[h1] = NO_PIECE;
    mailbox[e1] = NO_PIECE;
    mailbox[g1] = KING;
    mailbox[f1] = ROOK;

    if ((castlingRigths & WHITE_KING_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::whiteKingSide);
    }
    if ((castlingRigths & WHITE_QUEEN_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::whiteQueenSide);
    }
    break;

  case BLACK:
    pieces[BLACK][ROOK] &= ~(0b1ull << h8);
    Zobrist::removeAddPiece(zobristHash, h8, ROOK, BLACK);
    pieces[BLACK][ROOK] |= (0b1ull << f8);
    Zobrist::removeAddPiece(zobristHash, f8, ROOK, BLACK);
    pieces[BLACK][KING] <<= 2;
    Zobrist::removeAddPiece(zobristHash, g8, KING, WHITE);
    Zobrist::removeAddPiece(zobristHash, e8, KING, WHITE);
    mailbox[h8] = NO_PIECE;
    mailbox[e8] = NO_PIECE;
    mailbox[g8] = KING;
    mailbox[f8] = ROOK;
    if ((castlingRigths & BLACK_KING_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::blackKingSide);
    }
    if ((castlingRigths & BLACK_QUEEN_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::blackQueenSide);
    }
    break;

  default:
    break;
  }
  int epFile = gameState.getEnPassant();
  if (epFile != NO_EP) {
    Zobrist::flipEpStatus(zobristHash, epFile);
  }
  gameState.setEnPassant(NO_EP);
  gameState.setCastlingRigths(
      castlingRigths & NO_CASTLING_COLOR_MASK_LOOK_UP[gameState.getTurn()]);
}

void Position::makePromotion(const Move &move, Piece piece_) {
  // decoding move
  uint from = move.getFrom();
  uint to = move.getTo();
  Piece movingPiece = mailbox[from];
  int oppositePieceColor = getOppositeTurn();

  updateCastlingRights(from, movingPiece);
  // bit masks
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);

  // moving
  pieces[gameState.getTurn()][movingPiece] &= ~fromMask;
  Zobrist::removeAddPiece(zobristHash, from, movingPiece, getTurn());
  capturedInLastMove = NO_PIECE;

  pieces[gameState.getTurn()][piece_] |= toMask;
  Zobrist::removeAddPiece(zobristHash, to, piece_, getTurn());

  // Mailbox operations;
  mailbox[to] = piece_;
  mailbox[from] = NO_PIECE;
  int epFile = gameState.getEnPassant();
  if (epFile != NO_EP) {
    Zobrist::flipEpStatus(zobristHash, epFile);
  }
  gameState.setEnPassant(NO_EP);
}

void Position::makePromoCapture(const Move &move, Piece piece_) {

  // decoding move
  uint from = move.getFrom();
  uint to = move.getTo();
  Piece movingPiece = mailbox[from];
  int oppositePieceColor = getOppositeTurn();
  updateCastlingRights(from, movingPiece);
  // bit masks
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);
  // moving
  pieces[gameState.getTurn()][movingPiece] &= ~fromMask;
  Zobrist::removeAddPiece(zobristHash, from, movingPiece, getTurn());
  Piece capturedPieceType = mailbox[to];
  if (capturedPieceType == ROOK) {
    updateCastlingRights(to, ROOK);
  }
  pieces[oppositePieceColor][capturedPieceType] &= (~toMask);
  Zobrist::removeAddPiece(zobristHash, to, capturedPieceType,
                          getOppositeTurn());

  capturedInLastMove = capturedPieceType;
  pieces[gameState.getTurn()][piece_] |= toMask;
  Zobrist::removeAddPiece(zobristHash, to, piece_, getTurn());
  // Mailbox operations;
  mailbox[to] = piece_;
  mailbox[from] = NO_PIECE;
  int epFile = gameState.getEnPassant();
  if (epFile != NO_EP) {
    Zobrist::flipEpStatus(zobristHash, epFile);
  }
  gameState.setEnPassant(NO_EP);
}
// WARNING: this doesnt handle the case if rook is captured
// TODO: update after castling happens;
void Position::updateCastlingRights(int from, int movingPiece) {

  uint castlingRigths = gameState.getCastlingRigths();
  if (movingPiece == KING) {
    switch (getTurn()) {
    case WHITE:
      if ((castlingRigths & WHITE_KING_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, whiteKingSide);
      }
      if ((castlingRigths & WHITE_QUEEN_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, whiteQueenSide);
      }
      break;
    case BLACK:
      if ((castlingRigths & BLACK_KING_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, blackKingSide);
      }
      if ((castlingRigths & BLACK_QUEEN_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, blackQueenSide);
      }
      break;
    default:
      break;
    }
    gameState.setCastlingRigths(
        castlingRigths & NO_CASTLING_COLOR_MASK_LOOK_UP[gameState.getTurn()]);
  }
  if (movingPiece == ROOK) {
    switch (from) {
    case A1:
      if ((castlingRigths & WHITE_QUEEN_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, whiteQueenSide);
      }
      gameState.setCastlingRigths(castlingRigths &
                                  NO_WHITE_QUEEN_SIDE_CASTLING_MASK);
      break;

    case h1:
      if ((castlingRigths & WHITE_KING_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, whiteKingSide);
      }
      gameState.setCastlingRigths(castlingRigths &
                                  NO_WHITE_KING_SIDE_CASTLING_MASK);
      break;
    case a8:
      if ((castlingRigths & BLACK_QUEEN_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, blackQueenSide);
      }
      gameState.setCastlingRigths(castlingRigths &
                                  NO_BLACK_QUEEN_SIDE_CASTLING_MASK);
      break;

    case h8:
      if ((castlingRigths & BLACK_KING_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, blackKingSide);
      }
      gameState.setCastlingRigths(castlingRigths &
                                  NO_BLACK_KING_SIDE_CASTLING_MASK);
      break;

    default:
      break;
    }
  }
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
std::array<Piece, 64> Position::getMailbox() const { return mailbox; }
std::array<std::array<uint64_t, 6>, 2> Position::getPieces() const {
  return pieces;
}
// Misc
// TODO: this function is hideous refactor it;
void Position::printBoard() const {
  uint64_t allPieces = getAllPieces(WHITE) | getAllPieces(BLACK);
  Piece piece_;
  Color color_;
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
