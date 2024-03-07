#include "loader.hpp"
std::string pieceToStr(Piece pieceType) {
  switch (pieceType) {
  case BISHOP:
    return "bishop";
    break;
  case QUEEN:
    return "queen";
    break;
  case KING:
    return "king";
    break;
  case PAWN:
    return "pawn";
    break;
  case KNIGHT:
    return "knight";
    break;
  case ROOK:
    return "rook";
    break;
  default:
    std::cerr << "invlaid piece type";
    throw std::range_error("invalid piece type");
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
  std::string fullDir = getExecutableDirectory();
  fileName = "/mask_cache/" + fileName;
  fileName = fullDir + fileName;
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

  std::string fullDir = getExecutableDirectory();
  fileName = "/mask_cache/" + fileName;
  fileName = fullDir + fileName;
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
std::vector<magicTbls> fileToLookUpsVec(Piece pieceType) {
  std::string pieceNameStr = pieceToStr(pieceType);
  std::string shiftFile = pieceNameStr + "_shifts.txt";
  std::string masksFile = pieceNameStr + "_masks.txt";
  std::string magicNumFile = pieceNameStr + "_magic_numbers.txt";
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

std::string getExecutableDirectory() {
  char path[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);

  std::string fullPath(path, (count > 0) ? count : 0);

  // Find the last '/' character and remove everything after that to get the
  // directory
  size_t lastSlashPos = fullPath.find_last_of("/");
  if (lastSlashPos != std::string::npos) {
    return fullPath.substr(0, lastSlashPos);
  }

  // If for some reason the '/' character isn't found, return the full path as
  // fallback
  return fullPath;
}
std::string getExecutableName() {
  char path[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);

  std::string fullPath(path, (count > 0) ? count : 0);

  // Find the last '/' character and extract everything after that to get the
  // executable name
  size_t lastSlashPos = fullPath.find_last_of("/");
  if (lastSlashPos != std::string::npos) {
    return fullPath.substr(lastSlashPos + 1); // +1 to skip the '/'
  }

  // If for some reason the '/' character isn't found, return the full path as
  // fallback
  return fullPath;
}