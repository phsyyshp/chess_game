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
uint64_t MoveGeneration::generateSinglePawnPushes(Position position,
                                                  const color &colorIn) {
  // TODO: add promotions
  // FIXME: fix the type errors;
  uint64_t pawns_single_pushed;
  if (colorIn == white) {
    uint64_t pawns = position.getPieces()[white][pawn];
    pawns_single_pushed = (pawns << 8) & (~position.getPieces()[black][all]);
  } else {
    uint64_t pawns = position.getPieces()[black][pawn];
    pawns_single_pushed = (pawns >> 8) & ~position.getPieces()[white][all];
  }
  return pawns_single_pushed;
}
uint64_t MoveGeneration::generateDoublePawnPushes(Position position,
                                                  const color &colorIn) {
  std::vector<Move> movelist;
  switch (colorIn) {
  case white:
    uint64_t pawns = position.getPieces()[white][pawn];
    uint64_t pawns_at_initial_config = pawns & ((0b1ULL << 2 * 8) - 1);
    while (pawns_at_initial_config) {
    }
    return pawns_at_initial_config << 2 * 8;
    break;
  case black:

    uint64_t pawns = position.getPieces()[black][pawn];
    uint64_t pawns_at_initial_config =
        pawns & (((0b1ULL << 2 * 8) - 1) * 6 * 8);
    return pawns_at_initial_config >> 2 * 8;
    break;
  default:
    break;
  }
}
uint64_t MoveGeneration::generatePawnPushes(Position position,
                                            const color &colorIn) {
  uint64_t single_pawn_pushes = generateSinglePawnPushes(position, colorIn);
  uint64_t double_pawn_pushes = generateDoublePawnPushes(position, colorIn);
  return single_pawn_pushes | double_pawn_pushes;
}
// uint64_t generate_pawn_captures(Position position){
//   uint64_t pawns =
// }
void MoveGeneration::generateKnightMoves(Position position,
                                         const color &colorIn) {
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
      moveList.push_back(Move{from, to, piece::knight, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
  }
  remainingKnigths ^= (0b1ull << from);
}
void MoveGeneration::generateBishopMoves(Position position,
                                         const color &colorIn) {
  std::vector<Move> moveList;
  uint64_t eligibleSquares = ~position.getPieces()[colorIn][all];
  uint64_t occupancy =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  uint64_t remainingBishops = position.getPieces()[colorIn][bishop];
  square from;
  int to;
  while (remainingBishops) {
    from = static_cast<square>(__builtin_ctzll(remainingBishops));
    uint64_t generatedMoves =
        getBishopAttackMask(from, occupancy) & eligibleSquares;

    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      moveList.push_back(Move{from, to, piece::bishop, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
  }
  remainingBishops ^= (0b1ull << from);
}
void MoveGeneration::generateRookMoves(Position position,
                                       const color &colorIn) {

  std::vector<Move> moveList;
  uint64_t eligibleSquares = ~position.getPieces()[colorIn][all];
  uint64_t occupancy =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  uint64_t remainingRooks = position.getPieces()[colorIn][rook];
  square from;
  int to;
  while (remainingRooks) {
    from = static_cast<square>(__builtin_ctzll(remainingRooks));
    uint64_t generatedMoves =
        getRookAttackMask(from, occupancy) & eligibleSquares;

    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      moveList.push_back(Move{from, to, piece::rook, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
  }
  remainingRooks ^= (0b1ull << from);
}
void MoveGeneration::generateQueenMoves(Position position,
                                        const color &colorIn) {

  std::vector<Move> moveList;
  uint64_t eligibleSquares = ~position.getPieces()[colorIn][all];
  uint64_t occupancy =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  uint64_t queenMask = position.getPieces()[colorIn][queen];
  int to;
  square from = static_cast<square>(__builtin_ctzll(queenMask));
  if (queenMask) {

    uint64_t generatedMoves = (getRookAttackMask(from, occupancy) |
                               getBishopAttackMask(from, occupancy)) &
                              eligibleSquares;

    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      moveList.push_back(Move{from, to, piece::queen, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
  }
}
