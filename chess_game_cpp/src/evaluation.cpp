#include "evaluation.hpp"

// getters;
Position Evaluation::getPosition() const { return position; }
int Evaluation::getPieceCount(const piece &pieceType,
                              const color &pieceColor) const {
  return __builtin_popcountll(position.getPieces()[pieceColor][pieceType]);
}
// Pawn operations;
int Evaluation::getDoubledPawnCount(const color &pieceColor) const {
  switch (pieceColor) {
  case white:
    return __builtin_popcountll(
        whitePawnsInFrontOwn(position.getPieces()[white][pawn]));
  case black:
    return __builtin_popcountll(
        blackPawnsInFrontOwn(position.getPieces()[black][pawn]));
  default:
    break;
  }
}
int Evaluation::getBlockedPawnCount(const color &pieceColor) const {}
int Evaluation::getIsolatedPawnCount(const color &pieceColor) const {
  return __builtin_popcountll(isolanis(position.getPieces()[pieceColor][pawn]));
}
size_t Evaluation::getMobility(const color &pieceColor) const {

  MoveGeneration movgen;
  movgen.generateAllMoves(position, pieceColor);
  return movgen.getNumberOfMoves();
};
// Square tables
int Evaluation::pawnSquareTables(const color &pieceColor) const {
  int out = 0;
  int tempSq;
  uint64_t remainingPawns = position.getPieces()[pieceColor][pawn];
  while (remainingPawns) {
    tempSq = __builtin_ctzll(remainingPawns);
    out += pawnSqTbls[pieceColor][tempSq];
    remainingPawns ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::knightSquareTables(const color &pieceColor) const {

  int out = 0;
  int tempSq;
  uint64_t remainingKnights = position.getPieces()[pieceColor][knight];
  while (remainingKnights) {
    tempSq = __builtin_ctzll(remainingKnights);
    out += knightSqTbls[pieceColor][tempSq];
    remainingKnights ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::queenSquareTables(const color &pieceColor) const {

  int out = 0;
  int tempSq;
  uint64_t remainingQueens = position.getPieces()[pieceColor][queen];
  while (remainingQueens) {
    tempSq = __builtin_ctzll(remainingQueens);
    out += queenSqTbls[pieceColor][tempSq];
    remainingQueens ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::rookSquareTables(const color &pieceColor) const {
  int out = 0;
  int tempSq;
  uint64_t remainingRooks = position.getPieces()[pieceColor][rook];
  while (remainingRooks) {
    tempSq = __builtin_ctzll(remainingRooks);
    out += rookSqTbls[pieceColor][tempSq];
    remainingRooks ^= (0b1ull << tempSq);
  }
  return out;
}
int Evaluation::bishopSquareTables(const color &pieceColor) const {
  int out = 0;
  int tempSq;
  uint64_t remainingBishops = position.getPieces()[pieceColor][bishop];
  while (remainingBishops) {
    tempSq = __builtin_ctzll(remainingBishops);
    out += bishopSqTbls[pieceColor][tempSq];
    remainingBishops ^= (0b1ull << tempSq);
  }
  return out;
}
float Evaluation::pieceSquareTables() const {
  return rookSquareTables(white) - rookSquareTables(black) +
         pawnSquareTables(white) - pawnSquareTables(black) +
         bishopSquareTables(white) - bishopSquareTables(black) +
         queenSquareTables(white) - queenSquareTables(black) +
         knightSquareTables(white) - knightSquareTables(black);
}
float Evaluation::evaluate() const {
  int kingDiff = getPieceCount(king, white) - getPieceCount(king, black);
  int queenDiff = getPieceCount(queen, white) - getPieceCount(queen, black);
  int knightDiff = getPieceCount(knight, white) - getPieceCount(knight, black);
  int rookDiff = getPieceCount(rook, white) - getPieceCount(rook, black);
  int bishopDiff = getPieceCount(bishop, white) - getPieceCount(bishop, black);
  int pawnDiff = getPieceCount(pawn, white) - getPieceCount(pawn, black);
  int doubledPawnDiff = getDoubledPawnCount(white) - getDoubledPawnCount(black);
  int isolinDiff = getIsolatedPawnCount(white) - getIsolatedPawnCount(black);
  size_t mobilityDiff = getMobility(white) - getMobility(black);
  std::cout << pieceSquareTables() << std::endl;
  // return (200 * kingDiff + 9 * queenDiff + 5 * rookDiff +
  //         3 * (bishopDiff + knightDiff) + pawnDiff -
  //         0.5 * (isolinDiff + doubledPawnDiff) + 0.1 * (mobilityDiff)) *
  //        whoToMove[position.getTurn()];
  // std::cout << (200 * kingDiff + 9 * queenDiff + 5 * rookDiff +
  //               3 * (bishopDiff + knightDiff) + pawnDiff -
  //               0.5 * (isolinDiff + doubledPawnDiff) +
  //               pieceSquareTables())
  //           << "\n";
  return (200 * kingDiff + 9 * queenDiff + 5 * rookDiff +
          3 * (bishopDiff + knightDiff) + pawnDiff -
          0.5 * (isolinDiff + doubledPawnDiff) + pieceSquareTables() / 100) *
         whoToMove[position.getTurn()];
};