#include "gameState.hpp"

// getters
Color GameState::getTurn() const {
  return static_cast<Color>(gameStateNum & 0x1);
}
uint GameState::getCastlingRigths() const { return (gameStateNum >> 1) & 0xf; }
uint GameState::getEnPassant() const { return (gameStateNum >> 5) & 0xf; }
uint32_t GameState::getGameStateNum() const { return gameStateNum; }
// setters
void GameState::changeTurn() { gameStateNum ^= 0x1; }

void GameState::setCastlingRigths(uint castlingRigths) {

  gameStateNum = (gameStateNum & ~((0xf) << 1)) | (castlingRigths << 1);
}
void GameState::setEnPassant(uint file) {
  gameStateNum = (gameStateNum & ~((0xf) << 5)) | (file << 5);
}

void GameState::clear() {
  gameStateNum = (WHITE & 0x1) | ((0b1111u) << 1) | ((0x8) << 5);
}