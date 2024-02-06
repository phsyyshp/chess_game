#ifndef MOVE_HPP
#define MOVE_HPP

#include "loader.hpp"
// This forward declaration is for avoiding, circular dependency.

class Position;
class Move {
public:
  Move() = default;
  Move(int from, int to, int flags) : moveNum(from | (to << 6) | flags << 12){};
  Move(int from, int to, int flags, int score)
      : moveNum(from | (to << 6) | flags << 12 | (score << 16)){};

  // Setters;
  void setScore(const int &score);
  // Getters;
  uint32_t getMoveInt() const;
  int getTo() const;
  int getFrom() const;
  int getFlags() const;

  int getScore() const;
  // returns noPiece for non captures;
  bool isCapture() const;

  // Visualizers;
  void print() const;

private:
  uint32_t moveNum;
};

#endif