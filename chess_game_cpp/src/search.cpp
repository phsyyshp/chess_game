#include "search.hpp"
int Search::negaMax(int depth) {

  int score;
  Evaluation eval(position);
  Position tempPosition;
  if (depth == 0) {
    // temp
    return eval.evaluate();
  }
  int max = INT16_MIN;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    tempPosition = position;
    position.makeMove(move);
    score = -negaMax(depth - 1);
    position = tempPosition;
    if (score > max) {
      max = score;
    }
  }
  return max;
}
// TODO: incomplete
int Search::quiesce(int alpha, int beta) {
  Evaluation eval(position);
  int standingPat = eval.evaluate();
  if (standingPat >= beta) {
    return beta;
  }
  if (alpha < standingPat) {
    alpha = standingPat;
  }
}
Move Search::search(int depth) {
  int score;
  Move bestMove;
  Evaluation eval(position);
  Position tempPosition;
  if (depth <= 0) {
    throw std::out_of_range("depth must be positive int");
  }
  int max = INT16_MIN;
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
      // std::cout << score << std::endl;
    }
  }
  return bestMove;
}
// Careful pass by reference without const;
void Search::scoreMoves(MoveList &moveList_) { Evaluation eval; }