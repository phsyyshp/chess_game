#pragma once
#include "constants.hpp"
class GameState {
public:
  GameState()
      : gameStateNum((white & 0x1) | ((0b1111u << 1) & 0xf) |
                     ((0b0u << 5) & 0xf)){};
  // canWhiteCastleQueenSide, 1 0 0 0
  // canWhiteCastleKingSide,  0 1 0 0
  // canBlackCastleQueenSide, 0 0 1 0
  // canBlackCastleKingSide,  0 0 0 1
  GameState(color turn, const uint &castlingRigths, const uint &enPassant)
      : gameStateNum((turn & 0x1) | ((castlingRigths << 1) & 0xf) |
                     ((enPassant << 5) & 0xf)) {}
  // getters
  uint getTurn() const;
  uint getCastlingRigths() const;
  uint getEnPassant() const;
  uint32_t getGameStateNum() const;
  // setters
  void changeTurn();
  void setCastlingRigths();
  void setEnPassant();

private:
  uint32_t gameStateNum;
};