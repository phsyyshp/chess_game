#include "Move.hpp"
int Move::getFrom() const { return moveNum & 0x3f; }
int Move::getTo() const { return (moveNum >> 6) & 0x3f; }
int Move::getColor() const { return (moveNum >> 12) & 0x3f; }
int Move::getPiece() const { return (moveNum >> 18) & 0x3f; }
bool Move::checkIsCapture() const { return (moveNum >> 21) == 1; }
