#include "position.hpp"
// Constructors;
Position::Position(const std::string &FENstr) {
  std::vector<std::string> fieldVec = FENtoFields(FENstr);
  pieces = FENtoPieces(fieldVec[0]);
  gameState = FENtoGameStateInt(FENstr);
  turn = gameState.getTurn();
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
  occupanciesOfColor[WHITE] = pieces[WHITE][ROOK] | pieces[WHITE][KNIGHT] |
                              pieces[WHITE][BISHOP] | pieces[WHITE][QUEEN] |
                              pieces[WHITE][KING] | pieces[WHITE][PAWN];
  occupanciesOfColor[BLACK] = pieces[BLACK][ROOK] | pieces[BLACK][KNIGHT] |
                              pieces[BLACK][BISHOP] | pieces[BLACK][QUEEN] |
                              pieces[BLACK][KING] | pieces[BLACK][PAWN];

  occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];
  zobristHash = Zobrist::generateTotalZobristKey(*this);
}
Position::Position(const std::array<std::array<uint64_t, 6>, 2> &pieces_,
                   Color turn_)
    : pieces(pieces_), gameState(turn_) {
  turn = turn_;
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
  occupanciesOfColor[WHITE] = pieces[WHITE][ROOK] | pieces[WHITE][KNIGHT] |
                              pieces[WHITE][BISHOP] | pieces[WHITE][QUEEN] |
                              pieces[WHITE][KING] | pieces[WHITE][PAWN];
  occupanciesOfColor[BLACK] = pieces[BLACK][ROOK] | pieces[BLACK][KNIGHT] |
                              pieces[BLACK][BISHOP] | pieces[BLACK][QUEEN] |
                              pieces[BLACK][KING] | pieces[BLACK][PAWN];

  occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];
  zobristHash = Zobrist::generateTotalZobristKey(*this);
}
// Setters;
void Position::setWhitePiecesToInitialConfiguration() {
  pieces[WHITE][ROOK] = 0b1ULL << A1 | 0b1ULL << H1;
  pieces[WHITE][KNIGHT] = 0b1ULL << G1 | 0b1ULL << B1;
  pieces[WHITE][BISHOP] = 0b1ULL << F1 | 0b1ULL << C1;
  pieces[WHITE][QUEEN] = 0b1ULL << D1;
  pieces[WHITE][KING] = 0b1ULL << E1;
  pieces[WHITE][PAWN] = 0b11111111ULL << 8;
  mailbox[A1] = ROOK;
  mailbox[H1] = ROOK;
  mailbox[G1] = KNIGHT;
  mailbox[B1] = KNIGHT;
  mailbox[F1] = BISHOP;
  mailbox[C1] = BISHOP;
  mailbox[D1] = QUEEN;
  mailbox[E1] = KING;
  mailbox[A2] = PAWN;
  mailbox[B2] = PAWN;
  mailbox[C2] = PAWN;
  mailbox[D2] = PAWN;
  mailbox[E2] = PAWN;
  mailbox[F2] = PAWN;
  mailbox[G2] = PAWN;
  mailbox[H2] = PAWN;
}
void Position::setBlackPiecesToInitialConfiguration() {
  pieces[BLACK][ROOK] = 0b1ULL << A8 | 0b1ULL << H8;
  pieces[BLACK][KNIGHT] = 0b1ULL << G8 | 0b1ULL << B8;
  pieces[BLACK][BISHOP] = 0b1ULL << F8 | 0b1ULL << C8;
  pieces[BLACK][QUEEN] = 0b1ULL << D8;
  pieces[BLACK][KING] = 0b1ULL << E8;
  pieces[BLACK][PAWN] = 0b11111111ULL << 6 * 8;
  mailbox[A8] = ROOK;
  mailbox[H8] = ROOK;
  mailbox[G8] = KNIGHT;
  mailbox[B8] = KNIGHT;
  mailbox[F8] = BISHOP;
  mailbox[C8] = BISHOP;
  mailbox[D8] = QUEEN;
  mailbox[E8] = KING;
  mailbox[A7] = PAWN;
  mailbox[B7] = PAWN;
  mailbox[C7] = PAWN;
  mailbox[D7] = PAWN;
  mailbox[E7] = PAWN;
  mailbox[F7] = PAWN;
  mailbox[G7] = PAWN;
  mailbox[H7] = PAWN;
}
void Position::setBoardToInitialConfiguration() {
  clear();
  setWhitePiecesToInitialConfiguration();
  setBlackPiecesToInitialConfiguration();
  occupanciesOfColor[WHITE] = pieces[WHITE][ROOK] | pieces[WHITE][KNIGHT] |
                              pieces[WHITE][BISHOP] | pieces[WHITE][QUEEN] |
                              pieces[WHITE][KING] | pieces[WHITE][PAWN];
  occupanciesOfColor[BLACK] = pieces[BLACK][ROOK] | pieces[BLACK][KNIGHT] |
                              pieces[BLACK][BISHOP] | pieces[BLACK][QUEEN] |
                              pieces[BLACK][KING] | pieces[BLACK][PAWN];

  occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];

  zobristHash = Zobrist::generateTotalZobristKey(*this);
  positionHistory[0] = zobristHash;
}
void Position::clear() {
  mailbox.fill(NO_PIECE);
  pieces[WHITE].fill(0);
  pieces[BLACK].fill(0);
  ply = 0;
  gameState.clear();
  zobristHash = 0ull;
  turn = WHITE;
  occupancy = 0ull;
  occupanciesOfColor = {0ull, 0ull};
  capturedInLastMove = NO_PIECE;
  positionHistory.fill(0ull);
}
void Position::changeTurn() {
  turn = OPPOSITE_COLOR[turn];
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
  occupanciesOfColor = rhs.occupanciesOfColor;
  occupancy = rhs.occupancy;
  turn = rhs.turn;
  positionHistory = rhs.positionHistory;
  return *this;
}
// Getters;
Color Position ::getPieceColor(const uint64_t &sqMask) const {
  if (sqMask & getAllPieces(BLACK)) {
    return Color::BLACK;
  }
  if (sqMask & getAllPieces(WHITE)) {
    return Color::WHITE;
  }
  return Color::INVALID;
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
const Color &Position::getTurn() const { return turn; }
Color Position::getOppositeTurn() const { return OPPOSITE_COLOR[turn]; }
const GameState &Position::getGameState() const { return gameState; }
const uint64_t &Position::getAllPieces(const Color &color_) const {
  return occupanciesOfColor[color_];
}
const uint64_t &Position::getOccupancy() const { return occupancy; }
Piece Position::getCapturedInLastMove() const { return capturedInLastMove; }
uint64_t Position::getAttacksToKing() const {

  Color oppositeColor = getOppositeTurn();
  Square squareOfKing =
      static_cast<Square>(__builtin_ctzll(pieces[turn][KING]));
  uint64_t oppositePawns = pieces[oppositeColor][PAWN];
  uint64_t oppositeKnights = pieces[oppositeColor][KNIGHT];
  uint64_t oppositeRooks = pieces[oppositeColor][ROOK];
  uint64_t oppositeBishops = pieces[oppositeColor][BISHOP];
  uint64_t oppositeQueens = pieces[oppositeColor][QUEEN];
  uint64_t oppositeKing = pieces[oppositeColor][KING];
  uint64_t oppositeRookQueens = oppositeRooks | oppositeQueens;
  uint64_t oppositeBishopQueens = oppositeBishops | oppositeQueens;
  return (knightLookUpTable[squareOfKing] & oppositeKnights) |
         (pawnLookUpTable[turn][squareOfKing] & oppositePawns) |
         (getBishopAttackMask(squareOfKing, occupancy) & oppositeBishopQueens) |
         (getRookAttackMask(squareOfKing, occupancy) & oppositeRookQueens) |
         (kingLookUpTable[squareOfKing] & oppositeKing);
}

uint64_t Position::getAttacksToSquare(Square square_) const {

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
         (pawnLookUpTable[turn][victimSq] & oppositePawns) |
         (getBishopAttackMask(victimSq, occupancy) & oppositeBishopQueens) |
         (getRookAttackMask(victimSq, occupancy) & oppositeRookQueens) |
         (kingLookUpTable[victimSq] & oppositeKing);
}
// bools
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
  positionHistory[ply] = zobristHash;
  return isLegal;
}

// it does not change turn automatically;
void Position::makeQuietMove(const Move &move) {

  // decoding move
  uint from = move.getFrom();
  uint to = move.getTo();
  Piece movingPiece = mailbox[from];

  updateCastlingRights(from, movingPiece);
  // bit masks
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);

  // moving
  pieces[turn][movingPiece] &= ~fromMask;
  Zobrist::removeAddPiece(zobristHash, from, movingPiece, turn);
  capturedInLastMove = NO_PIECE;

  pieces[turn][movingPiece] |= toMask;
  Zobrist::removeAddPiece(zobristHash, to, movingPiece, turn);
  // Mailbox operations;
  mailbox[to] = mailbox[from];
  mailbox[from] = NO_PIECE;
  // Occupancy operations
  occupanciesOfColor[turn] &= ~fromMask;
  occupanciesOfColor[turn] |= toMask;
  occupancy &= ~fromMask;
  occupancy |= toMask;
  // En Passant operations
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
  pieces[turn][movingPiece] &= ~fromMask;
  Zobrist::removeAddPiece(zobristHash, from, movingPiece, turn);
  Piece capturedPieceType = mailbox[to];
  if (capturedPieceType == ROOK) {
    updateCastlingRights(to, ROOK);
  }
  pieces[oppositePieceColor][capturedPieceType] &= (~toMask);
  Zobrist::removeAddPiece(zobristHash, to, capturedPieceType,
                          getOppositeTurn());
  capturedInLastMove = capturedPieceType;
  pieces[turn][movingPiece] |= toMask;
  Zobrist::removeAddPiece(zobristHash, to, movingPiece, turn);
  // Occupancy operations
  occupanciesOfColor[turn] &= ~fromMask;
  occupanciesOfColor[turn] |= toMask;
  occupanciesOfColor[oppositePieceColor] &= ~toMask;
  occupancy &= ~fromMask;
  // Mailbox operations;
  mailbox[to] = mailbox[from];
  mailbox[from] = NO_PIECE;
  // En Passant operations
  int epFile = gameState.getEnPassant();
  if (epFile != NO_EP) {
    Zobrist::flipEpStatus(zobristHash, epFile);
  }
  gameState.setEnPassant(NO_EP);
}
void Position::makeDoublePawnPush(const Move &move) {
  makeQuietMove(move);
  uint file = SQUARE_TO_FILE[move.getFrom()];
  Zobrist::flipEpStatus(zobristHash, file);
  gameState.setEnPassant(file);
}
void Position::makeEPCapture(const Move &move) {
  uint fileEP = getGameState().getEnPassant();

  uint64_t victimMask;
  switch (turn) {

  case WHITE:
    victimMask = 0b1ull << (fileEP + 8 * 4);
    pieces[BLACK][PAWN] ^= victimMask;
    Zobrist::removeAddPiece(zobristHash, fileEP + 8 * 4, PAWN, BLACK);
    mailbox[fileEP + 8 * 4] = NO_PIECE;
    occupanciesOfColor[BLACK] ^= victimMask;
    occupancy ^= victimMask;
    break;

  case BLACK:
    victimMask = 0b1ull << (fileEP + 8 * 3);
    pieces[WHITE][PAWN] ^= victimMask;
    Zobrist::removeAddPiece(zobristHash, fileEP + 8 * 3, PAWN, WHITE);
    mailbox[fileEP + 8 * 3] = NO_PIECE;
    occupanciesOfColor[WHITE] ^= victimMask;
    occupancy ^= victimMask;
    break;
  default:
    break;
  }
  makeQuietMove(move);
}
// doesnt check if castling is legal;
void Position::makeQueenCastle(const Move &move) {
  uint castlingRigths = gameState.getCastlingRigths();

  switch (turn) {
  case WHITE:
    pieces[WHITE][ROOK] &= ~(0b1ull << A1);
    occupanciesOfColor[WHITE] &= ~(0b1ull << A1);

    Zobrist::removeAddPiece(zobristHash, A1, ROOK, WHITE);
    pieces[WHITE][ROOK] |= (0b1ull << D1);
    occupanciesOfColor[WHITE] |= (0b1ull << D1);

    Zobrist::removeAddPiece(zobristHash, D1, ROOK, WHITE);

    pieces[WHITE][KING] >>= 2;
    occupanciesOfColor[WHITE] &= ~(0b1ull << E1);
    occupanciesOfColor[WHITE] |= (0b1ull << C1);

    Zobrist::removeAddPiece(zobristHash, C1, KING, WHITE);
    Zobrist::removeAddPiece(zobristHash, E1, KING, WHITE);

    mailbox[A1] = NO_PIECE;
    mailbox[E1] = NO_PIECE;
    mailbox[C1] = KING;
    mailbox[D1] = ROOK;
    occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];

    if ((castlingRigths & WHITE_KING_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::WHITE_KING_SIDE);
    }
    if ((castlingRigths & WHITE_QUEEN_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::WHITE_QUEEN_SIDE);
    }

    break;

  case BLACK:
    pieces[BLACK][ROOK] &= ~(0b1ull << A8);
    occupanciesOfColor[BLACK] &= ~(0b1ull << A8);
    Zobrist::removeAddPiece(zobristHash, A8, ROOK, BLACK);
    pieces[BLACK][ROOK] |= (0b1ull << D8);
    occupanciesOfColor[BLACK] |= (0b1ull << D8);

    Zobrist::removeAddPiece(zobristHash, D8, ROOK, BLACK);
    pieces[BLACK][KING] >>= 2;
    occupanciesOfColor[BLACK] &= ~(0b1ull << E8);
    occupanciesOfColor[BLACK] |= (0b1ull << C8);
    Zobrist::removeAddPiece(zobristHash, C8, KING, BLACK);
    Zobrist::removeAddPiece(zobristHash, E8, KING, BLACK);
    mailbox[A8] = NO_PIECE;
    mailbox[E8] = NO_PIECE;
    mailbox[C8] = KING;
    mailbox[D8] = ROOK;
    occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];

    if ((castlingRigths & BLACK_KING_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::BLACK_KING_SIDE);
    }
    if ((castlingRigths & BLACK_QUEEN_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::BLACK_QUEEN_SIDE);
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
    pieces[WHITE][ROOK] &= ~(0b1ull << H1);
    occupanciesOfColor[WHITE] &= ~(0b1ull << H1);
    Zobrist::removeAddPiece(zobristHash, H1, ROOK, WHITE);
    pieces[WHITE][ROOK] |= (0b1ull << F1);
    occupanciesOfColor[WHITE] |= (0b1ull << F1);
    Zobrist::removeAddPiece(zobristHash, F1, ROOK, WHITE);
    pieces[WHITE][KING] <<= 2;
    occupanciesOfColor[WHITE] &= ~(0b1ull << E1);
    occupanciesOfColor[WHITE] |= (0b1ull << G1);
    Zobrist::removeAddPiece(zobristHash, G1, KING, WHITE);
    Zobrist::removeAddPiece(zobristHash, E1, KING, WHITE);
    mailbox[H1] = NO_PIECE;
    mailbox[E1] = NO_PIECE;
    mailbox[G1] = KING;
    mailbox[F1] = ROOK;
    occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];

    if ((castlingRigths & WHITE_KING_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::WHITE_KING_SIDE);
    }
    if ((castlingRigths & WHITE_QUEEN_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::WHITE_QUEEN_SIDE);
    }
    break;

  case BLACK:
    pieces[BLACK][ROOK] &= ~(0b1ull << H8);
    occupanciesOfColor[BLACK] &= ~(0b1ull << H8);
    Zobrist::removeAddPiece(zobristHash, H8, ROOK, BLACK);
    pieces[BLACK][ROOK] |= (0b1ull << F8);
    occupanciesOfColor[BLACK] |= (0b1ull << F8);

    Zobrist::removeAddPiece(zobristHash, F8, ROOK, BLACK);
    pieces[BLACK][KING] <<= 2;
    occupanciesOfColor[BLACK] &= ~(0b1ull << E8);
    occupanciesOfColor[BLACK] |= (0b1ull << G8);
    Zobrist::removeAddPiece(zobristHash, G8, KING, BLACK);
    Zobrist::removeAddPiece(zobristHash, E8, KING, BLACK);
    mailbox[H8] = NO_PIECE;
    mailbox[E8] = NO_PIECE;
    mailbox[G8] = KING;
    mailbox[F8] = ROOK;
    occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];

    if ((castlingRigths & BLACK_KING_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::BLACK_KING_SIDE);
    }
    if ((castlingRigths & BLACK_QUEEN_SIDE_CASTLING_MASK) != 0) {
      Zobrist::flipCastlingStatus(zobristHash, castlingType::BLACK_QUEEN_SIDE);
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

  updateCastlingRights(from, movingPiece);
  // bit masks
  uint64_t toMask = (0b1ull << to);
  uint64_t fromMask = (0b1ull << from);

  // moving
  pieces[turn][movingPiece] &= ~fromMask;
  occupanciesOfColor[turn] &= ~fromMask;
  Zobrist::removeAddPiece(zobristHash, from, movingPiece, getTurn());
  capturedInLastMove = NO_PIECE;

  pieces[turn][piece_] |= toMask;
  occupanciesOfColor[turn] |= toMask;
  occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];
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
  pieces[turn][movingPiece] &= ~fromMask;
  occupanciesOfColor[turn] &= ~fromMask;

  Zobrist::removeAddPiece(zobristHash, from, movingPiece, getTurn());
  Piece capturedPieceType = mailbox[to];
  if (capturedPieceType == ROOK) {
    updateCastlingRights(to, ROOK);
  }
  pieces[oppositePieceColor][capturedPieceType] &= (~toMask);
  occupanciesOfColor[oppositePieceColor] &= ~toMask;

  Zobrist::removeAddPiece(zobristHash, to, capturedPieceType,
                          getOppositeTurn());

  capturedInLastMove = capturedPieceType;
  pieces[turn][piece_] |= toMask;
  occupanciesOfColor[turn] |= toMask;

  occupancy = occupanciesOfColor[WHITE] | occupanciesOfColor[BLACK];
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
        Zobrist::flipCastlingStatus(zobristHash, WHITE_KING_SIDE);
      }
      if ((castlingRigths & WHITE_QUEEN_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, WHITE_QUEEN_SIDE);
      }
      break;
    case BLACK:
      if ((castlingRigths & BLACK_KING_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, BLACK_KING_SIDE);
      }
      if ((castlingRigths & BLACK_QUEEN_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, BLACK_QUEEN_SIDE);
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
        Zobrist::flipCastlingStatus(zobristHash, WHITE_QUEEN_SIDE);
      }
      gameState.setCastlingRigths(castlingRigths &
                                  NO_WHITE_QUEEN_SIDE_CASTLING_MASK);
      break;

    case H1:
      if ((castlingRigths & WHITE_KING_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, WHITE_KING_SIDE);
      }
      gameState.setCastlingRigths(castlingRigths &
                                  NO_WHITE_KING_SIDE_CASTLING_MASK);
      break;
    case A8:
      if ((castlingRigths & BLACK_QUEEN_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, BLACK_QUEEN_SIDE);
      }
      gameState.setCastlingRigths(castlingRigths &
                                  NO_BLACK_QUEEN_SIDE_CASTLING_MASK);
      break;

    case H8:
      if ((castlingRigths & BLACK_KING_SIDE_CASTLING_MASK) != 0) {
        Zobrist::flipCastlingStatus(zobristHash, BLACK_KING_SIDE);
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
const std::array<Piece, 64> &Position::getMailbox() const { return mailbox; }
const std::array<std::array<uint64_t, 6>, 2> &Position::getPieces() const {
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

bool Position::isThreeFoldRep() const {
  int repetition = 0;
  for (int i = ply - 2; i >= ply - 8; i -= 2) {
    if (positionHistory[i] == zobristHash) {
      repetition++;
    }
    if (repetition == 2) {
      return true;
    }
  }
  return false;
  // return std::count(positionHistory.begin(), positionHistory.begin() + ply +
  // 1,
  //                   zobristHash) >= 3;
}