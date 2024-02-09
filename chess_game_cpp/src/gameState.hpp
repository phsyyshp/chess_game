#pragma once
#include "constants.hpp"
class GameState {
public:
  GameState() : gameStateNum((white & 0x1) | ((0b1111u) << 1) | ((0x8) << 5)){};
  GameState(color turn)
      : gameStateNum((turn & 0x1) | ((0b1111u << 1)) | ((0x8 << 5))){};

  // Castling Rights Table
  // +-----------------------+---------------------+
  // |       Permission      |      Castling       |
  // +-----------------------+---------------------+
  // | canWhiteCastleQueenSide | 1 0 0 0 |
  // | canWhiteCastleKingSide  | 0 1 0 0 |
  // | canBlackCastleQueenSide | 0 0 1 0 |
  // | canBlackCastleKingSide  | 0 0 0 1 |
  // +-----------------------+---------------------+

  GameState(color turn, const uint &castlingRigths, const uint &enPassant)
      : gameStateNum((turn & 0x1) | ((castlingRigths & 0xf) << 1) |
                     ((enPassant & 0xf) << 5)) {}
  // getters
  color getTurn() const;
  uint getCastlingRigths() const;
  uint getEnPassant() const;
  uint32_t getGameStateNum() const;
  // setters
  void changeTurn();
  void setCastlingRigths();
  void setEnPassant(uint file);

private:
  uint32_t gameStateNum;
};