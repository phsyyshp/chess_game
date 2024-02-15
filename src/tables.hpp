#ifndef TABLES_HPP
#define TABLES_HPP

#include "loader.hpp"
#include "utilities.hpp"
// slider cache

const std::vector<magicTbls> rookTbls = fileToLookUpsVec(rook);
const std::vector<std::vector<uint64_t>> rookLookUpTables =
    fileToVec2("rook_look_up_tables.txt");
const std::vector<magicTbls> bishopTbls = fileToLookUpsVec(bishop);
const std::vector<std::vector<uint64_t>> bishopLookUpTables =
    fileToVec2("bishop_look_up_tables.txt");
const std::vector<uint64_t> knightLookUpTable =
    fileToVec("knight_look_up_table.txt");
const std::vector<std::vector<uint64_t>> pawnLookUpTable = {
    fileToVec("white_pawn_look_up_table.txt"),
    fileToVec("black_pawn_look_up_table.txt")};
const std::vector<uint64_t> kingLookUpTable =
    fileToVec("king_look_up_table.txt");
const std::vector<uint64_t> adjacentFiles = fileToVec("adjacentFiles.txt");
const std::vector<uint64_t> sqToFiles = fileToVec("Files.txt");

// assuming the sequence as;
// wPx64,wBx64,wRx64,wQx64,wNx64,wPx64,bBx64,bRx64,bQx64,bNx64,sideTomove,castlingRigths,theFileOFValidEnPassant
const std::array<uint64_t, 781> zobristTable = generateZobristTable();

uint64_t getRookAttackMask(const square &sq, const uint64_t &occupancy);
uint64_t getBishopAttackMask(const square &sq, const uint64_t &occupancy);
#endif