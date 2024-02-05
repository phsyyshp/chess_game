#include "constants.hpp"
#include "position.hpp"
class Zobrist {

public:
  // constructors;
  Zobrist() = default;
  Zobrist(const Position &position_) : position(position_){};
  // getters;
  uint64_t generatePieceZobristKey(int piece_, int color_) const;
  uint64_t generateTotalZobristKey() const;

private:
  Position position;
  // assuming the sequence as;
  // wPx12,wBx12,wRx12,wQx12,wNx12,wPx12,bBx12,bRx12,bQx12,bNx12,sideTomove,castlingRigths,theFileOFValidEnPassant
};