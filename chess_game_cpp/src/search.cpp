#include "search.hpp"

int Search::negaMax(int depth) {

  int score;
  Evaluation eval(position);
  Position tempPosition;
  if (depth == 0) {
    return eval.evaluate();
  }
  int max = INT16_MIN;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    tempPosition = position;
    position.makeMove(move);
    score = -negaMax(depth - 1);
    std::cout << score << std::endl;
    position = tempPosition;
    if (score > max) {
      max = score;
      std::cout << score << std::endl;
      bestMove = move;
      ;
    }
  }
  return max;
}
Move Search::getBestMove() const { return bestMove; }