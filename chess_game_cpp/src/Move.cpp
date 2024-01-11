#include "Move.hpp"
int Move::getFrom() { return moveNum & 0x3f; }
int Move::getTo() { return (moveNum >> 6) & 0x3f; }
int Move::getColor() { return (moveNum >> 12) & 0x3f; }
int Move::getPiece() { return (moveNum >> 18) & 0x3f; }
