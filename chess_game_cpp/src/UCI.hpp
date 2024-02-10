#pragma once
#include "constants.hpp"
#include "move_generation.hpp"
#include "position.hpp"
#include "utilities.hpp"
class UCI {

public:
  UCI() = default;
  UCI(Position &position_) : position(position_){};
  Move getMove(const std::string &moveStr) const;

private:
  Position &position;
  MoveType getFlag(const std::string &moveStr) const;
};