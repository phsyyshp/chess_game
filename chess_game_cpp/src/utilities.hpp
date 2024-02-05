#include "constants.hpp"
#include <array>
#include <random>
std::string colorizeString(std::string, std::string, std::string);
std::string getPieceIcon(piece pieceType, color pieceColor);

std::array<std::string, 8> FENtoRanks(std::string FENstring);

int charToPiece(char c);

int chartoColor(char c);

std::array<std::array<uint64_t, 6>, 2> FENtoPieces(std::string FENstring);
uint64_t pseudoRandomNumberGenerator();
std::array<uint64_t, 781> generateZobristTable();
