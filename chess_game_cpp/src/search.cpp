#include "search.hpp"

// constructors;
Search::Search(const Position &p) : position(p) {
  // this is just zero;
  Move invalidMove(a1, a1, false);
  for (int i = 0; i < MAX_DEPTH; i++) {
    for (int j = 0; j < MAX_KILLER_MOVES; j++) {
      killerMoves[j][i] = invalidMove;
    }
  }
};

Search::Search(const Position &p, int timeLeftWhite_, int timeIncrementWhite_,
               int timeLeftBlack_, int timeIncrementBlack_)
    : position(p), timeLeftWhite(timeLeftWhite_),
      timeIncrementWhite(timeIncrementWhite_), timeLeftBlack(timeLeftBlack_),
      timeIncrementBlack(timeIncrementBlack_) {

  Move invalidMove(a1, a1, false);
  for (int i = 0; i < MAX_DEPTH; i++) {
    for (int j = 0; j < MAX_KILLER_MOVES; j++) {
      killerMoves[j][i] = invalidMove;
    }
  }
}
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
    if (position.makeMove(move)) {
      ply++;
      score = -negaMax(depth - 1);
      ply--;
      if (score > max) {
        max = score;
      }
    }
    position = tempPosition;
  }
  return max;
}
int Search::quiesce(int alpha, int beta) {
  Evaluation eval(position);
  Position tempPosition;
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  int score;
  int moveCounter = 0;
  int standingPat = eval.evaluate();
  // FIX IT: is this correct
  if (standingPat >= beta) {
    return beta;
  }
  if (alpha < standingPat) {
    alpha = standingPat;
  }
  for (const Move &move : movegen.getMoves().getCapturedMoves()) {
    tempPosition = position;
    if (position.makeMove(move)) {
      moveCounter++;
      ply++;
      score = -quiesce(-beta, -alpha);
      ply--;
      if (score >= beta) {
        position = tempPosition;
        return beta;
      }
      if (score > alpha) {
        alpha = score;
      }
    }
    position = tempPosition;
  }
  if (moveCounter == 0 && position.isInCheck()) {
    return INT16_MIN;
  }
  return alpha;
}
Move Search::search(int depth) {
  int score;
  Move bestMove;
  Evaluation eval(position);
  Position tempPosition;
  int max = INT16_MIN;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    tempPosition = position;
    if (position.makeMove(move)) {
      ply++;
      score = -negaMax(depth - 1);
      ply--;
      if (score > max) {
        max = score;
        bestMove = move;
        // std::cout << score << std::endl;
      }
    }
    position = tempPosition;
  }
  return bestMove;
}
Move Search::searchIt(int maxDepth, bool isInfoOn) {
  int depth = 1;
  int remainingTime = 0;
  int timeIncrement = 0;
  int timeSpent = 0;
  auto start = std::chrono::high_resolution_clock::now();

  Move bestMove(a1, a1, 0); // invalid move;
  color turn = position.getTurn();
  switch (turn) {
  case white:
    remainingTime = timeLeftWhite;
    timeIncrement = timeIncrementWhite;
    break;

  case black:
    remainingTime = timeLeftBlack;
    timeIncrement = timeIncrementBlack;
    break;
  default:
    break;
  }
  int maxMoveDuration = remainingTime / 20 + timeIncrement / 2;
  int moveDuration = 0;
  bool didSearchOccured = false;
  while ((depth <= maxDepth) && (timeSpent <= maxMoveDuration)) {
    bestMove = searchAB(depth, start, remainingTime, timeIncrement);
    if (isInfoOn) {
      std::cout << "info "
                << "depth " << depth << '\n';
    }
    depth++;
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    timeSpent = elapsed.count();
    didSearchOccured = true;
  }
  if (didSearchOccured) {
    return bestMove;
  } else {
    MoveGeneration movgen(position);
    movgen.generateAllMoves();
    std::cout << "info depth 1\n";
    Position tempPosition;
    tempPosition = position;
    for (const Move &move : movgen.getMoves()) {
      if (position.makeMove(move)) {
        bestMove = move;
        position = tempPosition;
        std::cout << "legal\n";
        return bestMove;
      }

      std::cout << "nolegal\n";
      position = tempPosition;
    }
    return bestMove;
  }
}

// TODO: Rigorous testing;
Move Search::searchAB(int depth,
                      std::chrono::high_resolution_clock::time_point start,
                      int remainingTime, int timeIncrement) {
  int timeSpent = 0;
  int score;
  Move bestMove;
  Evaluation eval(position);
  Position tempPosition;
  int alpha = INT16_MIN;
  int beta = INT16_MAX;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    tempPosition = position;
    if (position.makeMove(move)) {
      ply++;
      score = -alphaBeta(-beta, -alpha, depth - 1);
      ply--;
      if (score >= beta) {
        position = tempPosition;
        return move;
      }
      if (score > alpha) {
        alpha = score;
        bestMove = move;
      }
    }
    position = tempPosition;

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    timeSpent = elapsed.count();
    // there may be a bug here
    if (timeSpent >= remainingTime / 20 + timeIncrement / 2) {
      return pv;
    }
  }
  pv = bestMove;
  return bestMove;
}

/*TODO:
-hash tables;
*/
int Search::alphaBeta(int alpha, int beta, int depthLeft) {
  Position tempPosition;
  MoveGeneration movgen(position);
  if (depthLeft == 0) {
    return quiesce(alpha, beta);
  }
  movgen.generateAllMoves();
  int score;
  scoreMoves(movgen.getMoves());
  int moveCounter = 0;
  // FIX IT: THERE MUST BE A BUG HERE
  for (int j = 0; j < movgen.getMoves().size(); j++) {
    pickMove(movgen.getMoves(), j);
    tempPosition = position;
    if (position.makeMove(movgen.getMoves()[j])) {
      moveCounter++;
      ply++;
      score = -alphaBeta(-beta, -alpha, depthLeft - 1);
      ply--;
      if (score >= beta) {
        storeKillerMove(movgen.getMoves()[j], ply);
        position = tempPosition;
        return beta;
      }
      if (score > alpha) {
        alpha = score;
      }
    }
    position = tempPosition;
  }
  if (moveCounter == 0 && position.isInCheck()) {
    return INT16_MIN;
  }
  return alpha;
}

// Move ordering;

// BE CAREFUL pass by reference without const;
// FIX ME: maybe a bug here due to value overflow;

void Search::scoreMoves(MoveList &moveList_) const {
  for (Move &move : moveList_) {
    int moveScore = 0;
    if (move.isCapture()) {
      moveScore = MVV_LVA_OFFSET + MVV_LVA[position.getPiece(move.getTo())]
                                          [position.getPiece(move.getFrom())];
      move.setScore(moveScore);
    } else {
      int i = 0;
      while (i < MAX_KILLER_MOVES && moveScore == 0) {
        if (move.getMoveInt() == killerMoves[i][ply].getMoveInt()) {
          // TODO: Be Careful about setting already set score;
          // FIX ME: maybe a bug here due to value overflow;
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

  if (!move_.isCapture()) {
    if (killerMoves[0][ply].getMoveInt() != move_.getMoveInt()) {
      killerMoves[1][ply] = killerMoves[0][ply];
      killerMoves[0][ply] = move_;
    }
  }
}