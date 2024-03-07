
#pragma once
#include "constants.hpp"
#include "moveGeneration.hpp"
#include "position.hpp"
#include "utilities.hpp"
class moveToStr {
public:
  moveToStr() = default;
  moveToStr(Position &position_) : position(position_){};

  Move getMove(const std::string &moveStr) const;
  // commands:

private:
  Position &position;
  MoveType getFlag(const std::string &moveStr) const;
};