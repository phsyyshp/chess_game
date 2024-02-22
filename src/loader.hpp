#pragma once
#include "constants.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <vector>
std::string pieceToStr(piece pieceType);
std::vector<uint64_t> lineToNumsVec(const std::string &line);
std::vector<uint64_t> fileToVec(std::string fileName);
std::vector<std::vector<uint64_t>> fileToVec2(std::string fileName);
std::vector<magicTbls> fileToLookUpsVec(piece pieceType);
std::string getExecutableDirectory();

std::string getExecutableName();
