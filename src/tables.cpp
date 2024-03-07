#include "tables.hpp"
uint64_t getRookAttackMask(const Square &sq, const uint64_t &occupancy) {
  uint64_t occupancyTemp = occupancy & rookTbls[sq].mask;

  auto magicIdx =
      (occupancyTemp * rookTbls[sq].magicNum) >> rookTbls[sq].shiftBit;
  return rookLookUpTables[sq][magicIdx];
}
uint64_t getBishopAttackMask(const Square &sq, const uint64_t &occupancy) {
  uint64_t occupancyTemp = occupancy & bishopTbls[sq].mask;

  auto magicIdx =
      (occupancyTemp * bishopTbls[sq].magicNum) >> bishopTbls[sq].shiftBit;
  return bishopLookUpTables[sq][magicIdx];
}
