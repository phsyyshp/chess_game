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
  } else {
    return "\u0020";
  }
}
