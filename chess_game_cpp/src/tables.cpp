#include "tables.hpp"
uint64_t getRookAttackMask(const square &sq, const uint64_t &occupancy) {
  auto magicIdx = (occupancy * rookTbls[sq].magicNum) >> rookTbls[sq].shiftBit;
  return rookLookUpTables[sq][magicIdx];
}
uint64_t getBishopAttackMask(const square &sq, const uint64_t &occupancy) {
  auto magicIdx =
      (occupancy * bishopTbls[sq].magicNum) >> bishopTbls[sq].shiftBit;
  return bishopLookUpTables[sq][magicIdx];
}