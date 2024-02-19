#include "constants.hpp"
#include "position.hpp"
class Zobrist {

public:
  // constructors;
  Zobrist() = default;
  // getters;
  static uint64_t generatePieceZobristKey(int piece_, int color_,
                                          const Position &position);
  static uint64_t generateCastlingZobristKey(const Position &position);
  static uint64_t generateTotalZobristKey(const Position &position);
  static uint64_t generateEpZobristKey(const Position &position);

  static uint64_t generateColorZobristKey(const Position &position);
  static void changeTurn(uint64_t &zobristKey);
  static void removeAddPiece(uint64_t &zobristKey, int square_, piece piece_,
                             color color_);
  static void flipEpStatus(uint64_t &zobristKey, int file);
  static void flipCastlingStatus(uint64_t &zobristKey,
                                 castlingType casltingType_);
  // assuming the sequence as;
  // wPx12,wBx12,wRx12,wQx12,wNx12,wPx12,bBx12,bRx12,bQx12,bNx12,sideTomove,castlingRigths,theFileOFValidEnPassant
};