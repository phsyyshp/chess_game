#ifndef MOVE_HPP
#define MOVE_HPP

#include "loader.hpp"
// This forward declaration is for avoiding, circular dependency.
class Position;
class Move {
public:
  Move() = default;
  Move(int from, int to, piece piece_, color color_, bool isCapture_)
      : moveNum(from | (to << 6) | (piece_ << 12) | (color_ << 15) |
                ((isCapture_ * 1) << 16)){};
  Move(int from, int to, piece piece_, color color_, bool isCapture_, int score)
      : moveNum(from | (to << 6) | (piece_ << 12) | (color_ << 15) |
                ((isCapture_ * 1) << 16) | (score << 17)){};

  // Setters;
  void setScore(const int &score);
  // Getters;
  int getTo() const;
  int getFrom() const;
  int getColor() const;
  int getPiece() const;
  int getScore() const;
  // returns noPiece for non captures;
  piece getCaptured(const Position &position) const;
  bool checkIsCapture() const;

  // Visualizers;
  void print() const;

private:
  uint32_t moveNum;
};
#endif