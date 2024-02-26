#pragma once
#include <array>
#include <iostream>
#include <vector>
enum Color { WHITE, BLACK, INVALID };
const std::array<Color, 2> OPPOSITE_COLOR = {BLACK, WHITE};
enum Piece { PAWN, BISHOP, ROOK, QUEEN, KNIGHT, KING, NO_PIECE };
struct TimeInfo {
  int remainingTime;
  int timeIncrement;
};
enum Square {
  A1,
  B1,
  C1,
  D1,
  E1,
  F1,
  G1,
  H1,
  A2,
  B2,
  C2,
  D2,
  E2,
  F2,
  G2,
  H2,
  A3,
  B3,
  C3,
  D3,
  E3,
  F3,
  G3,
  H3,
  A4,
  B4,
  C4,
  D4,
  E4,
  F4,
  G4,
  H4,
  A5,
  B5,
  C5,
  D5,
  E5,
  F5,
  G5,
  H5,
  A6,
  B6,
  C6,
  D6,
  E6,
  F6,
  G6,
  H6,
  A7,
  B7,
  C7,
  D7,
  E7,
  F7,
  G7,
  H7,
  A8,
  B8,
  C8,
  D8,
  E8,
  F8,
  G8,
  H8
};
const std::array<std::string, 2> COLOR_INITIALS = {"w", "b"};
const std::array<std::string, 64> SQUARE_NAMES = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2",
    "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3",
    "g3", "h3", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5",
    "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6",
    "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7",
    "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};
enum MoveType {
  quietMoves,
  doublePawnPush,
  kingCastle,
  queenCastle,
  captures,
  ePCapture, // En Passant capture
  knightPromotion,
  bishopPromotion,
  rookPromotion,
  queenPromotion,
  knightPromoCapture, // Knight promotion with capture
  bishopPromoCapture, // Bishop promotion with capture
  rookPromoCapture,   // Rook promotion with capture
  queenPromoCapture   // Queen promotion with capture
};
const std::array<char, 8> PROMOTION_TYPE_TO_STR = {'n', 'b', 'r', 'q',
                                                   'n', 'b', 'r', 'q'};
const std::array<char, 8> PROMOTION_TYPE_TO_STR_SHORT = {'n', 'b', 'r', 'q'};
struct magicTbls {
  uint64_t mask;
  uint64_t magicNum;
  int shiftBit;
};
// bitboard FILES;
const uint64_t A_FILE =
    0b0000000100000001000000010000000100000001000000010000000100000001ull;
const uint64_t H_FILE =
    0b1000000010000000100000001000000010000000100000001000000010000000ull;
// pawn pushes;
const std::array<int, 2> COLOR_TO_PUSH_FORWARD = {8, -8};
const std::array<int, 2> COLOR_TO_PUSH_TWO_FORWARD = {16, -16};
const std::array<int, 2> COLOR_TO_PUSH_LEFT_FORWARD = {9, -7};
const std::array<int, 2> COLOR_TO_PUSH_RIGHT_FORWARD = {7, -9};

// bitboard Ranks;

constexpr uint64_t RANK_1_MASK = (0b11111111ULL);
constexpr uint64_t RANK_8_MASK = (0b11111111ULL) << 7 * 8;
const std::array<uint64_t, 2> INITIAL_PAWN_RANK_MASK = {
    ((0b1ULL << 2 * 8) - 1), (0b11111111ULL << 6 * 8)};
const std::array<int, 2> WHO_TO_MOVE = {1, -1};
constexpr std::array<uint, 64> SQUARE_TO_FILE = {
    0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5,
    6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3,
    4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7};
// Castling;
// Castling Rights Table
// +-----------------------+---------------------+
// |       Permission      |      Castling       |
// +-----------------------+---------------------+
// | canWhiteCastleQueenSide | 1 0 0 0 | 8
// | canWhiteCastleKingSide  | 0 1 0 0 | 4
// | canBlackCastleQueenSide | 0 0 1 0 | 2
// | canBlackCastleKingSide  | 0 0 0 1 | 1
// +-----------------------+---------------------+
// QKqk
enum castlingType {
  BLACK_KING_SIDE,
  BLACK_QUEEN_SIDE,
  WHITE_KING_SIDE,
  WHITE_QUEEN_SIDE
};
const std::array<std::string, 5> CHAR_TO_CATSLING_ENCODING = {"-", "k", "q",
                                                              "K", "Q"};
constexpr uint NO_WHITE_CASTLING_MASK = 0b0011u;
constexpr uint NO_BLACK_CASTLING_MASK = 0b1100u;
constexpr uint NO_WHITE_QUEEN_SIDE_CASTLING_MASK = 0b0111u;
constexpr uint NO_WHITE_KING_SIDE_CASTLING_MASK = 0b1011u;
constexpr uint NO_BLACK_QUEEN_SIDE_CASTLING_MASK = 0b1101u;
constexpr uint NO_BLACK_KING_SIDE_CASTLING_MASK = 0b1110u;
constexpr uint WHITE_QUEEN_SIDE_CASTLING_MASK = 0b1000u;
constexpr uint WHITE_KING_SIDE_CASTLING_MASK = 0b0100u;
constexpr uint BLACK_QUEEN_SIDE_CASTLING_MASK = 0b0010u;
constexpr uint BLACK_KING_SIDE_CASTLING_MASK = 0b0001u;

// Castling moves;
// const std::array<Move, 4> CASTLING_MOVES = {
//     Move{e8, g8, MoveType::kingCastle},
//     Move{e8, c8, MoveType::queenCastle},
//     Move{e1, g1, MoveType::kingCastle},
//     Move{e1, c1, MoveType::queenCastle},
// };
const std::array<uint, 2> NO_CASTLING_COLOR_MASK_LOOK_UP = {
    NO_WHITE_CASTLING_MASK, NO_BLACK_CASTLING_MASK};

constexpr uint64_t WHITE_QUEEN_SIDE_CASTLING_RAY =
    (0b1ull << B1) | (0b1ull << C1) | (0b1ull << D1);
constexpr uint64_t WHITE_KING_SIDE_CASTLING_RAY =
    (0b1ull << F1) | (0b1ull << G1);
constexpr uint64_t BLACK_QUEEN_SIDE_CASTLING_RAY =
    (0b1ull << B8) | (0b1ull << C8) | (0b1ull << D8);
constexpr uint64_t BLACK_KING_SIDE_CASTLING_RAY =
    (0b1ull << F8) | (0b1ull << G8);

// enPassant;
constexpr uint NO_EP = 8;
// Move Flags;
/*
+------+---------+--------+----------+----------+----------------------+
| Code | Promo.  | Capt.  | Special1 | Special0 | Kind of Move         |
+------+---------+--------+----------+----------+----------------------+
| 0    | 0       | 0      | 0        | 0        | Quiet moves          |
| 1    | 0       | 0      | 0        | 1        | Double pawn push     |
| 2    | 0       | 0      | 1        | 0        | King castle          |
| 3    | 0       | 0      | 1        | 1        | Queen castle         |
| 4    | 0       | 1      | 0        | 0        | Captures             |
| 5    | 0       | 1      | 0        | 1        | EP-capture           |
| 8    | 1       | 0      | 0        | 0        | Knight-promotion     |
| 9    | 1       | 0      | 0        | 1        | Bishop-promotion     |
| 10   | 1       | 0      | 1        | 0        | Rook-promotion       |
| 11   | 1       | 0      | 1        | 1        | Queen-promotion      |
| 12   | 1       | 1      | 0        | 0        | Knight-promo capture |
| 13   | 1       | 1      | 0        | 1        | Bishop-promo capture |
| 14   | 1       | 1      | 1        | 0        | Rook-promo capture   |
| 15   | 1       | 1      | 1        | 1        | Queen-promo capture  |
+------+---------+--------+----------+----------+----------------------+
*/
// codes;
constexpr uint QUIET_MOVE = 0;
constexpr uint DOUBLE_PAWN_PUSH = 1;
constexpr uint CAPTURE = 4;
constexpr uint EP_CAPTURE = 5;

// shifted flags;
constexpr uint32_t CAPTURE_FLAG = 0b100u << 12;
// piece Square tables;
const std::array<int, 64> PAWN_SQ_TBLS_BLACK = {
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
                                                       PAWN_SQ_TBLS_BLACK};
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
const std::array<std::array<int, 64>, 2> middleGameKingSqTbls = {
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
const int MVV_LVA_OFFSET = INT16_MAX - 256;

// Killer move constants;

// TODO: becarefull with types reconsider it;
const int MAX_KILLER_MOVES = 2;
const int MAX_DEPTH = 100;
const int KILLER_VALUE = 10;
// Zobrist
constexpr uint COLOR_INDEX = 768;
// tt
enum nodeType : uint8_t { UPPERBOUND, LOWERBOUND, EXACT };
constexpr int TT_MOVE_SORT_VALUE = 260;
const std::size_t TT_SIZE = 1048576 / 2;