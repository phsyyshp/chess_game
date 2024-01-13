#include "move_generation.hpp"

std::vector<uint64_t> MoveGeneration::factor_mask(uint64_t mask) {
  uint64_t occupancy = 0b1ULL << 25 - 8;
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
void MoveGeneration::generateSinglePawnPushes(Position position,
                                              const color &colorIn) {
  // TODO: add promotions
  uint64_t generatedMoves;
  uint64_t pawns;
  int to, from;
  uint64_t ineligibleSquares =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  switch (colorIn) {
  case color::white:
    pawns = position.getPieces()[white][pawn];
    generatedMoves = (pawns << 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 8;
      moveList.push_back(Move{from, to, piece::pawn, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case color::black:

    pawns = position.getPieces()[black][pawn];
    generatedMoves = (pawns >> 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 8;
      moveList.push_back(Move{from, to, piece::pawn, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateDoublePawnPushes(Position position,
                                              const color &colorIn) {
  uint64_t pawns, pawns_at_initial_config, generatedMoves;
  int to, from;
  uint64_t ineligibleSquares =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  switch (colorIn) {
  case white:
    pawns = position.getPieces()[white][pawn];
    pawns_at_initial_config = pawns & ((0b1ULL << 2 * 8) - 1);
    generatedMoves = (pawns_at_initial_config << 2 * 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 16;
      moveList.push_back(Move{from, to, piece::pawn, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case black:
    pawns = position.getPieces()[black][pawn];
    pawns_at_initial_config = pawns & (((0b1ULL << 2 * 8) - 1) * 6 * 8);
    generatedMoves = (pawns_at_initial_config >> 2 * 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 16;
      moveList.push_back(Move{from, to, piece::pawn, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateLeftPawnCaptures(Position position,
                                              const color &colorIn) {
  uint64_t eligibleSquares;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[colorIn][pawn];
  int to, from;

  switch (colorIn) {
  case white:
    eligibleSquares = position.getPieces()[black][all];
    generatedMoves = ((pawns << 9) & (~A_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 9;
      moveList.push_back(Move{from, to, piece::pawn, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case black:
    eligibleSquares = position.getPieces()[white][all];
    generatedMoves = ((pawns >> 7) & (~A_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 7;
      moveList.push_back(Move{from, to, piece::pawn, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateRightPawnCaptures(Position position,
                                               const color &colorIn) {
  uint64_t eligibleSquares;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[colorIn][pawn];
  int to, from;

  switch (colorIn) {
  case white:
    eligibleSquares = position.getPieces()[black][all];
    generatedMoves = ((pawns << 7) & (~H_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 7;
      moveList.push_back(Move{from, to, piece::pawn, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case black:
    eligibleSquares = position.getPieces()[white][all];
    generatedMoves = ((pawns >> 9) & (~H_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 9;
      moveList.push_back(Move{from, to, piece::pawn, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}
// uint64_t generate_pawn_captures(Position position){
//   uint64_t pawns =
// }
void MoveGeneration::generateKnightMoves(Position position,
                                         const color &colorIn) {
  uint64_t eligibleSquares = ~position.getPieces()[colorIn][all];
  uint64_t remainingKnigths = position.getPieces()[colorIn][knight];
  uint64_t generatedMoves;

  int from;
  int to;
  while (remainingKnigths) {
    from = __builtin_ctzll(remainingKnigths);
    generatedMoves = knightLookUpTable[from] & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      moveList.push_back(Move{from, to, piece::knight, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    remainingKnigths ^= (0b1ull << from);
  }
}
void MoveGeneration::generateBishopMoves(Position position,
                                         const color &colorIn) {
  uint64_t eligibleSquares = ~position.getPieces()[colorIn][all];
  uint64_t occupancy =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  int64_t remainingBishops = position.getPieces()[colorIn][bishop];
  uint64_t generatedMoves;

  square from;
  int to;
  while (remainingBishops) {
    from = static_cast<square>(__builtin_ctzll(remainingBishops));
    generatedMoves = getBishopAttackMask(from, occupancy) & eligibleSquares;

    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      moveList.push_back(Move{from, to, piece::bishop, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    remainingBishops ^= (0b1ull << from);
  }
}
void MoveGeneration::generateRookMoves(Position position,
                                       const color &colorIn) {
  uint64_t eligibleSquares = ~position.getPieces()[colorIn][all];
  uint64_t occupancy =
      position.getPieces()[white][all] | position.getPieces()[black][all];
  uint64_t remainingRooks = position.getPieces()[colorIn][rook];
  square from;
  int to;
  uint64_t generatedMoves;
  while (remainingRooks) {
    from = static_cast<square>(__builtin_ctzll(remainingRooks));
    generatedMoves = getRookAttackMask(from, occupancy) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      moveList.push_back(Move{from, to, piece::rook, colorIn});
      generatedMoves ^= (0b1ull << to);
    }
    remainingRooks ^= (0b1ull << from);
  }
}
void MoveGeneration::generateQueenMoves(Position position,
                                        const color &colorIn) {
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
void MoveGeneration::generateKingMoves(Position position,
                                       const color &colorIn) {
  int from = __builtin_ctzll(position.getPieces()[colorIn][king]);
  int to;
  uint64_t eligibleSquares = ~position.getPieces()[colorIn][all];
  uint64_t generatedMoves = kingLookUpTable[from] & eligibleSquares;
  while (generatedMoves) {
    to = __builtin_ctzll(generatedMoves);
    moveList.push_back(Move{from, to, piece::king, colorIn});
    generatedMoves ^= (0b1ull << to);
  }
}
void MoveGeneration::generateAllMoves(Position position, const color &colorIn) {
  generateKnightMoves(position, colorIn);
  generateKingMoves(position, colorIn);
  generateSinglePawnPushes(position, colorIn);
  generateDoublePawnPushes(position, colorIn);
  generateLeftPawnCaptures(position, colorIn);
  generateRightPawnCaptures(position, colorIn);
  generateRookMoves(position, colorIn);
  generateBishopMoves(position, colorIn);
  generateQueenMoves(position, colorIn);
}

std::vector<Move> MoveGeneration::getMoves() const { return moveList; }