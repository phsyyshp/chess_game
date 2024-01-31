#include "Move.hpp"
int Move::getFrom() const { return moveNum & 0x3f; }
int Move::getTo() const { return (moveNum >> 6) & 0x3f; }
int Move::getColor() const { return (moveNum >> 15) & 0b1; }
int Move::getPiece() const { return (moveNum >> 12) & 0b111; }
bool Move::checkIsCapture() const { return (moveNum >> 16) == 1; }
int Move::getScore() const { return (moveNum) >> 17; }
