#include "zobrist.hpp"

uint64_t Zobrist::generatePieceZobristKey(int piece_, int color_) const {

  uint64_t remainingPieces = position.getPieces()[color_][piece_];
  uint64_t out = 0b0ull;
  int square_;
  while (remainingPieces) {
    square_ = __builtin_ctzll(remainingPieces);
    out |= zobristTable[square_ * (piece_ + 1) + 6 * 64 * color_];
    remainingPieces ^= (0b1ull << square_);
  }
  // zobrist tabloe is in form of;
  // wPx64,wBx64,wRx64,wQx64,wNx64,wKx64,wPx64,bBx64,bRx64,bQx64,bNx64,bKx64,sideTomove,castlingRigths,theFileOFValidEnPassant
  //   return zobristTable[piece_ * square_ + 6 * 64 * color_];
  return out;
  return zobristTable[piece_ * square_ + 6 * 64 * color_];
};

uint64_t Zobrist::generateTotalZobristKey() const {
  uint64_t zobristKey = 0b0ull;
  for (int color_ = 0; color_ < 2; color_++) {
    for (int piece_ = 0; piece_ < 6; piece_++)
      zobristKey |= generatePieceZobristKey(piece_, color_);
  }
}
