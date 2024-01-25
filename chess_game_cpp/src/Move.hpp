#ifndef MOVE_HPP
#define MOVE_HPP

#include "loader.hpp"
class Move {
public:
  Move() = default;
  Move(int from, int to, piece piece_, color color_, bool isCapture_)
      : moveNum(from | (to << 6) | (piece_ << 12) | (color_ << 18) |
                ((isCapture_ * 1) << 24)){};
  int getTo() const;
  int getFrom() const;
  int getColor() const;
  int getPiece() const;
  bool checkIsCapture() const;

private:
  uint32_t moveNum;
};
#endif