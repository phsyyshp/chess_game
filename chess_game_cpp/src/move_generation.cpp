#include "move_generation.hpp"

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
      moveList.push_back(
          Move{from, to, piece::pawn, position.getTurn(), false});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case color::black:

    pawns = position.getPieces()[black][pawn];
    generatedMoves = (pawns >> 8) & (~ineligibleSquares);
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 8;
      moveList.push_back(
          Move{from, to, piece::pawn, position.getTurn(), false});
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
      moveList.push_back(
          Move{from, to, piece::pawn, position.getTurn(), false});
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
      moveList.push_back(
          Move{from, to, piece::pawn, position.getTurn(), false});
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
      moveList.push_back(Move{from, to, piece::pawn, position.getTurn(), true});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case black:
    eligibleSquares = position.getAllPieces(white);
    generatedMoves = ((pawns >> 7) & (~A_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 7;
      moveList.push_back(Move{from, to, piece::pawn, position.getTurn(), true});
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
      moveList.push_back(Move{from, to, piece::pawn, position.getTurn(), true});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  case black:
    eligibleSquares = position.getAllPieces(white);
    generatedMoves = ((pawns >> 9) & (~H_FILE)) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      from = to + 9;
      moveList.push_back(Move{from, to, piece::pawn, position.getTurn(), true});
      generatedMoves ^= (0b1ull << to);
    }
    break;
  default:
    break;
  }
}
void MoveGeneration::generateKnightMoves() {

  uint64_t eligibleSquares = ~position.getAllPieces(position.getTurn());
  uint64_t allPieces =
      position.getAllPieces(black) | position.getAllPieces(white);
  uint64_t remainingKnigths = position.getPieces()[position.getTurn()][knight];
  uint64_t generatedMoves;
  int from;
  int to;
  bool isCapture;
  while (remainingKnigths) {
    from = __builtin_ctzll(remainingKnigths);
    generatedMoves = knightLookUpTable[from] & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      isCapture = ((0b1ull << to) & allPieces) != 0;
      moveList.push_back(
          Move{from, to, piece::knight, position.getTurn(), isCapture});
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
  bool isCapture;
  while (remainingBishops) {
    from = static_cast<square>(__builtin_ctzll(remainingBishops));
    generatedMoves = getBishopAttackMask(from, occupancy) & eligibleSquares;

    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      isCapture = ((0b1ull << to) & occupancy) != 0;
      moveList.push_back(
          Move{from, to, piece::bishop, position.getTurn(), isCapture});
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
  bool isCapture;
  while (remainingRooks) {
    from = static_cast<square>(__builtin_ctzll(remainingRooks));
    generatedMoves = getRookAttackMask(from, occupancy) & eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);
      isCapture = ((0b1ull << to) & occupancy) != 0;
      moveList.push_back(
          Move{from, to, piece::rook, position.getTurn(), isCapture});
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
  bool isCapture;
  if (queenMask) {
    uint64_t generatedMoves = (getRookAttackMask(from, occupancy) |
                               getBishopAttackMask(from, occupancy)) &
                              eligibleSquares;
    while (generatedMoves) {
      to = __builtin_ctzll(generatedMoves);

      isCapture = ((0b1ull << to) & occupancy) != 0;
      moveList.push_back(
          Move{from, to, piece::queen, position.getTurn(), isCapture});
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
  bool isCapture;
  while (generatedMoves) {
    to = __builtin_ctzll(generatedMoves);
    isCapture = ((0b1ull << to) & allPieces) != 0;
    moveList.push_back(
        Move{from, to, piece::king, position.getTurn(), isCapture});
    generatedMoves ^= (0b1ull << to);
  }
}
void MoveGeneration::generateAllMoves() {
  generateKnightMoves();
  generateKingMoves();
  generateSinglePawnPushes();
  generateDoublePawnPushes();
  generateLeftPawnCaptures();
  generateRightPawnCaptures();
  generateRookMoves();
  generateBishopMoves();
  generateQueenMoves();
}
// TODO: finish implementing this;
//  void MoveGeneration::orderMoves(){

// }
MoveList MoveGeneration::getMoves() const { return moveList; }
int MoveGeneration::getNumberOfMoves() const { return moveList.size(); }
