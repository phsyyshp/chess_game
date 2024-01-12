#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <iostream>
enum color { white, black };
enum piece { pawn, bishop, rook, queen, knight, king, all };
enum square {
  a1,
  b1,
  c1,
  d1,
  e1,
  f1,
  g1,
  h1,
  a2,
  b2,
  c2,
  d2,
  e2,
  f2,
  g2,
  h2,
  a3,
  b3,
  c3,
  d3,
  e3,
  f3,
  g3,
  h3,
  a4,
  b4,
  c4,
  d4,
  e4,
  f4,
  g4,
  h4,
  a5,
  b5,
  c5,
  d5,
  e5,
  f5,
  g5,
  h5,
  a6,
  b6,
  c6,
  d6,
  e6,
  f6,
  g6,
  h6,
  a7,
  b7,
  c7,
  d7,
  e7,
  f7,
  g7,
  h7,
  a8,
  b8,
  c8,
  d8,
  e8,
  f8,
  g8,
  h8
};
struct magicTbls {
  uint64_t mask;
  uint64_t magicNum;
  int shiftBit;
};
const uint64_t A_FILE =
    0b0000000100000001000000010000000100000001000000010000000100000001ull;
const uint64_t H_FILE =
    0b1000000010000000100000001000000010000000100000001000000010000000ull;
#endif