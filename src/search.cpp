// FIX IT: check if you used non reference for range where you need to change to
// element of container.
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

void Search::clear() {
  clearKillerMoves();
  tt.clear();
}
void Search::clearKillerMoves() {

  Move invalidMove(a1, a1, false);
  for (int i = 0; i < MAX_DEPTH; i++) {
    for (int j = 0; j < MAX_KILLER_MOVES; j++) {
      killerMoves[j][i] = invalidMove;
    }
  }
}
// void Search::flipGlobalAncientFlag() { globalAncientFlag =
// !globalAncientFlag; } getters;
// std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES>
// Search::getKillerMoves() const {
//   return killerMoves;
// }
int Search::getPly() const { return ply; }

// Searchers;

Move Search::getBestMove(const Position &position, int maxDepth_, int wtime,
                         int winc, int btime, int binc, bool isInfoOn_) {
  clearKillerMoves();
  ply = 0;
  nodes = 0ull;
  Move invalidMove(a1, a1, 0);
  pv = invalidMove;
  pvScore = 0;
  isTimeExeeded = false;
  start = std::chrono::high_resolution_clock::now();
  maxMoveDuration =
      getMaxMoveDuration(position.getTurn(), wtime, winc, btime, binc);
  isInfoOn = isInfoOn_;
  maxDepth = maxDepth_;
  return searchIt(position);
}
Move Search::searchIt(const Position &position) {
  int timeSpent = 0;
  int depth = 1;
  Move bestMove(a1, a1, 0); // invalid move;
  while ((depth <= maxDepth) && (timeSpent < maxMoveDuration)) {
    bestMove = searchAB(depth, position);
    pv = bestMove;
    timeSpent = countTime(start);
    if (isInfoOn) {
      std::cout << "info "
                << "depth " << depth << " time " << timeSpent << " nodes "
                << nodes << " nps " << nodes / (timeSpent + 1) * 1000
                << " score cp " << pvScore << " pv " << pv.toStr() << '\n';
    }
    depth++;
  }
  return bestMove;
}

Move Search::search(int depth, const Position &position) {
  int score = 0;
  Move bestMove(a1, a1, 0); // invalid move
  Evaluation eval(position);
  Position tempPosition;
  int max = INT16_MIN;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    if (isTimeExeeded) {
      return pv; // isTimeExeeded can not be true before getting first pv;
      break;
    }
    tempPosition = position;
    if (tempPosition.makeMove(move)) {
      nodes++;
      ply++;
      score = -negaMax(depth - 1, tempPosition);
      ply--;
      if (score > max) {
        max = score;
        bestMove = move;
      }
    }
  }
  pvScore = score;
  // std::cout << bestMove.toStr() << "\n";
  return bestMove;
}
int Search::negaMax(int depth, const Position &position) {

  int score = 0;
  int moveCounter = 0;
  uint64_t nodesSearched = 0;
  Evaluation eval(position);
  Position tempPosition;
  if (depth == 0) {
    return eval.evaluate();
  }
  int max = INT16_MIN;
  MoveGeneration movGen(position);
  movGen.generateAllMoves();
  for (Move move : movGen.getMoves()) {
    if (countTime(start) > maxMoveDuration) {
      isTimeExeeded = true; // data member;
      break;
    }
    tempPosition = position;
    if (tempPosition.makeMove(move)) {
      moveCounter++;
      ply++;
      score = -negaMax(depth - 1, tempPosition);
      nodesSearched++;
      ply--;
      if (score > max) {
        max = score;
      }
    }
  }
  if (moveCounter == 0 && position.isInCheck()) {
    return INT16_MIN + ply;
  } else if (moveCounter == 0) {
    return 0;
  }
  nodes += nodesSearched;
  return max;
}
Move Search::searchAB(int depth, const Position &position) {
  int timeSpent = 0;
  int score = 0;
  Move bestMove(a1, a1, 0); // invalid move
  Evaluation eval(position);
  Position tempPosition;
  int alpha = INT16_MIN;
  int beta = INT16_MAX;
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  scoreMoves(movegen.getMoves(), position);
  for (int j = 0; j < movegen.getMoves().size(); j++) {
    if (isTimeExeeded) {
      // isTimeExeeded can not be true before getting first pv because at depth
      // 0 alpha beta only runs qsearch and it can not  switch this time flag.
      return pv;
      break;
    }
    pickMove(movegen.getMoves(), j);
    tempPosition = position;
    if (tempPosition.makeMove(movegen.getMoves()[j])) {
      nodes++;
      ply++;
      score = -alphaBeta(-beta, -alpha, depth - 1, tempPosition);
      ply--;
      tt.add(hashEntry{position.getZobrist(), 0, beta, nodeType::ALPHA,
                       movegen.getMoves()[j]});
      if (score >= beta) {
        pvScore = score;
        storeKillerMove(movegen.getMoves()[j], ply);
        tt.add(hashEntry{position.getZobrist(), 0, beta, nodeType::BETA,
                         movegen.getMoves()[j]});
        return movegen.getMoves()[j];
      }
      if (score > alpha) {
        alpha = score;
        bestMove = movegen.getMoves()[j];
        tt.add(hashEntry{position.getZobrist(), 0, beta, nodeType::EXACT,
                         movegen.getMoves()[j]});
      }
    }
  }
  pvScore = score;
  return bestMove;
}
int Search::alphaBeta(int alpha, int beta, int depthLeft,
                      const Position &position) {
  hashEntry entry = tt.get(position.getZobrist());
  if (entry.depth >= depthLeft && entry.zobristKey == position.getZobrist()) {
    if (entry.flag == nodeType::EXACT) {
      return entry.score;
    } else if (entry.flag == nodeType::ALPHA && entry.score > alpha) {
      alpha = entry.score;
    } else if (entry.flag == nodeType::BETA && entry.score < beta) {
      beta = entry.score;
    }
    if (alpha >= beta) {
      return entry.score;
    }
  }
  Position tempPosition;
  MoveGeneration movegen(position);
  uint64_t nodesSearched = 0;
  int score = 0;
  if (depthLeft == 0) {
    return quiesce(alpha, beta, position);
  }
  movegen.generateAllMoves();
  int moveCounter = 0;
  scoreMoves(movegen.getMoves(), position);
  for (int j = 0; j < movegen.getMoves().size(); j++) {
    if (countTime(start) > maxMoveDuration) {
      isTimeExeeded = true; // data member;
      break;
    }
    pickMove(movegen.getMoves(), j);
    tempPosition = position;
    if (tempPosition.makeMove(movegen.getMoves()[j])) {
      moveCounter++;
      ply++;
      nodesSearched++;
      score = -alphaBeta(-beta, -alpha, depthLeft - 1, tempPosition);
      ply--;
      tt.add(hashEntry{position.getZobrist(), depthLeft, alpha, nodeType::ALPHA,
                       movegen.getMoves()[j]});
      if (score >= beta) {
        nodes += nodesSearched;
        storeKillerMove(movegen.getMoves()[j], ply);
        tt.add(hashEntry{position.getZobrist(), depthLeft, beta, nodeType::BETA,
                         movegen.getMoves()[j]});
        return beta;
      }
      if (score > alpha) {
        alpha = score;
        tt.add(hashEntry{position.getZobrist(), depthLeft, score,
                         nodeType::EXACT, movegen.getMoves()[j]});
      }
    }
  }
  if (moveCounter == 0 && position.isInCheck()) {
    return INT16_MIN + ply;
  } else if (moveCounter == 0) {
    return 0;
  }
  nodes += nodesSearched;
  return alpha;
}

int Search::quiesce(int alpha, int beta, const Position &position) {
  Evaluation eval(position);
  Position tempPosition;
  MoveGeneration movegen(position);
  int score = 0;
  uint64_t nodesSearched = 0;
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
    if (countTime(start) > maxMoveDuration) {
      // isTimeExeeded = true; // data member;
      break;
    }
    pickMove(capturedMoves, j);
    tempPosition = position;
    if (tempPosition.makeMove(capturedMoves[j])) {
      ply++;
      score = -quiesce(-beta, -alpha, tempPosition);
      nodesSearched++;
      ply--;
      if (score >= beta) {
        nodes += nodesSearched;
        return beta;
      }
      if (score > alpha) {
        alpha = score;
      }
    }
  }

  nodes += nodesSearched;
  return alpha;
}

// Move ordering;

// BE CAREFUL pass by reference without const;
// FIX ME: maybe a bug here due to value overflow;

void Search::scoreMoves(MoveList &moveList_, const Position &position) const {
  for (Move &move : moveList_) {
    int moveScore = 0;
    int i = 0;

    if (move.getMoveInt() == tt.getMove(position.getZobrist()).getMoveInt()) {
      move.setScore(MVV_LVA_OFFSET + TT_MOVE_SORT_VALUE);
    } else if (move.isCapture()) {
      // TODO: becarefull with overflow here
      moveScore = MVV_LVA_OFFSET + MVV_LVA[position.getPiece(move.getTo())]
                                          [position.getPiece(move.getFrom())];
      move.setScore(moveScore);
    } else {
      i = 0;
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
void Search::storeKillerMove(const Move &move_, int ply) {

  if (!move_.isCapture()) {
    if (killerMoves[0][ply].getMoveInt() != move_.getMoveInt()) {
      killerMoves[1][ply] = killerMoves[0][ply];
      killerMoves[0][ply] = move_;
    }
  }
}
// void Search::orderMoves(MoveList &movelist_) {}
// TODO: test
int Search::countTime(std::chrono::high_resolution_clock::time_point start) {
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  return elapsed.count();
}
int Search::getMaxMoveDuration(color turn, int wtime, int winc, int btime,
                               int binc) const {
  switch (turn) {
  case white:
    return wtime / 20 + winc / 2;
    break;
  case black:
    return btime / 20 + binc / 2;
    break;
  default:
    std::cerr << "invalid color\n";
    return 0;
    break;
  }
}