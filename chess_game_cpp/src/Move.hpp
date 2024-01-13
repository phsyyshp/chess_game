#ifndef MOVE_HPP
#define MOVE_HPP

#include "loader.hpp"
class Move {
public:
  Move(int from, int to, piece pieceType, color colorIn, bool isCapture)
      : moveNum(from | (to << 6) | (pieceType << 12) | (colorIn << 18) |
                ((isCapture * 1) << 24)) {}
  int getTo() const;
  int getFrom() const;
  int getColor() const;
  int getPiece() const;
  bool checkIsCapture() const;

private:
  uint32_t moveNum;
};
#endif