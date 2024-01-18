#include "bitoperations.hpp"
uint64_t northFill(uint64_t pawns) {
  pawns |= pawns << 8;
  pawns |= pawns << 16;
  pawns |= pawns << 32;
  return pawns;
}
uint64_t southFill(uint64_t pawns) {
  pawns |= pawns >> 8;
  pawns |= pawns >> 16;
  pawns |= pawns >> 32;
  return pawns;
}
uint64_t whiteFrontFill(uint64_t whitePawns) { return northFill(whitePawns); }
uint64_t blackFrontFill(uint64_t blackPawns) { return southFill(blackPawns); }
uint64_t whiteRearFill(uint64_t whitePawns) { return southFill(whitePawns); }
uint64_t blackRearFill(uint64_t blackPawns) { return northFill(blackPawns); }
uint64_t fileFill(uint64_t pawns) {
  return northFill(pawns) | southFill(pawns);
}
uint64_t eastOne(uint64_t bitboard) { return (~A_FILE) & (bitboard << 1); }
uint64_t westOne(uint64_t bitboard) { return (~H_FILE) & (bitboard >> 1); }
uint64_t eastAttackFileFill(uint64_t pawns) { return eastOne(fileFill(pawns)); }
uint64_t westAttackFileFill(uint64_t pawns) { return westOne(fileFill(pawns)); }
uint64_t noNeighborOnWestFile(uint64_t pawns) {
  return pawns & ~westAttackFileFill(pawns);
}
uint64_t noNeighborOnEastFile(uint64_t pawns) {
  return pawns & ~eastAttackFileFill(pawns);
}
uint64_t isolanis(uint64_t pawns) {
  return noNeighborOnEastFile(pawns) | noNeighborOnWestFile(pawns);
}
