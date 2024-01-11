#include "loader.hpp"
class Move {
public:
  Move(square from, square to, piece pieceType, color colorIn)
      : moveNum(from | (to << 6) | (pieceType << 12) | (colorIn << 18)){};
  int getTo();
  int getFrom();
  int getColor();
  int getPiece();

private:
  uint32_t moveNum;
};