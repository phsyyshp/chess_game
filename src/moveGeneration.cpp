#include "moveGeneration.hpp"
// generate moves;
void MoveGeneration::generateSinglePawnPushes() {
  uint to, from;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[position.getTurn()][PAWN];
  switch (position.getTurn()) {
  case Color::WHITE:
    generatedMoves =
        ((pawns << 8) & (~position.getOccupancy())) & (~RANK_8_MASK);
    break;
  case Color::BLACK:
    generatedMoves =
        ((pawns >> 8) & (~position.getOccupancy())) & (~RANK_1_MASK);
    break;
  default:
    break;
  }
  while (generatedMoves) {
    to = __builtin_ctzll(generatedMoves);
    from = to - COLOR_TO_PUSH_FORWARD[position.getTurn()];
    moveList.push_back(Move{from, to, QUIET_MOVE});
    generatedMoves &= generatedMoves - 1;
  }
}
void MoveGeneration::generateDoublePawnPushes() {
  uint64_t generatedMoves, oneStepMoves;
  uint to, from;

  switch (position.getTurn()) {
  case WHITE:

    oneStepMoves = ((position.getPieces()[position.getTurn()][PAWN] &
                     INITIAL_PAWN_RANK_MASK[position.getTurn()])
                    << 8) &
                   (~position.getOccupancy());
    generatedMoves = (oneStepMoves << 8) & (~position.getOccupancy());
    break;
  case BLACK:
    oneStepMoves = ((position.getPieces()[position.getTurn()][PAWN] &
                     INITIAL_PAWN_RANK_MASK[position.getTurn()]) >>
                    8) &
                   (~position.getOccupancy());
    generatedMoves = (oneStepMoves >> 8) & (~position.getOccupancy());
    break;
  default:
    break;
  }
  while (generatedMoves) {
    to = __builtin_ctzll(generatedMoves);
    from = to - COLOR_TO_PUSH_TWO_FORWARD[position.getTurn()];
    moveList.push_back(Move{from, to, DOUBLE_PAWN_PUSH});
    generatedMoves &= generatedMoves - 1;
  }
}
void MoveGeneration::generateLeftPawnCaptures() {
  uint64_t eligibleSquares;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[position.getTurn()][PAWN];
  uint to, from;

  switch (position.getTurn()) {
  case WHITE:
    eligibleSquares = position.getAllPieces(BLACK);
    generatedMoves =
        (((pawns << 9) & (~A_FILE)) & eligibleSquares) & (~RANK_8_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 9;
      moveList.push_back(Move{from, to, CAPTURE});
      generatedMoves &= generatedMoves - 1;
    }
    break;
  case BLACK:
    eligibleSquares = position.getAllPieces(WHITE);
    generatedMoves =
        (((pawns >> 7) & (~A_FILE)) & eligibleSquares) & (~RANK_1_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 7;
      moveList.push_back(Move{from, to, CAPTURE});
      generatedMoves &= generatedMoves - 1;
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateRightPawnCaptures() {
  uint64_t eligibleSquares;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[position.getTurn()][PAWN];
  uint to, from;

  switch (position.getTurn()) {
  case WHITE:
    eligibleSquares = position.getAllPieces(BLACK);
    generatedMoves =
        (((pawns << 7) & (~H_FILE)) & eligibleSquares) & (~RANK_8_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 7;
      moveList.push_back(Move{from, to, CAPTURE});
      generatedMoves &= generatedMoves - 1;
    }
    break;
  case BLACK:
    eligibleSquares = position.getAllPieces(WHITE);
    generatedMoves =
        (((pawns >> 9) & (~H_FILE)) & eligibleSquares) & (~RANK_1_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 9;
      moveList.push_back(Move{from, to, CAPTURE});
      generatedMoves &= generatedMoves - 1;
    }
    break;
  default:
    break;
  }
}

void MoveGeneration::generateSinglePawnPromotions() {
  uint64_t generatedMoves;
  uint64_t pawns;
  uint to, from;
  switch (position.getTurn()) {
  case Color::WHITE:
    pawns = position.getPieces()[WHITE][PAWN];
    generatedMoves =
        ((pawns << 8) & (~position.getOccupancy())) & (RANK_8_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 8;
      moveList.push_back(Move{from, to, MoveType::bishopPromotion});
      moveList.push_back(Move{from, to, MoveType::queenPromotion});
      moveList.push_back(Move{from, to, MoveType::rookPromotion});
      moveList.push_back(Move{from, to, MoveType::knightPromotion});

      generatedMoves &= generatedMoves - 1;
    }
    break;
  case Color::BLACK:

    pawns = position.getPieces()[BLACK][PAWN];
    generatedMoves =
        ((pawns >> 8) & (~position.getOccupancy())) & (RANK_1_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 8;
      generatedMoves &= generatedMoves - 1;
      moveList.push_back(Move{from, to, MoveType::bishopPromotion});
      moveList.push_back(Move{from, to, MoveType::queenPromotion});
      moveList.push_back(Move{from, to, MoveType::rookPromotion});
      moveList.push_back(Move{from, to, MoveType::knightPromotion});
    }
    break;
  default:
    break;
  }
}

void MoveGeneration::generateLeftPawnPromoCaptures() {
  uint64_t eligibleSquares;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[position.getTurn()][PAWN];
  uint to, from;

  switch (position.getTurn()) {
  case WHITE:
    eligibleSquares = position.getAllPieces(BLACK);
    generatedMoves =
        (((pawns << 9) & (~A_FILE)) & eligibleSquares) & (RANK_8_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 9;
      moveList.push_back(Move{from, to, MoveType::bishopPromoCapture});
      moveList.push_back(Move{from, to, MoveType::queenPromoCapture});
      moveList.push_back(Move{from, to, MoveType::rookPromoCapture});
      moveList.push_back(Move{from, to, MoveType::knightPromoCapture});
      generatedMoves &= generatedMoves - 1;
    }
    break;
  case BLACK:
    eligibleSquares = position.getAllPieces(WHITE);
    generatedMoves =
        (((pawns >> 7) & (~A_FILE)) & eligibleSquares) & (RANK_1_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 7;
      moveList.push_back(Move{from, to, MoveType::bishopPromoCapture});
      moveList.push_back(Move{from, to, MoveType::queenPromoCapture});
      moveList.push_back(Move{from, to, MoveType::rookPromoCapture});
      moveList.push_back(Move{from, to, MoveType::knightPromoCapture});
      generatedMoves &= generatedMoves - 1;
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateRightPawnPromoCaptures() {
  uint64_t eligibleSquares;
  uint64_t generatedMoves;
  uint64_t pawns = position.getPieces()[position.getTurn()][PAWN];
  uint to, from;

  switch (position.getTurn()) {
  case WHITE:
    eligibleSquares = position.getAllPieces(BLACK);
    generatedMoves =
        (((pawns << 7) & (~H_FILE)) & eligibleSquares) & (RANK_8_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to - 7;
      moveList.push_back(Move{from, to, MoveType::bishopPromoCapture});
      moveList.push_back(Move{from, to, MoveType::queenPromoCapture});
      moveList.push_back(Move{from, to, MoveType::rookPromoCapture});
      moveList.push_back(Move{from, to, MoveType::knightPromoCapture});
      generatedMoves &= generatedMoves - 1;
    }
    break;
  case BLACK:
    eligibleSquares = position.getAllPieces(WHITE);
    generatedMoves =
        (((pawns >> 9) & (~H_FILE)) & eligibleSquares) & (RANK_1_MASK);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 9;
      moveList.push_back(Move{from, to, MoveType::bishopPromoCapture});
      moveList.push_back(Move{from, to, MoveType::queenPromoCapture});
      moveList.push_back(Move{from, to, MoveType::rookPromoCapture});
      moveList.push_back(Move{from, to, MoveType::knightPromoCapture});
      generatedMoves &= generatedMoves - 1;
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
  case Color::WHITE:
    fileEP = position.getGameState().getEnPassant();
    victimMask = 0b1ull << (fileEP + 8 * 4);
    eligibleSquares = ((victimMask) >> 1) & (~H_FILE);
    AttackerPawnMask = eligibleSquares & position.getPieces()[WHITE][PAWN];
    if (AttackerPawnMask) {
      moveList.push_back(Move{__builtin_ctzll(AttackerPawnMask),
                              __builtin_ctzll(AttackerPawnMask << 9),
                              EP_CAPTURE});
    }
    break;

  case Color::BLACK:
    fileEP = position.getGameState().getEnPassant();
    victimMask = 0b1ull << (fileEP + 8 * 3);

    eligibleSquares = ((victimMask) >> 1) & (~H_FILE);
    AttackerPawnMask = eligibleSquares & position.getPieces()[BLACK][PAWN];
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
  case Color::WHITE:
    fileEP = position.getGameState().getEnPassant();
    victimMask = 0b1ull << (fileEP + 8 * 4);
    eligibleSquares = ((victimMask) << 1) & (~A_FILE);
    AttackerPawnMask = eligibleSquares & position.getPieces()[WHITE][PAWN];
    if (AttackerPawnMask) {
      moveList.push_back(Move{__builtin_ctzll(AttackerPawnMask),
                              __builtin_ctzll(AttackerPawnMask << 7),
                              EP_CAPTURE});
    }
    break;

  case Color::BLACK:
    fileEP = position.getGameState().getEnPassant();
    victimMask = 0b1ull << (fileEP + 8 * 3);

    eligibleSquares = ((victimMask) << 1) & (~A_FILE);
    AttackerPawnMask = eligibleSquares & position.getPieces()[BLACK][PAWN];
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
  uint64_t remainingKnigths = position.getPieces()[position.getTurn()][KNIGHT];
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
      generatedMoves &= generatedMoves - 1;
    }
    remainingKnigths &= remainingKnigths - 1;
  }
}
void MoveGeneration::generateBishopMoves() {
  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  int64_t remainingBishops = position.getPieces()[position.getTurn()][BISHOP];
  uint64_t generatedMoves;

  Square from;
  uint to;
  uint isCaptureFlag;
  while (remainingBishops) {
    from = static_cast<Square>(__builtin_ctzll(remainingBishops));
    generatedMoves =
        getBishopAttackMask(from, position.getOccupancy()) & eligibleSquares;

    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      isCaptureFlag = !position.isEmpty(to) * 4;
      moveList.push_back(Move{from, to, isCaptureFlag});
      generatedMoves &= generatedMoves - 1;
    }
    remainingBishops &= remainingBishops - 1;
  }
}
void MoveGeneration::generateRookMoves() {
  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t remainingRooks = position.getPieces()[position.getTurn()][ROOK];
  Square from;
  uint to;
  uint64_t generatedMoves;
  uint isCaptureFlag;
  while (remainingRooks) {
    from = static_cast<Square>(__builtin_ctzll(remainingRooks));
    generatedMoves =
        getRookAttackMask(from, position.getOccupancy()) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      isCaptureFlag = !position.isEmpty(to) * 4;
      moveList.push_back(Move{from, to, isCaptureFlag});
      generatedMoves &= generatedMoves - 1;
    }
    remainingRooks &= remainingRooks - 1;
  }
}
void MoveGeneration::generateQueenMoves() {
  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t remainingQueens = position.getPieces()[position.getTurn()][QUEEN];
  uint to;
  uint isCaptureFlag;
  while (remainingQueens) {
    Square from = static_cast<Square>(__builtin_ctzll(remainingQueens));
    uint64_t generatedMoves =
        (getRookAttackMask(from, position.getOccupancy()) |
         getBishopAttackMask(from, position.getOccupancy())) &
        eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);

      isCaptureFlag = !position.isEmpty(to) * 4;
      moveList.push_back(Move{from, to, isCaptureFlag});
      generatedMoves &= generatedMoves - 1;
    }
    remainingQueens &= remainingQueens - 1;
  }
}
void MoveGeneration::generateKingMoves() {
  uint from = __builtin_ctzll(position.getPieces()[position.getTurn()][KING]);
  uint to;
  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t generatedMoves = kingLookUpTable[from] & eligibleSquares;
  uint64_t allPieces =
      position.getAllPieces(BLACK) | position.getAllPieces(WHITE);
  uint isCaptureFlag;
  while (generatedMoves) {
    to = __builtin_ctzll(generatedMoves);
    isCaptureFlag = !position.isEmpty(to) * 4;
    moveList.push_back(Move{from, to, isCaptureFlag});
    generatedMoves &= generatedMoves - 1;
  }
}
// TODO: Optimize it;
void MoveGeneration::generateCastling() {
  int color_ = position.getTurn();
  uint castlingRigths = position.getGameState().getCastlingRigths();
  switch (color_) {

  case WHITE:
    if ((!position.isInCheck(E1)) && (!position.isInCheck(D1)) &&
        (!position.isInCheck(C1)) &&
        (WHITE_QUEEN_SIDE_CASTLING_MASK & castlingRigths) &&
        ((position.getOccupancy() & WHITE_QUEEN_SIDE_CASTLING_RAY) == 0)) {

      moveList.push_back(Move{E1, C1, MoveType::queenCastle});
    }

    if ((!position.isInCheck(E1)) && (!position.isInCheck(F1)) &&
        (!position.isInCheck(G1)) &&
        (WHITE_KING_SIDE_CASTLING_MASK & castlingRigths) &&
        ((position.getOccupancy() & WHITE_KING_SIDE_CASTLING_RAY) == 0)) {

      moveList.push_back(Move{E1, G1, MoveType::kingCastle});
    }
    break;
  case BLACK:
    if ((!position.isInCheck(E8)) && (!position.isInCheck(D8)) &&
        (!position.isInCheck(C8)) &&
        (BLACK_QUEEN_SIDE_CASTLING_MASK & castlingRigths) &&
        ((position.getOccupancy() & BLACK_QUEEN_SIDE_CASTLING_RAY) == 0)) {

      moveList.push_back(Move{E8, C8, MoveType::queenCastle});
    }

    if ((!position.isInCheck(E8)) && (!position.isInCheck(F8)) &&
        (!position.isInCheck(G8)) &&
        (BLACK_KING_SIDE_CASTLING_MASK & castlingRigths) &&
        ((position.getOccupancy() & BLACK_KING_SIDE_CASTLING_RAY) == 0)) {

      moveList.push_back(Move{E8, G8, MoveType::kingCastle});
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
  generateCastling();
  generateSinglePawnPromotions();
  generateLeftPawnPromoCaptures();
  generateRightPawnPromoCaptures();
}
// generate attacks;

uint64_t MoveGeneration::generateRookAttackMaps() {
  uint64_t eligibleSquares = ~position.getAllPieces(position.getOppositeTurn());
  uint64_t remainingRooks =
      position.getPieces()[position.getOppositeTurn()][ROOK];
  Square square_;
  uint to;
  uint64_t generatedAttacks = 0b0ull;
  while (remainingRooks) {
    square_ = static_cast<Square>(__builtin_ctzll(remainingRooks));
    generatedAttacks |=
        getRookAttackMask(square_, position.getOccupancy()) & eligibleSquares;
    remainingRooks &= remainingRooks - 1;
  }
  return generatedAttacks;
}
uint64_t MoveGeneration::generateBishopAttackMaps() {
  uint64_t remainingBishops =
      position.getPieces()[position.getOppositeTurn()][BISHOP];
  Square square_;
  uint64_t generatedAttacks = 0b0ull;
  while (remainingBishops) {
    square_ = static_cast<Square>(__builtin_ctzll(remainingBishops));
    generatedAttacks |= getBishopAttackMask(square_, position.getOccupancy());
    remainingBishops &= remainingBishops - 1;
  }
  return generatedAttacks;
}
uint64_t MoveGeneration::generateQueenAttackMaps() {
  uint64_t queenMask = position.getPieces()[position.getOppositeTurn()][QUEEN];
  Square square_;
  if (queenMask) {
    square_ = static_cast<Square>(__builtin_ctzll(queenMask));
    return (getRookAttackMask(square_, position.getOccupancy()) |
            getBishopAttackMask(square_, position.getOccupancy()));
  }
  return 0b0ull;
}
uint64_t MoveGeneration::generateKnightAttackMaps() {
  uint64_t remainingKnights =
      position.getPieces()[position.getOppositeTurn()][KNIGHT];
  Square square_;
  uint64_t generatedAttacks = 0b0ull;
  while (remainingKnights) {
    square_ = static_cast<Square>(__builtin_ctzll(remainingKnights));
    generatedAttacks |= knightLookUpTable[square_];
    remainingKnights ^= (0b1ull << square_);
  }
  return generatedAttacks;
}
uint64_t MoveGeneration::generateLeftPawnAttackMaps() {
  uint64_t pawns = position.getPieces()[position.getTurn()][PAWN];

  switch (position.getOppositeTurn()) {
  case WHITE:
    return ((pawns << 9) & (~A_FILE));
    break;
  case BLACK:
    return ((pawns >> 7) & (~A_FILE));
    break;
  default:
    throw std::out_of_range("invalid color");
    break;
  }
}
uint64_t MoveGeneration::generateRightPawnAttackMaps() {
  uint64_t pawns = position.getPieces()[position.getTurn()][PAWN];
  switch (position.getOppositeTurn()) {
  case WHITE:
    return ((pawns << 7) & (~H_FILE));
    break;
  case BLACK:
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