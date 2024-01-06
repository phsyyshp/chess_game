#include "constants.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
std::vector<uint64_t> lineToNumsVec(const std::string &line);
std::vector<uint64_t> readMagicNumbersToVec(piece pieceType);
std::vector<std::vector<uint64_t>> readLookUpTables(piece pieceType);
std::vector<uint64_t> readKnightLookUpTable();
std::vector<uint64_t> readWhitePawnLookUpTable();
std::vector<uint64_t> readBlackPawnLookUpTable();
std::vector<uint64_t> readKingLookUpTable();

int getLinearPosition(const uint64_t &position);
std::vector<int> positionToRowCol(const uint64_t &position);
int rookRelevantBits(const uint64_t &position);
// int bishopRelevantBits(const uint64_t &position);
int generateMagicIndex(const uint64_t &bitboard, const uint64_t &magicNumber,
                       int shiftBits);

uint64_t getAttackMask(const uint64_t &position, const uint64_t &bitboard,
                       const std::vector<uint64_t> &magicNumbers,
                       const std::vector<std::vector<uint64_t>> &lookUpTables,
                       piece pieceType);
