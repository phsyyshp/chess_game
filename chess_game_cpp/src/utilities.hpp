#include "constants.hpp"
#include <array>

std::string colorizeString(std::string, std::string, std::string);
std::string getPieceIcon(piece pieceType, color pieceColor);

std::array<std::string, 8> FENtoRanks(std::string FENstring);

int charToPiece(char c);

int chartoColor(char c);

std::array<std::array<uint64_t, 6>, 2> FENtoPieces(std::string FENstring);