#include "evaluation.hpp"

// getters;
Position Evaluation::getPosition() const { return position; }
int Evaluation::getPieceCount(const piece &piece_, const color &color_) const {
  return __builtin_popcountll(position.getPieces()[color_][piece_]);
}
// Pawn operations;
int Evaluation::getDoubledPawnCount(const color &color_) const {
  switch (color_) {
  case white:
    return __builtin_popcountll(
        whitePawnsInFrontOwn(position.getPieces()[white][pawn]));
  case black:
    return __builtin_popcountll(
        blackPawnsInFrontOwn(position.getPieces()[black][pawn]));
  default:
    std::cerr << color_ << " is not valid color" << std::endl;
    throw std::range_error("invalid color");
    break;
  }
}
// int Evaluation::getBlockedPawnCount(const color &color_) const {}
int Evaluation::getIsolatedPawnCount(const color &color_) const {
  return __builtin_popcountll(isolanis(position.getPieces()[color_][pawn]));
}
size_t Evaluation::getMobility(const color &color_) const {
  MoveGeneration movgen(position);
  movgen.generateAllMoves();
  return movgen.getNumberOfMoves();
};
// Square tables
int Evaluation::pawnSquareTables(const color &color_) const {
  int out = 0;
  int tempSq;
  uint64_t remainingPawns = position.getPieces()[color_][pawn];
  while (remainingPawns) {
    tempSq = __builtin_ctzll(remainingPawns);
    out += pawnSqTbls[color_][tempSq];
    remainingPawns ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::knightSquareTables(const color &color_) const {

  int out = 0;
  int tempSq;
  uint64_t remainingKnights = position.getPieces()[color_][knight];
  while (remainingKnights) {
    tempSq = __builtin_ctzll(remainingKnights);
    out += knightSqTbls[color_][tempSq];
    remainingKnights ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::queenSquareTables(const color &color_) const {

  int out = 0;
  int tempSq;
  uint64_t remainingQueens = position.getPieces()[color_][queen];
  while (remainingQueens) {
    tempSq = __builtin_ctzll(remainingQueens);
    out += queenSqTbls[color_][tempSq];
    remainingQueens ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::rookSquareTables(const color &color_) const {
  int out = 0;
  int tempSq;
  uint64_t remainingRooks = position.getPieces()[color_][rook];
  while (remainingRooks) {
    tempSq = __builtin_ctzll(remainingRooks);
    out += rookSqTbls[color_][tempSq];
    remainingRooks ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::bishopSquareTables(const color &color_) const {
  int out = 0;
  int tempSq;
  uint64_t remainingBishops = position.getPieces()[color_][bishop];
  while (remainingBishops) {
    tempSq = __builtin_ctzll(remainingBishops);
    out += bishopSqTbls[color_][tempSq];
    remainingBishops ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::kingSquareTables(const color &color_) const {

  int out = 0;
  int tempSq;
  uint64_t kingMask = position.getPieces()[color_][king];
  tempSq = __builtin_ctzll(kingMask);
  if (position.getPly() >= 20 && position.getPly() <= 80) {

    out = middleGameKingSqTbls[color_][tempSq];
  } else if (position.getPly() > 80) {
    out = endGameKingSqTbls[color_][tempSq];
  }
  return out;
}
int Evaluation::pieceSquareTables() const {
  return rookSquareTables(white) - rookSquareTables(black) +
         pawnSquareTables(white) - pawnSquareTables(black) +
         bishopSquareTables(white) - bishopSquareTables(black) +
         queenSquareTables(white) - queenSquareTables(black) +
         knightSquareTables(white) - knightSquareTables(black);
  //  kingSquareTables(white) - kingSquareTables(black);
}
int Evaluation::evaluate() const {
  int kingDiff = getPieceCount(king, white) - getPieceCount(king, black);
  int queenDiff = getPieceCount(queen, white) - getPieceCount(queen, black);
  int knightDiff = getPieceCount(knight, white) - getPieceCount(knight, black);
  int rookDiff = getPieceCount(rook, white) - getPieceCount(rook, black);
  int bishopDiff = getPieceCount(bishop, white) - getPieceCount(bishop, black);
  int pawnDiff = getPieceCount(pawn, white) - getPieceCount(pawn, black);
  int doubledPawnDiff = getDoubledPawnCount(white) - getDoubledPawnCount(black);
  int isolinDiff = getIsolatedPawnCount(white) - getIsolatedPawnCount(black);
  // size_t mobilityDiff = getMobility(white) - getMobility(black);

  return (20000 * kingDiff + 900 * queenDiff + 500 * rookDiff +
          300 * (bishopDiff + knightDiff) + 100 * pawnDiff -
          50 * (isolinDiff + doubledPawnDiff) + pieceSquareTables()) *
         whoToMove[position.getTurn()];
};