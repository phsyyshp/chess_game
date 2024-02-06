#ifndef MOVE_HPP
#define MOVE_HPP

#include "loader.hpp"
// This forward declaration is for avoiding, circular dependency.

class Position;
class Move {
public:
  Move() = default;
  Move(uint from, uint to, uint flags)
      : moveNum(from | (to << 6) | flags << 12){};
  Move(uint from, uint to, uint flags, uint16_t score)
      : moveNum(from | (to << 6) | flags << 12 | (score << 16)){};

  // Setters;
  void setScore(const uint16_t &score);
  // Getters;
  uint32_t getMoveInt() const;
  uint getTo() const;
  uint getFrom() const;
  uint getFlags() const;
  uint16_t getScore() const;
  // returns noPiece for non captures;
  bool isCapture() const;

  // Visualizers;
  void print() const;

private:
  uint32_t moveNum;
};

#endif