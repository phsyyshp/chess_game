#include "move_generation.hpp"
// #include "position.hpp"
#include <cstdint>
#include <vector>

std::vector<uint64_t> MoveGeneration::factor_mask(uint64_t mask) {
  std::vector<uint64_t> out_vec(__builtin_popcount(mask));
  int i = 0;
  int counter = 1;
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
    uint64_t pawns = position.white_pieces.pawns;
    uint64_t pawns_single_pushed = (pawns << 8) & -position.black_pieces.all;
  } else {
    uint64_t pawns = position.black_pieces.pawns;
    uint64_t pawns_single_pushed = (pawns >> 8) & -position.white_pieces.all;
  }
  return pawns_single_pushed;
}
uint64_t MoveGeneration::generate_double_pawn_pushes(Position position,
                                                     int color) {
  uint64_t out;
  if (color == 1) {
    uint64_t pawns = position.white_pieces.pawns;
    uint64_t pawns_at_initial_config = pawns & ((0b1ULL << 2 * 8) - 1);
    uint64_t out = pawns_at_initial_config << 2 * 8;
  } else {
    uint64_t pawns = position.black_pieces.pawns;
    uint64_t pawns_at_initial_config = pawns & ((0b1ULL << 63) - 1);
    uint64_t out = pawns_at_initial_config >> 2 * 8;
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
