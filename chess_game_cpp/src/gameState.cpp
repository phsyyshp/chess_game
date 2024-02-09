#include "gameState.hpp"

// getters
color GameState::getTurn() const {
  return static_cast<color>(gameStateNum & 0x1);
}
uint GameState::getCastlingRigths() const { return (gameStateNum >> 1) & 0xf; }
uint GameState::getEnPassant() const { return (gameStateNum >> 5) & 0x8; }
uint32_t GameState::getGameStateNum() const { return gameStateNum; }
// setters
void GameState::changeTurn() { gameStateNum ^= 0x1; }

void GameState::setCastlingRigths() {}
void GameState::setEnPassant(uint file) {
  gameStateNum = (gameStateNum & ~((0x8) << 5)) | (file << 5);
}