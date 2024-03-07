#include "zobrist.hpp"
// TODO: check all the methods for wrong indexing
// enum Piece { PAWN, BISHOP, ROOK, QUEEN, KNIGHT, KING, NO_PIECE };
uint64_t Zobrist::generatePieceZobristKey(int piece_, int color_,
                                          const Position &position) {

  uint64_t remainingPieces = position.getPieces()[color_][piece_];
  uint64_t out = 0b0ull;
  int square_;
  while (remainingPieces) {
    square_ = __builtin_ctzll(remainingPieces);
    out ^= zobristTable[square_ + piece_ * 64 + color_ * 6 * 64];
    remainingPieces ^= (0b1ull << square_);
  }
  // zobrist table is in form of;
  // wPx64,wBx64,wRx64,wQx64,wNx64,wKx64,bPx64,bBx64,bRx64,bQx64,bNx64,bKx64,sideTomove,castlingRigths,theFileOFValidEnPassant
  //   return zobristTable[piece_ * square_ + 6 * 64 * color_];
  return out;
};

uint64_t Zobrist::generateCastlingZobristKey(const Position &position) {
  // Castling Rights Table
  // +-----------------------+---------------------+
  // |       Permission      |      Castling       |
  // +-----------------------+---------------------+
  // | canWhiteCastleQueenSide | 1 0 0 0 | 8
  // | canWhiteCastleKingSide  | 0 1 0 0 | 4
  // | canBlackCastleQueenSide | 0 0 1 0 | 2
  // | canBlackCastleKingSide  | 0 0 0 1 | 1
  // +-----------------------+---------------------+
  // QKqk
  uint remainingCastlingRigths = position.getGameState().getCastlingRigths();
  if (remainingCastlingRigths == 0) {
    return 0ull;
  }
  uint64_t out = 0b0ull;
  int castlingRights;
  while (remainingCastlingRigths) {
    castlingRights = __builtin_ctzll(remainingCastlingRigths);
    out ^= zobristTable[769 + castlingRights];
    remainingCastlingRigths ^= (0b1ull << castlingRights);
  }
  // zobrist table is in form of;
  // wPx64,wBx64,wRx64,wQx64,wNx64,wKx64,wPx64,bBx64,bRx64,bQx64,bNx64,bKx64,sideTomove,castlingRigths,theFileOFValidEnPassant
  return out;
}
uint64_t Zobrist::generateEpZobristKey(const Position &position) {
  uint epFile = position.getGameState().getEnPassant();
  if (epFile == NO_EP) {
    return 0ull;
  }
  uint64_t out = 0b0ull;
  out = zobristTable[epFile + 773];
  // zobrist table is in form of;
  // wPx64,wBx64,wRx64,wQx64,wNx64,wKx64,wPx64,bBx64,bRx64,bQx64,bNx64,bKx64,sideTomove,castlingRigths,theFileOFValidEnPassant
  return out;
}
uint64_t Zobrist::generateColorZobristKey(const Position &position) {
  if (position.getTurn() != WHITE) {
    return zobristTable[COLOR_INDEX];
  }
  return 0ull;
}
uint64_t Zobrist::generateTotalZobristKey(const Position &position) {
  uint64_t zobristKey = 0b0ull;
  for (int color_ = 0; color_ < 2; color_++) {
    for (int piece_ = 0; piece_ < 6; piece_++)
      zobristKey ^= generatePieceZobristKey(piece_, color_, position);
  }
  zobristKey ^= generateCastlingZobristKey(position) ^
                generateEpZobristKey(position) ^
                generateColorZobristKey(position);
  return zobristKey;
}

void Zobrist::changeTurn(uint64_t &zobristKey) {
  zobristKey ^= zobristTable[COLOR_INDEX];
}
void Zobrist::removeAddPiece(uint64_t &zobristKey, int square_, Piece piece_,
                             Color color_) {
  zobristKey ^= zobristTable[square_ + piece_ * 64 + color_ * 6 * 64];
}

void Zobrist::flipEpStatus(uint64_t &zobristKey, int epFile) {
  zobristKey ^= zobristTable[epFile + 773];
}
void Zobrist::flipCastlingStatus(uint64_t &zobristKey,
                                 castlingType castlingType_) {
  zobristKey ^= zobristTable[769 + castlingType_];
}
