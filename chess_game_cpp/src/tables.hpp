#ifndef TABLES_HPP
#define TABLES_HPP

#include "loader.hpp"
// slider cache

const std::vector<magicTbls> rookTbls = fileToLookUpsVec(rook);
const std::vector<std::vector<uint64_t>> rookLookUpTables =
    fileToVec2("rook_look_up_tables");
const std::vector<magicTbls> bishopTbls = fileToLookUpsVec(bishop);
const std::vector<std::vector<uint64_t>> bishopLookUpTables =
    fileToVec2("bishop_look_up_tables");
const std::vector<uint64_t> knightLookUpTable =
    fileToVec("knight_look_up_table");
const std::vector<std::vector<uint64_t>> pawnLookUpTable = {
    fileToVec("white_pawn_look_up_table"),
    fileToVec("black_pawn_look_up_table")};
const std::vector<uint64_t> kingLookUpTable = fileToVec("king_look_up_table");
uint64_t getRookAttackMask(const square &sq, const uint64_t &occupancy);
uint64_t getBishopAttackMask(const square &sq, const uint64_t &occupancy);
#endif