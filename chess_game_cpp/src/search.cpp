#include "search.hpp"

int Search::negaMax(int depth) {

  int score;
  Evaluation eval(position);
  Position tempPosition;
  if (depth == 0) {
    return eval.evaluate();
  }
  int max = -9999;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    tempPosition = position;
    position.makeMove(move);
    score = -negaMax(depth - 1);
    position = tempPosition;
    if (score > max) {
      max = score;
      bestMove = move;
    }
  }
  return max;
}
Move Search::getBestMove() const { return bestMove; }