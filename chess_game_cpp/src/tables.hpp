#include "loader.hpp"
// slider cache
const std::vector<magicTbls> rookTbls;
const std::vector<std::vector<uint64_t>> rookLookUpTables;
const std::vector<magicTbls> bishopTbls;
const std::vector<std::vector<uint64_t>> bishopLookUpTables;
// rest cache
const std::vector<uint64_t> knightLookUpTable;
const std::vector<std::vector<uint64_t>> pawnLookUpTable;
const std::vector<uint64_t> kingLookUpTable;
uint64_t getRookAttackMask(const square &sq, const uint64_t &occupancy);
uint64_t getBishopAttackMask(const square &sq, const uint64_t &occupancy);
