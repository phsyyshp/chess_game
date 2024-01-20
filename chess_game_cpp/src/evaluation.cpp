#include "evaluation.hpp"

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
// TODO: finish this impolementation;
int Evaluation::pieceSquareTables(const color &pieceColor) const {

  int pieceSqVal;
  for (int clr = 0; clr < 2; clr++) {
    for (int sq = 0; sq < 64; sq++) {

      pieceVal +=
    }
  }

  return 0;
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
  // return (200 * kingDiff + 9 * queenDiff + 5 * rookDiff +
  //         3 * (bishopDiff + knightDiff) + pawnDiff -
  //         0.5 * (isolinDiff + doubledPawnDiff) + 0.1 * (mobilityDiff)) *
  //        whoToMove[position.getTurn()];
  return (200 * kingDiff + 9 * queenDiff + 5 * rookDiff +
          3 * (bishopDiff + knightDiff) + pawnDiff -
          0.5 * (isolinDiff + doubledPawnDiff)) *
         whoToMove[position.getTurn()];
};