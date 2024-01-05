#include "loader.hpp"
// #include <fstream>
// #include <iostream>
// #include <vector>
std::vector<uint64_t> lineToNumsVec(const std::string &line) {
  std::vector<uint64_t> out;
  std::string number;
  for (auto c : line) {
    if (c != ' ') {
      number += c;
    } else {
      out.push_back(stoull(number));
      number.clear();
    }
  }
  return out;
}
std::vector<uint64_t> readMagicNumbersToVec(std::string piece) {
  std::string fileName = "maskCache/" + piece + "MagicNumbers.txt";
  std::vector<uint64_t> out;
  std::string temp;
  std::fstream in(fileName);
  if (in) {
    while (getline(in, temp)) {
      out.push_back(stoull(temp));
    }
  } else {
    std::cerr << "the " << fileName << " can not be opened" << std::endl;
  }
  in.close();
  return out;
}
std::vector<std::vector<uint64_t>> readLookUpTables(std::string piece) {
  std::string fileName = "maskCache/" + piece + "LookUpTables.txt";
  std::vector<std::vector<uint64_t>> lookUpTables;
  std::string line;
  std::fstream in(fileName);
  std::vector<uint64_t> lookUpTable;
  std::string number;
  if (in) {
    while (getline(in, line)) {
      lookUpTable = lineToNumsVec(line);
      lookUpTables.push_back(lookUpTable);
    }
  } else {
    std::cerr << "the " << fileName << " can not be opened" << std::endl;
  }
  in.close();
  return lookUpTables;
}
std::vector<u_int64_t> readKnightLookUpTable() {

  std::string pieceType = "knight";
  std::string fileName = "maskCache/" + pieceType + "LookUpTables.txt";
  std::string line;
  std::fstream in(fileName);
  std::vector<uint64_t> lookUpTable;
  std::string number;
  if (in) {
    while (getline(in, line)) {
      lookUpTable.push_back(stoll(line));
    }
  } else {
    std::cerr << "the " << fileName << " can not be opened" << std::endl;
  }
  in.close();
  return lookUpTable;
};

std::vector<uint64_t> readWhitePawnLookUpTable() {

  std::string pieceType = "whitePawn";
  std::string fileName = "maskCache/" + pieceType + "LookUpTables.txt";
  std::string line;
  std::fstream in(fileName);
  std::vector<uint64_t> lookUpTable(64);
  std::string number;
  int i = 0;
  if (in) {

    while (getline(in, line)) {
      lookUpTable[i] = stoll(line);
    }
  } else {
    std::cerr << "the " << fileName << " can not be opened" << std::endl;
  }
  in.close();
  return lookUpTable;
};
std::vector<uint64_t> readBlackPawnLookUpTable() {

  std::string pieceType = "blackPawn";
  std::string fileName = "maskCache/" + pieceType + "LookUpTables.txt";
  std::string line;
  std::fstream in(fileName);
  std::vector<uint64_t> lookUpTable;
  std::string number;
  if (in) {
    while (getline(in, line)) {
      lookUpTable.push_back(stoll(line));
    }
  } else {
    std::cerr << "the " << fileName << " can not be opened" << std::endl;
  }
  in.close();
  return lookUpTable;
};
std::vector<uint64_t> readKingLookUpTable() {
  std::string file_name = "mask_cache/king_look_up_table.txt";
  std::ifstream in(file_name);
  std::vector<uint64_t> kingLookUptable;
  std::string line;
  if (in) {
    while (getline(in, line)) {
      kingLookUptable.push_back(stoll(line));
    }
  } else {
    std::cerr << "file " + file_name + "is not found";
  }
  in.close();
  return kingLookUptable;
};

int getLinearPosition(const uint64_t &position) {
  // Warning!! it starts from 0, i.e. a1 square is 0;
  return __builtin_ctzll(position);
}
std::vector<int> positionToRowCol(const uint64_t &position) {

  int linearPosition = getLinearPosition(position);
  int column = linearPosition % 8;
  int row = (linearPosition / 8);
  return {row, column};
}
int rookRelevantBits(const uint64_t &position) {
  std::vector<int> rowColVec = positionToRowCol(position);
  int row = rowColVec[0];
  int column = rowColVec[1];
  int relevantBits;
  bool isAtSides = (column == 0 || column == 7);
  bool isAtTopbot = (row == 0 || row == 7);
  if ((isAtTopbot && !isAtSides) || (!isAtTopbot && isAtSides)) {
    relevantBits = 11;
  } else if (!isAtTopbot && !isAtSides) {
    relevantBits = 10;
  } else {
    relevantBits = 12;
  }
  return relevantBits;
}

int bishopRelevantBits(const uint64_t &position) {
  std::vector<int> rowColVec = positionToRowCol(position);
  int row = rowColVec[0];
  int column = rowColVec[1];

  // Diagonal distances: min distance to any corner in each diagonal direction
  int distNESW = std::min(row, column);         // Northeast-Southwest diagonal
  int distNWSE = std::min(row, 7 - column);     // Northwest-Southeast diagonal
  int distSENW = std::min(7 - row, column);     // Southeast-Northwest diagonal
  int distSWNE = std::min(7 - row, 7 - column); // Southwest-Northeast diagonal

  // Maximum distance to the edge in any diagonal direction
  int maxDiagonalDistance = std::max({distNESW, distNWSE, distSENW, distSWNE});

  // Total number of squares on the longest diagonal minus the square the bishop
  // is on
  int relevantBits = 2 * maxDiagonalDistance + 1;

  return relevantBits;
}

uint64_t generateMagicIndex(const uint64_t &bitboard,
                            const uint64_t &magicNumber, int shiftBits) {
  return (bitboard * magicNumber) >> shiftBits;
}
uint64_t getAttackMask(const u_int64_t &position, const u_int64_t &bitboard,
                       const std::vector<uint64_t> &magicNumbers,
                       const std::vector<std::vector<uint64_t>> &lookUpTables,
                       std::string pieceType) {
  // TODO: dont forget to generalize this to bishop;
  int shiftBits;
  if (pieceType == "rook") {

    shiftBits = 64 - rookRelevantBits(position);
  } else if (pieceType == "bishop") {

    shiftBits = 64 - bishopRelevantBits(position);
  } else {
    std::cerr << "wrong piece type";
    return 0;
  }
  int linearPosition = getLinearPosition(position);
  uint64_t magicIndex =
      generateMagicIndex(bitboard, magicNumbers[linearPosition], shiftBits);
  return lookUpTables[linearPosition][magicIndex];
}