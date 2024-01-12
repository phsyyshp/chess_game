#include "loader.hpp"
// #include <fstream>
// #include <iostream>
// #include <vector>
std::string pieceToStr(piece pieceType) {
  switch (pieceType) {
  case bishop:
    return "bishop";
    break;
  case queen:
    return "queen";
    break;
  case king:
    return "king";
    break;
  case pawn:
    return "pawn";
    break;
  case knight:
    return "knight";
    break;
  case rook:
    return "rook";
    break;
  default:
    break;
  }
}
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
std::vector<uint64_t> fileToVec(std::string fileName) {
  // fileName = "mask_cache/" + fileName + ".txt";
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
std::vector<std::vector<uint64_t>> fileToVec2(std::string fileName) {
  fileName = "mask_cache/" + fileName + ".txt";
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
std::vector<magicTbls> fileToLookUpsVec(piece pieceType) {
  std::string pieceNameStr = pieceToStr(pieceType);
  std::string shiftFile = "mask_cache/" + pieceNameStr + "_shifts.txt";
  std::string masksFile = "mask_cache/" + pieceNameStr + "_masks.txt";
  std::string magicNumFile =
      "mask_cache/" + pieceNameStr + "_magic_numbers.txt";
  std::vector<uint64_t> shiftVec = fileToVec(shiftFile);
  std::vector<uint64_t> magicNumVec = fileToVec(magicNumFile);
  std::vector<uint64_t> masksVec = fileToVec(masksFile);
  std::vector<magicTbls> out;
  magicTbls tempLookUp;
  for (decltype(shiftVec.size()) i = 0; i < shiftVec.size(); i++) {
    tempLookUp.shiftBit = shiftVec[i];
    tempLookUp.magicNum = magicNumVec[i];
    tempLookUp.mask = masksVec[i];
    out.push_back(tempLookUp);
  }
  return out;
}

uint64_t getAttackMask(const square &sq, const uint64_t &occupancy,
                       const std::vector<std::vector<uint64_t>> &lookUpTables,
                       const std::vector<magicTbls> &magicTblsIn) {
  auto magicIdx =
      (occupancy * magicTblsIn[sq].magicNum) >> magicTblsIn[sq].shiftBit;
  return lookUpTables[sq][magicIdx];
}
