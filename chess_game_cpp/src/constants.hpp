#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <array>
#include <iostream>
enum color { white, black, invalid };
const std::array<color, 2> oppositeColor = {black, white};
enum piece { pawn, bishop, rook, queen, knight, king, noPiece };
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
const std::array<std::string, 64> chessSq = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2",
    "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3",
    "g3", "h3", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5",
    "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6",
    "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7",
    "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};
struct magicTbls {
  uint64_t mask;
  uint64_t magicNum;
  int shiftBit;
};
const uint64_t A_FILE =
    0b0000000100000001000000010000000100000001000000010000000100000001ull;
const uint64_t H_FILE =
    0b1000000010000000100000001000000010000000100000001000000010000000ull;
const std::array<int, 2> whoToMove = {1, -1};
// piece Square tables;
const std::array<int, 64> pawnSqTblsBlack = {
    0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
    10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
    0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
    5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0};
const std::array<int, 64> knightSqTblsBlack = {
    -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,
    0,   -20, -40, -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,
    15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,
    -30, -30, 5,   10,  15,  15,  10,  5,   -30, -40, -20, 0,   5,
    5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50};
const std::array<int, 64> bishopSqTblsBlack = {
    -20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,
    0,   0,   -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 5,
    5,   10,  10,  5,   5,   -10, -10, 0,   10,  10,  10,  10,  0,
    -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 5,   0,   0,
    0,   0,   5,   -10, -20, -10, -10, -10, -10, -10, -10, -20};
const std::array<int, 64> rookSqTblsBlack = {
    0,  0, 0, 0, 0, 0, 0, 0,  5,  10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
    -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
    -5, 0, 0, 0, 0, 0, 0, -5, 0,  0,  0,  5,  5,  0,  0,  0};
const std::array<int, 64> queenSqTblsBlack = {
    -20, -10, -10, -5, -5, -10, -10, -20, -10, 0,   0,   0,  0,  0,   0,   -10,
    -10, 0,   5,   5,  5,  5,   0,   -10, -5,  0,   5,   5,  5,  5,   0,   -5,
    0,   0,   5,   5,  5,  5,   0,   -5,  -10, 5,   5,   5,  5,  5,   0,   -10,
    -10, 0,   5,   0,  0,  0,   0,   -10, -20, -10, -10, -5, -5, -10, -10, -20};
const std::array<int, 64> endGameKingSqTblsBlack = {
    -50, -40, -30, -20, -20, -30, -40, -50, -30, -20, -10, 0,   0,
    -10, -20, -30, -30, -10, 20,  30,  30,  20,  -10, -30, -30, -10,
    30,  40,  40,  30,  -10, -30, -30, -10, 30,  40,  40,  30,  -10,
    -30, -30, -10, 20,  30,  30,  20,  -10, -30, -30, -30, 0,   0,
    0,   0,   -30, -30, -50, -30, -30, -30, -30, -30, -30, -50};
const std::array<int, 64> middleGameKingSqTblsBlack = {
    -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50,
    -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40,
    -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30,
    -20, -10, -20, -20, -20, -20, -20, -20, -10, 20,  20,  0,   0,
    0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20};
const std::array<int, 64> pawnSqTblsWhite = {
    0,  0,  0,   0,  0,  0,   0,  0,  5,  10, 10, -20, -20, 10, 10, 5,
    5,  -5, -10, 0,  0,  -10, -5, 5,  0,  0,  0,  20,  20,  0,  0,  0,
    5,  5,  10,  25, 25, 10,  5,  5,  10, 10, 20, 30,  30,  20, 10, 10,
    50, 50, 50,  50, 50, 50,  50, 50, 0,  0,  0,  0,   0,   0,  0,  0};
const std::array<int, 64> knightSqTblsWhite = {
    -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   5,   5,
    0,   -20, -40, -30, 5,   10,  15,  15,  10,  5,   -30, -30, 0,
    15,  20,  20,  15,  0,   -30, -30, 5,   15,  20,  20,  15,  5,
    -30, -30, 0,   10,  15,  15,  10,  0,   -30, -40, -20, 0,   0,
    0,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50};
const std::array<int, 64> bishopSqTblsWhite = {
    -20, -10, -10, -10, -10, -10, -10, -20, -10, 5,   0,   0,   0,
    0,   5,   -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,
    10,  10,  10,  10,  0,   -10, -10, 5,   5,   10,  10,  5,   5,
    -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 0,   0,   0,
    0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20};
const std::array<int, 64> rookSqTblsWhite = {
    0,  0,  0,  5,  5,  0,  0,  0,  -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
    5,  10, 10, 10, 10, 10, 10, 5,  0,  0, 0, 0, 0, 0, 0, 0};
const std::array<int, 64> queenSqTblsWhite = {
    -20, -10, -10, -5, -5, -10, -10, -20, -10, 0,   0,   0,  0,  5,   0,   -10,
    -10, 0,   5,   5,  5,  5,   5,   -10, -5,  0,   5,   5,  5,  5,   0,   0,
    -5,  0,   5,   5,  5,  5,   0,   -5,  -10, 0,   5,   5,  5,  5,   0,   -10,
    -10, 0,   0,   0,  0,  0,   0,   -10, -20, -10, -10, -5, -5, -10, -10, -20};
const std::array<int, 64> endGameKingSqTblsWhite = {
    -50, -30, -30, -30, -30, -30, -30, -50, -30, -30, 0,   0,   0,
    0,   -30, -30, -30, -10, 20,  30,  30,  20,  -10, -30, -30, -10,
    30,  40,  40,  30,  -10, -30, -30, -10, 30,  40,  40,  30,  -10,
    -30, -30, -10, 20,  30,  30,  20,  -10, -30, -30, -20, -10, 0,
    0,   -10, -20, -30, -50, -40, -30, -20, -20, -30, -40, -50};
const std::array<int, 64> middleGameKingSqTblsWhite = {
    20,  30,  10,  0,   0,   10,  30,  20,  20,  20,  0,   0,   0,
    0,   20,  20,  -10, -20, -20, -20, -20, -20, -20, -10, -20, -30,
    -30, -40, -40, -30, -30, -20, -30, -40, -40, -50, -50, -40, -40,
    -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50,
    -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30};
const std::array<std::array<int, 64>, 2> pawnSqTbls = {pawnSqTblsWhite,
                                                       pawnSqTblsBlack};
const std::array<std::array<int, 64>, 2> knightSqTbls = {knightSqTblsWhite,
                                                         knightSqTblsBlack};
const std::array<std::array<int, 64>, 2> rookSqTbls = {rookSqTblsWhite,
                                                       rookSqTblsBlack};
const std::array<std::array<int, 64>, 2> bishopSqTbls = {bishopSqTblsWhite,
                                                         bishopSqTblsBlack};
const std::array<std::array<int, 64>, 2> queenSqTbls = {queenSqTblsWhite,
                                                        queenSqTblsBlack};
const std::array<std::array<int, 64>, 2> endGameKingSqTbls = {
    endGameKingSqTblsWhite, endGameKingSqTblsBlack};
const std::array<std::array<int, 64>, 2> earlyGameKingSqTbls = {
    middleGameKingSqTblsWhite, middleGameKingSqTblsBlack};

// MVV_LVA

// MVV_LVA[victim][attacker]
// victim P, attacker P,B,R,Q,N,K,None
// victim B,
// victim R,
// victim Q,
// victim N,
// victim K,
// victim None
const std::array<int, 7> pawnMVV_LVA = {15, 13, 12, 11, 14, 10, 0};
const std::array<int, 7> bishopMVV_LVA = {35, 33, 32, 31, 34, 30, 0};
const std::array<int, 7> rookMVV_LVA = {45, 43, 42, 41, 44, 40, 0};
const std::array<int, 7> queenMVV_LVA = {55, 53, 52, 51, 54, 50, 0};
const std::array<int, 7> knightMVV_LVA = {25, 23, 22, 21, 24, 20, 0};
const std::array<int, 7> kingMVV_LVA = {0, 0, 0, 0, 0, 0, 0};
const std::array<int, 7> noPieceMVV_LVA = {0, 0, 0, 0, 0, 0, 0};
const std::array<std::array<int, 7>, 7> MVV_LVA = {
    pawnMVV_LVA,   bishopMVV_LVA, rookMVV_LVA,   queenMVV_LVA,
    knightMVV_LVA, kingMVV_LVA,   noPieceMVV_LVA};
// TODO: becarefull with types reconsider it;
const uint32_t MVV_LVA_OFFSET = UINT32_MAX - 256;

// Killer move constants;

// TODO: becarefull with types reconsider it;
const int MAX_KILLER_MOVES = 2;
const int MAX_DEPTH = 64;
const int KILLER_VALUE = 10;
#endif