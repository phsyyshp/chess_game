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
// BE CAREFUL pass by reference without const;
void Search::scoreMoves(MoveList &moveList_) {
  for (Move &move : moveList_) {
    int moveScore = MVV_LVA[move.getCaptured(position)][move.getPiece()];
    move.setScore(moveScore);
  }
}
// BE CAREFUL pass by reference without const;
void Search::pickMove(MoveList &scoredMoveList_, int startingIdx) {

  for (int i = startingIdx + 1; i << scoredMoveList_.size(); i++) {
    if (scoredMoveList_[i].getScore() >
        scoredMoveList_[startingIdx].getScore()) {
      scoredMoveList_.swap(startingIdx, i);
    }
  }
}
void Search::orderMoves(MoveList &movelist_) {}
int Search::alphaBeta(int alpha, int beta, int depthLeft) {

  if (depthLeft == 0) {
    return quiesce(alpha, beta);
  }
  int score;
  MoveGeneration movgen(position);
  movgen.generateAllMoves();
  // TODO: embed the moveordering here
  for (const Move &move : movgen.getMoves()) {
    score = -alphaBeta(-beta, -alpha, depthLeft - 1);
    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
    }
  }
  return alpha;
}