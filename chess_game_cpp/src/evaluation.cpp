#include "evaluation.hpp"

Position Evaluation::getPosition() const {}
int Evaluation::getPieceCount(const piece &pieceType,
                              const piece &pieceColor) const {
  return __builtin_popcountll(position.getPieces()[pieceColor][pieceType]);
}
int Evaluation::getDoubledPawnCount(const color &pieceColor) const {}
int Evaluation::getBlockedPawnCount(const color &pieceColor) const {}
int Evaluation::getIsolatedPawnCount(const color &pieceColor) const {
  uint64_t remainingPawns = position.getPieces()[pieceColor][pawn];
  square tempsq;
  while (remainingPawns) {
    tempsq = static_cast<square>((remainingPawns));
    if (~isPawnIsolated(tempsq, pieceColor)) {
      remainingPawns =
          remainingPawns & adjacentFiles[tempsq] & sqToFiles[tempsq];
    }
  }
}
int Evaluation::getMobility(const color &pieceColor) const {}
bool Evaluation::isPawnDoubled(square sq) const {};
bool Evaluation::isPawnIsolated(square sq, const color &pawnColor) const {
  return (adjacentFiles[sq] & position.getPieces()[pawnColor][pawn]) == 0;
};
bool Evaluation::isPawnBlocked(square sq, const color &pawnColor) const {};
