#include "gameState.hpp"

// getters
uint GameState::getTurn() const { return gameStateNum & 0x1; }
uint GameState::getCastlingRigths() const { return (gameStateNum >> 1) & 0xf; }
uint GameState::getEnPassant() const { return (gameStateNum >> 5) & 0xf; }
uint32_t GameState::getGameStateNum() const { return gameStateNum; }
// setters
void GameState::changeTurn() { gameStateNum ^= 0x1; }

void GameState::setCastlingRigths() {}
void GameState::setEnPassant() {}