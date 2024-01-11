#include "move_generation.hpp"

std::vector<uint64_t> MoveGeneration::factor_mask(uint64_t mask) {
  std::vector<uint64_t> out_vec(__builtin_popcountll(mask));
  int i = 0;
  uint64_t counter = 1;
  while (counter != 0) {
    counter = ((mask) & (mask - 1));
    out_vec[i] = mask ^ counter;
    mask = counter;
    i++;
  }
  return out_vec;
}

uint64_t MoveGeneration::generate_single_pawn_pushes(Position position,
                                                     int color) {
  // TODO add promotions
  uint64_t pawns_single_pushed;
  if (color == 0) {
    uint64_t pawns = position.getPieces()[white][pawn];
    pawns_single_pushed = (pawns << 8) & (~position.getPieces()[black][all]);
  } else {
    uint64_t pawns = position.getPieces()[black][pawn];
    pawns_single_pushed = (pawns >> 8) & ~position.getPieces()[white][all];
  }
  return pawns_single_pushed;
}
uint64_t MoveGeneration::generate_double_pawn_pushes(Position position,
                                                     int color) {
  uint64_t out;
  if (color == 0) {
    uint64_t pawns = position.getPieces()[white][pawn];
    uint64_t pawns_at_initial_config = pawns & ((0b1ULL << 2 * 8) - 1);
    out = pawns_at_initial_config << 2 * 8;
  } else {
    uint64_t pawns = position.getPieces()[black][pawn];
    uint64_t pawns_at_initial_config =
        pawns & (((0b1ULL << 2 * 8) - 1) * 6 * 8);
    out = pawns_at_initial_config >> 2 * 8;
  }
  return out;
}
uint64_t MoveGeneration::generate_pawn_pushes(Position position, int color) {
  uint64_t single_pawn_pushes = generate_single_pawn_pushes(position, color);
  uint64_t double_pawn_pushes = generate_double_pawn_pushes(position, color);
  return single_pawn_pushes | double_pawn_pushes;
}
// uint64_t generate_pawn_captures(Position position){
//   uint64_t pawns =
// }
void MoveGeneration::generateKnightMoves(Position position, color colorIn) {
  // int oppositePieceColor = (colorIn + 1) % 2;
  std::vector<Move> moveList;
  uint64_t eligibleSquares = ~position.getPieces()[colorIn][all];
  uint64_t remainingKnigths = position.getPieces()[colorIn][knight];
  int from;
  int to;
  while (remainingKnigths) {
    from = __builtin_ctzll(remainingKnigths);
    uint64_t generatedMoves = knightLookUpTable[from] & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      moveList.push_back(Move{from, to, knight, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
  }
}