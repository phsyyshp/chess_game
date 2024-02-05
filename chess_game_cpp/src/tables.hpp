#ifndef TABLES_HPP
#define TABLES_HPP

#include "loader.hpp"
#include "utilities.hpp"
// slider cache

const std::vector<magicTbls> rookTbls = fileToLookUpsVec(rook);
const std::vector<std::vector<uint64_t>> rookLookUpTables =
    fileToVec2("rook_look_up_tables");
const std::vector<magicTbls> bishopTbls = fileToLookUpsVec(bishop);
const std::vector<std::vector<uint64_t>> bishopLookUpTables =
    fileToVec2("bishop_look_up_tables");
const std::vector<uint64_t> knightLookUpTable =
    fileToVec("mask_cache/knight_look_up_table.txt");
const std::vector<std::vector<uint64_t>> pawnLookUpTable = {
    fileToVec("mask_cache/white_pawn_look_up_table.txt"),
    fileToVec("mask_cache/black_pawn_look_up_table.txt")};
const std::vector<uint64_t> kingLookUpTable =
    fileToVec("mask_cache/king_look_up_table.txt");
const std::vector<uint64_t> adjacentFiles =
    fileToVec("mask_cache/adjacentFiles.txt");
const std::vector<uint64_t> sqToFiles = fileToVec("mask_cache/Files.txt");

// assuming the sequence as;
// wPx64,wBx64,wRx64,wQx64,wNx64,wPx64,bBx64,bRx64,bQx64,bNx64,sideTomove,castlingRigths,theFileOFValidEnPassant
const std::array<uint64_t, 781> zobristTable = generateZobristTable();

uint64_t getRookAttackMask(const square &sq, const uint64_t &occupancy);
uint64_t getBishopAttackMask(const square &sq, const uint64_t &occupancy);
#endif