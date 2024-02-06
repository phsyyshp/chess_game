#include "utilities.hpp"
std::string colorizeString(std::string str, std::string fgASCII,
                           std::string bgASCII) {

  std::string colorPrefix = ("\x1b[;" + fgASCII + "m\x1b[" + bgASCII + 'm');
  std::string colorizedString = colorPrefix + ' ' + str + ' ' + "\x1b[0m";

  return colorizedString;
}
std::string getPieceIcon(piece pieceType, color pieceColor) {
  if (pieceColor == white) {
    switch (pieceType) {
    case pawn:
      return "\u2659";
    case bishop:
      return "\u2657";
    case rook:
      return "\u2656";
    case queen:
      return "\u2655";
    case knight:
      return "\u2658";
    case king:
      return "\u2654";
    }
  } else if (pieceColor == black) { // black
    switch (pieceType) {
    case pawn:
      return "\u265F";
    case bishop:
      return "\u265D";
    case rook:
      return "\u265C";
    case queen:
      return "\u265B";
    case knight:
      return "\u265E";
    case king:
      return "\u265A";
    }
  }
  return "\u0020";
}
// put / at the end of the FEn
std::array<std::string, 8> FENtoRanks(std::string FENstring) {

  std::string rank;
  std::array<std::string, 8> out;
  int i = 0;
  for (auto c : FENstring) {

    if (c != '/') {
      rank += c;

    } else if (c == '/') {
      out[i] = rank;
      rank.clear();
      i++;
    }
  }
  return out;
}
int charToPiece(char c) {

  c = tolower(c);
  switch (c) {
  case 'p':
    return 0;
    break;

  case 'b':
    return 1;
    break;
  case 'r':
    return 2;
    break;
  case 'q':
    return 3;
    break;
  case 'n':
    return 4;
    break;
  case 'k':
    return 5;
    break;

  default:
    std::cerr << c << " is an illegal argument" << std::endl;
    throw std::invalid_argument("no valid chess piece with that letter");
    break;
  }
}
int chartoColor(char c) {
  if (isupper(c)) {
    return 0;
  } else {
    return 1;
  }
}
std::array<std::array<uint64_t, 6>, 2> FENtoPieces(std::string FENstring) {

  std::array<std::string, 8> ranks = FENtoRanks(FENstring);
  int col = 0;
  int row = 7;
  int pieceID;
  int colorID;
  int i = 0;
  std::array<std::array<uint64_t, 6>, 2> pieces = {
      {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}};

  for (auto rank : ranks) {
    col = 0;
    for (char c : rank) {
      if (isdigit(c)) {

        col += (c - '0');
      } else {
        pieceID = charToPiece(c);
        colorID = chartoColor(c);
        // std::cout << pieceID << colorID << std::endl;
        pieces[colorID][pieceID] |= (0b1ull << ((row)*8 + col));
        col++;
      }
    }
    row--;
  }
  return pieces;
}

uint64_t pseudoRandomNumberGenerator() {
  std::mt19937_64 rng(std::random_device{}());

  std::uniform_int_distribution<uint64_t> dist(
      std::numeric_limits<uint64_t>::min(),
      std::numeric_limits<uint64_t>::max());

  return dist(rng);
}

std::array<uint64_t, 781> generateZobristTable() {
  std::array<uint64_t, 781> zobristTable;
  for (int i = 0; i < 781; i++) {
    zobristTable[i] = pseudoRandomNumberGenerator();
  }
  return zobristTable;
}
