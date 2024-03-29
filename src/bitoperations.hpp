#include "constants.hpp"
#include <iostream>
// fillers
uint64_t northFill(uint64_t pawns);
uint64_t southFill(uint64_t pawns);
uint64_t whiteFrontFill(uint64_t whitePawns);
uint64_t blackFrontFill(uint64_t blackPawns);
uint64_t whiteRearFill(uint64_t whitePawns);
uint64_t blackRearFill(uint64_t blackPawns);
uint64_t fileFill(uint64_t pawns);

// spaners
uint64_t whiteFrontSpan(uint64_t whitePawns);
uint64_t blackFrontSpan(uint64_t blackPawns);
uint64_t whiteRearSpan(uint64_t whitePawns);
uint64_t blackRearSpan(uint64_t blackPawns);

// ones
uint64_t eastOne(uint64_t bitboard);
uint64_t westOne(uint64_t bitboard);
uint64_t northOne(uint64_t bitboard);
uint64_t southOne(uint64_t bitboard);

uint64_t eastAttackFileFill(uint64_t pawns);
uint64_t westAttackFileFill(uint64_t pawns);
uint64_t noNeighborOnWestFile(uint64_t pawns);
uint64_t noNeighborOnEastFile(uint64_t pawns);
uint64_t isolanis(uint64_t pawns);

uint64_t whitePawnsInFrontOwn(uint64_t whitePawns);
uint64_t blackPawnsInFrontOwn(uint64_t blackPawns);