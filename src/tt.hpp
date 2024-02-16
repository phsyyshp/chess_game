#pragma once
#include "constants.hpp"
class TranspositionTable {
  static std::array<hashEntry, 100> tt;

public:
  TranspositionTable() = default;
  void add(const hashEntry &entry);
  void replaceNyDepth(const hashEntry &entry);
  hashEntry get(const hashEntry &entry);
};