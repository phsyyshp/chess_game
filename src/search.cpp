#include "search.hpp"

// constructors;
Search::Search() {
  // this is just zero;
  Move invalidMove(a1, a1, false);
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
int Search::negaMax(int depth, const Position &position) {

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
    if (tempPosition.makeMove(move)) {
      ply++;
      score = -negaMax(depth - 1, tempPosition);
      ply--;
      if (score > max) {
        max = score;
      }
    }
  }
  return max;
}
int Search::quiesce(int alpha, int beta, const Position &position) {
  Evaluation eval(position);
  Position tempPosition;
  MoveGeneration movegen(position);
  int score;
  int moveCounter = 0;
  int standingPat = eval.evaluate();
  if (standingPat >= beta) {
    return beta;
  }
  if (alpha < standingPat) {
    alpha = standingPat;
  }

  movegen.generateAllMoves();
  MoveList capturedMoves = movegen.getMoves().getCapturedMoves();
  scoreMoves(capturedMoves, position);
  for (int j = 0; j < capturedMoves.size(); j++) {
    pickMove(capturedMoves, j);
    tempPosition = position;
    if (tempPosition.makeMove(capturedMoves[j])) {
      moveCounter++;
      ply++;
      score = -quiesce(-beta, -alpha, tempPosition);
      ply--;
      if (score >= beta) {
        return beta;
      }
      if (score > alpha) {
        alpha = score;
      }
    }
  }

  return alpha;
}
Move Search::search(int depth, const Position &position) {
  int score;
  Move bestMove;
  Evaluation eval(position);
  Position tempPosition;
  int max = INT16_MIN;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    tempPosition = position;
    if (tempPosition.makeMove(move)) {
      ply++;
      score = -negaMax(depth - 1, tempPosition);
      ply--;
      if (score > max) {
        max = score;
        bestMove = move;
        // std::cout << score << std::endl;
      }
    }
  }
  return bestMove;
}
Move Search::searchIt(int maxDepth, bool isInfoOn, const Position &position,
                      int wtime, int winc, int btime, int binc) {
  ply = 0;
  int depth = 1;
  int remainingTime = 0;
  int timeIncrement = 0;
  int timeSpent = 0;
  auto start = std::chrono::high_resolution_clock::now();

  Move bestMove(a1, a1, 0); // invalid move;
  color turn = position.getTurn();
  switch (turn) {
  case white:
    remainingTime = wtime;
    timeIncrement = winc;
    break;

  case black:
    remainingTime = btime;
    timeIncrement = binc;
    break;
  default:
    break;
  }
  int maxMoveDuration = remainingTime / 20 + timeIncrement / 2;
  bool didSearchOccured = false;
  while ((depth <= maxDepth) && (timeSpent <= maxMoveDuration)) {
    bestMove = searchAB(depth, start, remainingTime, timeIncrement, position);
    pv = bestMove;
    if (isInfoOn) {
      std::cout << "info "
                << "depth " << depth << '\n';
      // << "depth " << depth << "ply" << ply + '\n';
    }
    depth++;
    timeSpent = countTime(start);
    didSearchOccured = true;
  }
  if (didSearchOccured) {
    return bestMove;
  } else {
    // std::cout << "la\n";
    MoveGeneration movgen(position);
    movgen.generateAllMoves();
    std::cout << "info depth 1\n";
    Position tempPosition;
    tempPosition = position;
    for (const Move &move : movgen.getMoves()) {
      if (tempPosition.makeMove(move)) {
        bestMove = move;
        // std::cout << "legal\n";
        return bestMove;
      }
      // std::cout << "nolegal\n";
    }
    return bestMove;
  }
}

// TODO: Rigorous testing;
Move Search::searchAB(int depth,
                      std::chrono::high_resolution_clock::time_point start,
                      int remainingTime, int timeIncrement,
                      const Position &position) {
  int timeSpent = 0;
  int score = 0;
  bool moveFound = false;
  int maxMoveDuration = remainingTime / 20 + timeIncrement / 2;
  Move bestMove;
  Evaluation eval(position);
  Position tempPosition;
  int alpha = INT16_MIN;
  int beta = INT16_MAX;
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  scoreMoves(movegen.getMoves(), position);
  int moveCounter = 0;
  for (int j = 0; j < movegen.getMoves().size(); j++) {
    pickMove(movegen.getMoves(), j);
    tempPosition = position;
    if (tempPosition.makeMove(movegen.getMoves()[j])) {
      if (!moveFound) {
        bestMove = movegen.getMoves()[j];
        moveFound = true;
      }
      ply++;

      score = -alphaBeta(-beta, -alpha, depth - 1, tempPosition);
      ply--;
      if (score >= beta) {
        storeKillerMove(movegen.getMoves()[j], ply);
        return movegen.getMoves()[j];
      }
      if (score > alpha) {
        alpha = score;
        bestMove = movegen.getMoves()[j];
      }
    }
    timeSpent = countTime(start);
    if (timeSpent >= maxMoveDuration) {
      if (depth > 1) {

        bestMove = pv;
      }
      break;
    }
  }
  return bestMove;
}

/*TODO:
-hash tables;
*/
int Search::alphaBeta(int alpha, int beta, int depthLeft,
                      const Position &position) {
  Position tempPosition;
  MoveGeneration movgen(position);
  if (depthLeft == 0) {
    return quiesce(alpha, beta, position);
  }
  movgen.generateAllMoves();
  int score = 0;
  scoreMoves(movgen.getMoves(), position);
  int moveCounter = 0;
  for (int j = 0; j < movgen.getMoves().size(); j++) {
    pickMove(movgen.getMoves(), j);
    tempPosition = position;
    if (tempPosition.makeMove(movgen.getMoves()[j])) {
      moveCounter++;
      ply++;
      score = -alphaBeta(-beta, -alpha, depthLeft - 1, tempPosition);
      ply--;
      if (score >= beta) {
        storeKillerMove(movgen.getMoves()[j], ply);
        return beta;
      }
      if (score > alpha) {
        alpha = score;
      }
    }
  }
  if (moveCounter == 0 && position.isInCheck()) {
    return -10000 + ply;
  }
  return alpha;
}

// Move ordering;

// BE CAREFUL pass by reference without const;
// FIX ME: maybe a bug here due to value overflow;

void Search::scoreMoves(MoveList &moveList_, const Position &position) const {
  for (Move &move : moveList_) {
    int moveScore = 0;
    if (move.isCapture()) {
      // TODO: becarefull with overflow here
      moveScore = MVV_LVA[position.getPiece(move.getTo())]
                         [position.getPiece(move.getFrom())];
      move.setScore(moveScore);
    } else {
      int i = 0;
      while (i < MAX_KILLER_MOVES && moveScore == 0) {
        if (move.getMoveInt() == killerMoves[i][ply].getMoveInt()) {
          // TODO: Be Careful about setting already set score;
          // FIX ME: maybe a bug here due to value overflow;
          moveScore = -((i + 1) * KILLER_VALUE);
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
int Search::countTime(std::chrono::high_resolution_clock::time_point start) {
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  return elapsed.count();
}