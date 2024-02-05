#include "search.hpp"

// constructors;
Search::Search(const Position &p) : position(p) {
  // this is just zero;
  Move invalidMove(a1, a1, pawn, white, false);
  for (int i = 0; i < MAX_DEPTH; i++) {
    for (int j = 0; j < MAX_KILLER_MOVES; j++) {
      killerMoves[j][i] = invalidMove;
    }
  }
};

// getters;
std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES>
Search::getKillerMoves() const {
  return killerMoves;
}
int Search::getPly() const { return ply; }

// Searchers;
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
    ply++;
    score = -negaMax(depth - 1);
    ply--;
    position = tempPosition;
    if (score > max) {
      max = score;
    }
  }
  return max;
}
int Search::quiesce(int alpha, int beta) {
  Evaluation eval(position);
  Position tempPosition;
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  int score;
  int standingPat = eval.evaluate();
  if (standingPat >= beta) {
    return beta;
  }
  if (alpha < standingPat) {
    alpha = standingPat;
  }
  for (const Move &move : movegen.getMoves().getCapturedMoves()) {
    tempPosition = position;
    position.makeMove(move);
    ply++;
    score = -quiesce(-beta, -alpha);
    ply--;
    position = tempPosition;
    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
    }
  }
  return alpha;
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
    ply++;
    score = -negaMax(depth - 1);
    position = tempPosition;
    ply--;
    if (score > max) {
      max = score;
      bestMove = move;
      // std::cout << score << std::endl;
    }
  }
  return bestMove;
}
// TODO: Rigirous testing;
Move Search::searchAB(int depth) {
  int score;
  Move bestMove;
  Evaluation eval(position);
  Position tempPosition;
  if (depth <= 0) {
    throw std::out_of_range("depth must be positive int");
  }
  int alpha = INT16_MIN;
  int beta = INT16_MAX;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    tempPosition = position;
    position.makeMove(move);
    ply++;
    score = -alphaBeta(-beta, -alpha, depth - 1);
    ply--;
    position = tempPosition;
    if (score >= beta) {
      // bestMove = move;
      return move;
      // std::cout << score << std::endl;
    }
    if (score > alpha) {
      alpha = score;
      bestMove = move;
    }
  }
  return bestMove;
}
/*TODO:
-killer moves;
-hash tables;
-iterative deepning;
*/
int Search::alphaBeta(int alpha, int beta, int depthLeft) {
  Position tempPosition;
  MoveGeneration movgen(position);
  movgen.generateAllMoves();
  if (depthLeft == 0) {
    return quiesce(alpha, beta);
  }
  int score;
  scoreMoves(movgen.getMoves());
  for (int j = 0; j < movgen.getMoves().size(); j++) {
    pickMove(movgen.getMoves(), j);
    tempPosition = position;
    position.makeMove(movgen.getMoves()[j]);
    ply++;
    score = -alphaBeta(-beta, -alpha, depthLeft - 1);
    position = tempPosition;
    ply--;
    if (score >= beta) {
      storeKillerMove(movgen.getMoves()[j], ply);
      return beta;
    }
    if (score > alpha) {
      alpha = score;
    }
  }
  return alpha;
}

// Move ordering;

// BE CAREFUL pass by reference without const;
void Search::scoreMoves(MoveList &moveList_) const {
  for (Move &move : moveList_) {
    int moveScore = 0;
    if (move.checkIsCapture()) {
      moveScore =
          MVV_LVA_OFFSET + MVV_LVA[move.getCaptured(position)][move.getPiece()];
      move.setScore(moveScore);
    } else {
      int i = 0;
      while (i < MAX_KILLER_MOVES && moveScore == 0) {
        if (move.getMoveInt() == killerMoves[i][ply].getMoveInt()) {
          // TODO: Be Careful about setting already set score;
          moveScore = MVV_LVA_OFFSET - ((i + 1) * KILLER_VALUE);
          move.setScore(moveScore);
        }
        i++;
      }
    }
  }
}
// BE CAREFUL pass by reference without const;
void Search::pickMove(MoveList &scoredMoveList_, int startingIdx) const {

  for (int i = startingIdx + 1; i < scoredMoveList_.size(); i++) {
    if (scoredMoveList_[i].getScore() >
        scoredMoveList_[startingIdx].getScore()) {
      scoredMoveList_.swap(startingIdx, i);
    }
  }
}
void Search::orderMoves(MoveList &movelist_) {}
// TODO: test
void Search::storeKillerMove(const Move &move_, int ply) {

  if (!move_.checkIsCapture()) {
    if (killerMoves[0][ply].getMoveInt() != move_.getMoveInt()) {
      killerMoves[1][ply] = killerMoves[0][ply];
      killerMoves[0][ply] = move_;
    }
  }
}