#pragma once

#include "loader.hpp"
// This forward declaration is for avoiding, circular dependency.

class Position;
class Move {
public:
  Move() = default;
  Move(uint from, uint to, uint flags)
      : moveNum((from & 0x3f) | ((to & 0x3f) << 6) | ((flags & 0xf) << 12)){};
  Move(uint from, uint to, uint flags, int score)
      : moveNum((from & 0x3f) | ((to & 0x3f) << 6) | ((flags & 0xf) << 12) |
                (score << 16)){};
  Move &operator=(const Move &);
  // Setters;
  // TODO: maybe overflow error
  void setScore(const int &score);
  // Getters;
  uint32_t getMoveInt() const;
  uint getTo() const;
  uint getFrom() const;
  uint getFlags() const;
  int getScore() const;
  // returns noPiece for non captures;
  bool isCapture() const;
  bool isPromo() const;

  // Visualizers;
  void print() const;

  std::string toStr() const;

private:
  uint32_t moveNum;
};
