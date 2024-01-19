#include "constants.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#ifndef LOADER_HPP
#define LOADER_HPP
std::string pieceToStr(piece pieceType);
std::vector<uint64_t> lineToNumsVec(const std::string &line);
std::vector<uint64_t> fileToVec(std::string fileName);
std::vector<std::vector<uint64_t>> fileToVec2(std::string fileName);
std::vector<magicTbls> fileToLookUpsVec(piece pieceType);
#endif // LOADER_HPP
