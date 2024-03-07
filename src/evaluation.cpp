#include "evaluation.hpp"

// getters;
Position Evaluation::getPosition() const { return position; }
// FIX IT: be sure if they are working correctly
int Evaluation::getPieceCount(const Piece &piece_, const Color &color_) const {
  return __builtin_popcountll(position.getPieces()[color_][piece_]);
}
// Pawn operations;
int Evaluation::getDoubledPawnCount(const Color &color_) const {
  switch (color_) {
  case WHITE:
    return __builtin_popcountll(
        whitePawnsInFrontOwn(position.getPieces()[WHITE][PAWN]));
  case BLACK:
    return __builtin_popcountll(
        blackPawnsInFrontOwn(position.getPieces()[BLACK][PAWN]));
  default:
    std::cerr << color_ << " is not valid color" << std::endl;
    throw std::range_error("invalid color");
    break;
  }
}
// int Evaluation::getBlockedPawnCount(const color &color_) const {}
int Evaluation::getIsolatedPawnCount(const Color &color_) const {
  return __builtin_popcountll(isolanis(position.getPieces()[color_][PAWN]));
}
size_t Evaluation::getMobility(const Color &color_) const {
  MoveGeneration movgen(position);
  movgen.generateAllMoves();
  return movgen.getNumberOfMoves();
};
// Square tables
int Evaluation::pawnSquareTables(const Color &color_) const {
  int out = 0;
  int tempSq;
  uint64_t remainingPawns = position.getPieces()[color_][PAWN];
  while (remainingPawns) {
    tempSq = __builtin_ctzll(remainingPawns);
    out += pawnSqTbls[color_][tempSq];
    remainingPawns ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::knightSquareTables(const Color &color_) const {

  int out = 0;
  int tempSq;
  uint64_t remainingKnights = position.getPieces()[color_][KNIGHT];
  while (remainingKnights) {
    tempSq = __builtin_ctzll(remainingKnights);
    out += knightSqTbls[color_][tempSq];
    remainingKnights ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::queenSquareTables(const Color &color_) const {

  int out = 0;
  int tempSq;
  uint64_t remainingQueens = position.getPieces()[color_][QUEEN];
  while (remainingQueens) {
    tempSq = __builtin_ctzll(remainingQueens);
    out += queenSqTbls[color_][tempSq];
    remainingQueens ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::rookSquareTables(const Color &color_) const {
  int out = 0;
  int tempSq;
  uint64_t remainingRooks = position.getPieces()[color_][ROOK];
  while (remainingRooks) {
    tempSq = __builtin_ctzll(remainingRooks);
    out += rookSqTbls[color_][tempSq];
    remainingRooks ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::bishopSquareTables(const Color &color_) const {
  int out = 0;
  int tempSq;
  uint64_t remainingBishops = position.getPieces()[color_][BISHOP];
  while (remainingBishops) {
    tempSq = __builtin_ctzll(remainingBishops);
    out += bishopSqTbls[color_][tempSq];
    remainingBishops ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::kingSquareTables(const Color &color_) const {

  int out = 0;
  int tempSq;
  uint64_t kingMask = position.getPieces()[color_][KING];
  tempSq = __builtin_ctzll(kingMask);
  if (position.getPly() >= 20 && position.getPly() <= 80) {

    out = middleGameKingSqTbls[color_][tempSq];
  } else if (position.getPly() > 80) {
    out = endGameKingSqTbls[color_][tempSq];
  }
  return out;
}
int Evaluation::pieceSquareTables() const {
  return rookSquareTables(WHITE) - rookSquareTables(BLACK) +
         pawnSquareTables(WHITE) - pawnSquareTables(BLACK) +
         bishopSquareTables(WHITE) - bishopSquareTables(BLACK) +
         queenSquareTables(WHITE) - queenSquareTables(BLACK) +
         knightSquareTables(WHITE) - knightSquareTables(BLACK);
  //  kingSquareTables(white) - kingSquareTables(black);
}
// be carefull with value overflow;
int16_t Evaluation::evaluate() const {
  int16_t queenDiff = getPieceCount(QUEEN, WHITE) - getPieceCount(QUEEN, BLACK);
  int16_t knightDiff =
      getPieceCount(KNIGHT, WHITE) - getPieceCount(KNIGHT, BLACK);
  int16_t rookDiff = getPieceCount(ROOK, WHITE) - getPieceCount(ROOK, BLACK);
  int16_t bishopDiff =
      getPieceCount(BISHOP, WHITE) - getPieceCount(BISHOP, BLACK);
  int16_t pawnDiff = getPieceCount(PAWN, WHITE) - getPieceCount(PAWN, BLACK);
  int16_t doubledPawnDiff =
      getDoubledPawnCount(WHITE) - getDoubledPawnCount(BLACK);
  int16_t isolinDiff =
      getIsolatedPawnCount(WHITE) - getIsolatedPawnCount(BLACK);
  // size_t mobilityDiff = getMobility(white) - getMobility(black);

  return (900 * queenDiff + 500 * rookDiff + 300 * (bishopDiff + knightDiff) +
          100 * pawnDiff - 50 * (isolinDiff + doubledPawnDiff) +
          pieceSquareTables()) *
         WHO_TO_MOVE[position.getTurn()];
};