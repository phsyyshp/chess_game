#include "move_generation.hpp"
// generate moves;
void MoveGeneration::generateSinglePawnPushes() {
  // TODO: add promotions
  uint64_t generatedMoves;
  uint64_t pawns;
  int to, from;
  uint64_t ineligibleSquares =
      position.getAllPieces(white) | position.getAllPieces(black);
  switch (position.getTurn()) {
  case color::white:
    pawns = position.getPieces()[white][pawn];
    generatedMoves = (pawns << 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 8;
      moveList.push_back(Move{from, to, QUIET_MOVE});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case color::black:

    pawns = position.getPieces()[black][pawn];
    generatedMoves = (pawns >> 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 8;
      moveList.push_back(Move{from, to, QUIET_MOVE});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateDoublePawnPushes() {
  uint64_t pawns, pawns_at_initial_config, generatedMoves;
  int to, from;
  uint64_t ineligibleSquares =
      position.getAllPieces(white) | position.getAllPieces(black);
  uint64_t oneStepMoves;
  switch (position.getTurn()) {
  case white:
    pawns = position.getPieces()[white][pawn];
    pawns_at_initial_config = pawns & ((0b1ULL << 2 * 8) - 1);
    oneStepMoves = (pawns_at_initial_config << 8) & (~ineligibleSquares);
    generatedMoves = (oneStepMoves << 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 16;
      moveList.push_back(Move{from, to, DOUBLE_PAWN_PUSH});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case black:
    pawns = position.getPieces()[black][pawn];
    pawns_at_initial_config = pawns & (0b11111111ULL << 6 * 8);
    oneStepMoves = (pawns_at_initial_config >> 8) & (~ineligibleSquares);
    generatedMoves = (oneStepMoves >> 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 16;
      moveList.push_back(Move{from, to, DOUBLE_PAWN_PUSH});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateLeftPawnCaptures() {
  uint64_t eligibleSquares;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[position.getTurn()][pawn];
  int to, from;

  switch (position.getTurn()) {
  case white:
    eligibleSquares = position.getAllPieces(black);
    generatedMoves = ((pawns << 9) & (~A_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 9;
      moveList.push_back(Move{from, to, CAPTURE});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case black:
    eligibleSquares = position.getAllPieces(white);
    generatedMoves = ((pawns >> 7) & (~A_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 7;
      moveList.push_back(Move{from, to, CAPTURE});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateRightPawnCaptures() {
  uint64_t eligibleSquares;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[position.getTurn()][pawn];
  int to, from;

  switch (position.getTurn()) {
  case white:
    eligibleSquares = position.getAllPieces(black);
    generatedMoves = ((pawns << 7) & (~H_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 7;
      moveList.push_back(Move{from, to, CAPTURE});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case black:
    eligibleSquares = position.getAllPieces(white);
    generatedMoves = ((pawns >> 9) & (~H_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 9;
      moveList.push_back(Move{from, to, CAPTURE});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}

void MoveGeneration::generateRightEPCaptures() {
  uint fileEP;
  uint64_t victimMask;
  uint64_t eligibleSquares;
  uint64_t AttackerPawnMask;

  switch (position.getTurn()) {
  case color::white:
    fileEP = position.getGameState().getEnPassant();
    victimMask = 0b1ull << (fileEP + 8 * 4);
    eligibleSquares = ((victimMask) >> 1) & (~H_FILE);
    AttackerPawnMask = eligibleSquares & position.getPieces()[white][pawn];
    if (AttackerPawnMask) {
      moveList.push_back(Move{__builtin_ctzll(AttackerPawnMask),
                              __builtin_ctzll(AttackerPawnMask << 9),
                              EP_CAPTURE});
    }
    break;

  case color::black:
    fileEP = position.getGameState().getEnPassant();
    victimMask = 0b1ull << (fileEP + 8 * 3);

    eligibleSquares = ((victimMask) >> 1) & (~H_FILE);
    AttackerPawnMask = eligibleSquares & position.getPieces()[black][pawn];
    if (AttackerPawnMask) {
      moveList.push_back(Move{__builtin_ctzll(AttackerPawnMask),
                              __builtin_ctzll(AttackerPawnMask >> 7),
                              EP_CAPTURE});
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateLeftEPCaptures() {

  uint fileEP;
  uint64_t victimMask;
  uint64_t eligibleSquares;
  uint64_t AttackerPawnMask;
  switch (position.getTurn()) {
  case color::white:
    fileEP = position.getGameState().getEnPassant();
    victimMask = 0b1ull << (fileEP + 8 * 4);
    eligibleSquares = ((victimMask) << 1) & (~A_FILE);
    AttackerPawnMask = eligibleSquares & position.getPieces()[white][pawn];
    if (AttackerPawnMask) {
      moveList.push_back(Move{__builtin_ctzll(AttackerPawnMask),
                              __builtin_ctzll(AttackerPawnMask << 7),
                              EP_CAPTURE});
    }
    break;

  case color::black:
    fileEP = position.getGameState().getEnPassant();
    victimMask = 0b1ull << (fileEP + 8 * 3);

    eligibleSquares = ((victimMask) << 1) & (~A_FILE);
    AttackerPawnMask = eligibleSquares & position.getPieces()[black][pawn];
    if (AttackerPawnMask) {
      moveList.push_back(Move{__builtin_ctzll(AttackerPawnMask),
                              __builtin_ctzll(AttackerPawnMask >> 9),
                              EP_CAPTURE});
    }
    break;
  default:
    break;
  }
}

void MoveGeneration::generateKnightMoves() {

  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t remainingKnigths = position.getPieces()[position.getTurn()][knight];
  uint64_t generatedMoves;
  uint from;
  uint to;
  uint isCaptureFlag;
  while (remainingKnigths) {
    from = __builtin_ctzll(remainingKnigths);
    generatedMoves = knightLookUpTable[from] & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      isCaptureFlag = !position.isEmpty(to) * 4;
      moveList.push_back(Move{from, to, isCaptureFlag});
      generatedMoves ^= (0b1ull << to);
    }
    remainingKnigths ^= (0b1ull << from);
  }
}
void MoveGeneration::generateBishopMoves() {
  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t occupancy =
      position.getAllPieces(white) | position.getAllPieces(black);
  int64_t remainingBishops = position.getPieces()[position.getTurn()][bishop];
  uint64_t generatedMoves;

  square from;
  int to;
  uint isCaptureFlag;
  while (remainingBishops) {
    from = static_cast<square>(__builtin_ctzll(remainingBishops));
    generatedMoves = getBishopAttackMask(from, occupancy) & eligibleSquares;

    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      isCaptureFlag = !position.isEmpty(to) * 4;
      moveList.push_back(Move{from, to, isCaptureFlag});
      generatedMoves ^= (0b1ull << to);
    }
    remainingBishops ^= (0b1ull << from);
  }
}
void MoveGeneration::generateRookMoves() {
  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t occupancy =
      position.getAllPieces(white) | position.getAllPieces(black);
  uint64_t remainingRooks = position.getPieces()[position.getTurn()][rook];
  square from;
  int to;
  uint64_t generatedMoves;
  uint isCaptureFlag;
  while (remainingRooks) {
    from = static_cast<square>(__builtin_ctzll(remainingRooks));
    generatedMoves = getRookAttackMask(from, occupancy) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      isCaptureFlag = !position.isEmpty(to) * 4;
      moveList.push_back(Move{from, to, isCaptureFlag});
      generatedMoves ^= (0b1ull << to);
    }
    remainingRooks ^= (0b1ull << from);
  }
}
void MoveGeneration::generateQueenMoves() {
  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t occupancy =
      position.getAllPieces(white) | position.getAllPieces(black);
  uint64_t queenMask = position.getPieces()[position.getTurn()][queen];
  int to;
  square from = static_cast<square>(__builtin_ctzll(queenMask));
  uint isCaptureFlag;
  if (queenMask) {
    uint64_t generatedMoves = (getRookAttackMask(from, occupancy) |
                               getBishopAttackMask(from, occupancy)) &
                              eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);

      isCaptureFlag = !position.isEmpty(to) * 4;
      moveList.push_back(Move{from, to, isCaptureFlag});
      generatedMoves ^= (0b1ull << to);
    }
  }
}
void MoveGeneration::generateKingMoves() {
  int from = __builtin_ctzll(position.getPieces()[position.getTurn()][king]);
  int to;
  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t generatedMoves = kingLookUpTable[from] & eligibleSquares;
  uint64_t allPieces =
      position.getAllPieces(black) | position.getAllPieces(white);
  uint isCaptureFlag;
  while (generatedMoves) {
    to = __builtin_ctzll(generatedMoves);
    isCaptureFlag = !position.isEmpty(to) * 4;
    moveList.push_back(Move{from, to, isCaptureFlag});
    generatedMoves ^= (0b1ull << to);
  }
}
void MoveGeneration::generateCastling() {
  int color_ = position.getTurn();
  uint64_t allPieces =
      position.getAllPieces(black) | position.getAllPieces(white);
  switch (color_) {
  case white:
    if (~(allPieces & WHITE_QUEEN_SIDE_CASTLING_RAY)) {

      moveList.push_back(Move{
          e1,
          c1,
      });
    }
    break;

  default:
    break;
  }
}
void MoveGeneration::generateAllMoves() {
  generateSinglePawnPushes();
  generateDoublePawnPushes();
  generateLeftPawnCaptures();
  generateRightPawnCaptures();
  generateKnightMoves();
  generateKingMoves();
  generateRookMoves();
  generateBishopMoves();
  generateQueenMoves();
  if (position.getGameState().getEnPassant() != NO_EP) {
    generateLeftEPCaptures();
    generateRightEPCaptures();
  }
}
// generate attacks;

uint64_t MoveGeneration::generateRookAttackMaps() {
  uint64_t eligibleSquares = ~position.getAllPieces(position.getOppositeTurn());
  uint64_t occupancy =
      position.getAllPieces(white) | position.getAllPieces(black);
  uint64_t remainingRooks =
      position.getPieces()[position.getOppositeTurn()][rook];
  square square_;
  int to;
  uint64_t generatedAttacks = 0b0ull;
  while (remainingRooks) {
    square_ = static_cast<square>(__builtin_ctzll(remainingRooks));
    generatedAttacks |= getRookAttackMask(square_, occupancy) & eligibleSquares;
    remainingRooks ^= (0b1ull << square_);
  }
  return generatedAttacks;
}
uint64_t MoveGeneration::generateBishopAttackMaps() {
  uint64_t occupancy =
      position.getAllPieces(white) | position.getAllPieces(black);
  uint64_t remainingBishops =
      position.getPieces()[position.getOppositeTurn()][bishop];
  square square_;
  uint64_t generatedAttacks = 0b0ull;
  while (remainingBishops) {
    square_ = static_cast<square>(__builtin_ctzll(remainingBishops));
    generatedAttacks |= getBishopAttackMask(square_, occupancy);
    remainingBishops ^= (0b1ull << square_);
  }
  return generatedAttacks;
}
uint64_t MoveGeneration::generateQueenAttackMaps() {
  uint64_t occupancy =
      position.getAllPieces(white) | position.getAllPieces(black);
  uint64_t queenMask = position.getPieces()[position.getOppositeTurn()][queen];
  square square_;
  if (queenMask) {
    square_ = static_cast<square>(__builtin_ctzll(queenMask));
    return (getRookAttackMask(square_, occupancy) |
            getBishopAttackMask(square_, occupancy));
  }
  return 0b0ull;
}
uint64_t MoveGeneration::generateKnightAttackMaps() {
  uint64_t remainingKnights =
      position.getPieces()[position.getOppositeTurn()][knight];
  square square_;
  uint64_t generatedAttacks = 0b0ull;
  while (remainingKnights) {
    square_ = static_cast<square>(__builtin_ctzll(remainingKnights));
    generatedAttacks |= knightLookUpTable[square_];
    remainingKnights ^= (0b1ull << square_);
  }
  return generatedAttacks;
}
uint64_t MoveGeneration::generateLeftPawnAttackMaps() {
  uint64_t pawns = position.getPieces()[position.getTurn()][pawn];

  switch (position.getOppositeTurn()) {
  case white:
    return ((pawns << 9) & (~A_FILE));
    break;
  case black:
    return ((pawns >> 7) & (~A_FILE));
    break;
  default:
    throw std::out_of_range("invalid color");
    break;
  }
}
uint64_t MoveGeneration::generateRightPawnAttackMaps() {
  uint64_t pawns = position.getPieces()[position.getTurn()][pawn];
  switch (position.getOppositeTurn()) {
  case white:
    return ((pawns << 7) & (~H_FILE));
    break;
  case black:
    return ((pawns >> 9) & (~H_FILE));
    break;
  default:
    throw std::out_of_range("invalid color");
    break;
  }
}
uint64_t MoveGeneration::generateAllAttackMaps() {
  return generateBishopAttackMaps() | generateKnightAttackMaps() |
         generateLeftPawnAttackMaps() | generateRightPawnAttackMaps() |
         generateQueenAttackMaps() | generateRookAttackMaps();
}
// generate Legal Move related operations;
void MoveGeneration::generateLegalKingMoves() {
  uint64_t attackedSquares = generateAllAttackMaps();
}
// TODO: finish implementing this;
void MoveGeneration::generateOrderedMoves() { generateAllMoves(); }
MoveList &MoveGeneration::getMoves() { return moveList; }
int MoveGeneration::getNumberOfMoves() const { return moveList.size(); }
bool MoveGeneration::isPseudoLegal(const Move &move) {
  generateAllMoves();
  for (const Move &pseudoLegalMove : moveList) {
    if (pseudoLegalMove.getMoveInt() == move.getMoveInt()) {
      return true;
    }
  }
  return false;
}