#include "search.hpp"

// Constructors;

Search::Search() { clearKillerMoves(); };

void Search::clear() {
  clearKillerMoves();
  hits = 0;
  tt.clear();
}
void Search::clearKillerMoves() {

  Move invalidMove(A1, A1, false);
  for (int i = 0; i < MAX_DEPTH; i++) {
    for (int j = 0; j < MAX_KILLER_MOVES; j++) {
      killerMoves[j][i] = invalidMove;
    }
  }
}
// Getters;
int Search::getPly() const { return ply; }

// Searchers;

Move Search::getBestMove(const Position &position, int maxDepth_, int wtime,
                         int winc, int btime, int binc, bool isInfoOn_) {
  clearKillerMoves();
  ply = 0;
  nodes = 0ull;
  Move invalidMove(A1, A1, 0);
  pv = invalidMove;
  pvScore = 0;
  isTimeExeeded = false;
  start = std::chrono::high_resolution_clock::now();
  maxMoveDuration =
      getMaxMoveDuration(position.getTurn(), wtime, winc, btime, binc);
  isInfoOn = isInfoOn_;
  maxDepth = maxDepth_;
  return iterativeDeepening(position);
}
Move Search::iterativeDeepening(const Position &position) {
  int timeSpent = 0;
  int depth = 1;
  int16_t score;
  while ((depth <= maxDepth) && (timeSpent < maxMoveDuration)) {
    int16_t alpha = -MAX_SCORE;
    int16_t beta = MAX_SCORE;

    score = search(alpha, beta, depth, position, true);
    timeSpent = countTime(start);
    if (isInfoOn) {
      std::cout << "info "
                << "depth " << depth << " time " << timeSpent << " nodes "
                << nodes << " nps " << nodes / (timeSpent + 1) * 1000
                << " tthits " << hits << " hashfull " << tt.fullness()
                << " score cp " << score << " pv " << bestMove.toStr() << '\n';
    }
    depth++;
  }
  return bestMove;
}

Move Search::searchRoot(int depth, const Position &position) {
  int16_t alpha = -MAX_SCORE;
  int16_t beta = MAX_SCORE;
  int16_t originalAlpha = alpha;

  // hashEntry entry = tt.get(position.getZobrist());
  // if (entry.zobristKey == position.getZobrist() && entry.depth >= depth &&
  //     ply != 0) {
  //   if (entry.flag == nodeType::EXACT) {
  //     return entry.move;
  //   }
  //   if (entry.flag == nodeType::LOWERBOUND) {
  //     alpha = std::max(alpha, static_cast<int>(entry.score));
  //   } else if (entry.flag == nodeType::UPPERBOUND) {
  //     beta = std::min(beta, static_cast<int>(entry.score));
  //   }
  //   if (alpha >= beta) {
  //     return entry.move;
  //   }
  // }
  int timeSpent = 0;
  int16_t score = -MAX_SCORE;
  Move bestMove(A1, A1, 0); // invalid move
  Evaluation eval(position);
  Position tempPosition;
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
      score = std::max(score, static_cast<int16_t>(-alphaBeta(
                                  -beta, -alpha, depth - 1, tempPosition)));
      ply--;
      if (score > alpha) {
        alpha = score;
        bestMove = movegen.getMoves()[j];
      }
      if (alpha >= beta) {
        pvScore = score;
        storeKillerMove(movegen.getMoves()[j], ply);
        bestMove = movegen.getMoves()[j];
        break;
      }
    }
  }
  pvScore = score;

  if (score <= originalAlpha) {
    tt.add(hashEntry{position.getZobrist(), depth, score, nodeType::UPPERBOUND,
                     bestMove});
  } else if (score >= beta) {
    tt.add(hashEntry{position.getZobrist(), depth, score, nodeType::LOWERBOUND,
                     bestMove});
  } else {

    tt.add(hashEntry{position.getZobrist(), depth, score, nodeType::EXACT,
                     bestMove});
  }

  return bestMove;
}
int16_t Search::search(int16_t alpha, int16_t beta, int depthLeft,
                       const Position &position, bool isRoot) {

  int16_t originalAlpha = alpha;

  hashEntry entry = tt.get(position.getZobrist());
  if (entry.zobristKey == position.getZobrist() && entry.depth >= depthLeft &&
      !isRoot) {
    if (entry.flag == nodeType::EXACT) {
      return entry.score;
    }
    if (entry.flag == nodeType::LOWERBOUND) {
      alpha = std::max(alpha, entry.score);
    } else if (entry.flag == nodeType::UPPERBOUND) {
      beta = std::min(beta, entry.score);
    }
    if (alpha >= beta) {
      return entry.score;
    }
  }
  if (depthLeft == 0) {
    return quiesce(alpha, beta, position);
  }
  Position tempPosition;
  MoveGeneration movegen(position);
  int16_t score = -MAX_SCORE;
  Move move_(A1, A1, 0); // invalid move;
  movegen.generateAllMoves();
  int moveCounter = 0;
  scoreMoves(movegen.getMoves(), position);
  for (int j = 0; j < movegen.getMoves().size(); j++) {
    // if (nodes % 4096 == 0) {
    if (isRoot) {
      if (isTimeExeeded) {
        // isTimeExeeded can not be true before getting first pv because at
        // depth 0 alpha beta only runs qsearch and it can not  switch this time
        // flag.
        break;
      }
    } else if (countTime(start) > maxMoveDuration) {
      isTimeExeeded = true; // data member;
      break;
    }
    // }
    pickMove(movegen.getMoves(), j);
    tempPosition = position;
    if (tempPosition.makeMove(movegen.getMoves()[j])) {
      move_ = movegen.getMoves()[j];
      moveCounter++;
      ply++;
      nodes++;
      score = std::max(
          score, static_cast<int16_t>(-search(-beta, -alpha, depthLeft - 1,
                                              tempPosition, false)));
      ply--;
      alpha = std::max(alpha, score);
      if (alpha >= beta) {
        storeKillerMove(movegen.getMoves()[j], ply);
        break;
      }
    }
  }
  if (moveCounter == 0 && position.isInCheck()) {
    return -MAX_SCORE + ply;
  } else if (moveCounter == 0) {
    return 0;
  }

  if (score <= originalAlpha) {
    tt.add(hashEntry{position.getZobrist(), depthLeft, score,
                     nodeType::UPPERBOUND, move_});
  } else if (score >= beta) {
    tt.add(hashEntry{position.getZobrist(), depthLeft, score,
                     nodeType::LOWERBOUND, move_});
  } else {
    tt.add(hashEntry{position.getZobrist(), depthLeft, score, nodeType::EXACT,
                     move_});
  }
  if (isRoot) {
    bestMove = move_;
  }
  return score;
}

int16_t Search::quiesce(int16_t alpha, int16_t beta, const Position &position) {
  Evaluation eval(position);
  Position tempPosition;
  MoveGeneration movegen(position);
  int16_t score = -MAX_SCORE;
  int16_t standingPat = eval.evaluate();
  if (standingPat >= beta) {
    return beta;
  }
  if (alpha < standingPat) {
    alpha = standingPat;
  }

  movegen.generateAllMoves();
  MoveList capturedOrPromoMoves = movegen.getMoves().getCapturedOrPromoMoves();
  scoreMoves(capturedOrPromoMoves, position);
  for (int j = 0; j < capturedOrPromoMoves.size(); j++) {
    // if (nodes % 4096 == 0) {
    if (countTime(start) > maxMoveDuration) {
      // isTimeExeeded = true; // data member;
      break;
    }
    // }
    pickMove(capturedOrPromoMoves, j);
    tempPosition = position;
    if (tempPosition.makeMove(capturedOrPromoMoves[j])) {
      ply++;
      score = -quiesce(-beta, -alpha, tempPosition);
      nodes++;
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

// Move ordering;

// BE CAREFUL pass by reference without const;
// FIX ME: maybe a bug here due to value overflow;

void Search::scoreMoves(MoveList &moveList_, const Position &position) {
  for (Move &move : moveList_) {
    uint moveScore = 0;
    int i = 0;
    Move ttMove = tt.getMove(position.getZobrist());
    if (move.getMoveInt() == ttMove.getMoveInt()) {
      if (tt.get(position.getZobrist()).zobristKey == position.getZobrist()) {
        hits++;
        move.setScore(MVV_LVA_OFFSET + TT_MOVE_SORT_VALUE);
        // std::cout << move.toStr() << "\n";
      }
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

int Search::countTime(std::chrono::high_resolution_clock::time_point start) {
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  return elapsed.count();
}
int Search::getMaxMoveDuration(Color turn, int wtime, int winc, int btime,
                               int binc) const {
  switch (turn) {
  case WHITE:
    return wtime / 20 + winc / 2;
    break;
  case BLACK:
    return btime / 20 + binc / 2;
    break;
  default:
    std::cerr << "invalid color\n";
    return 0;
    break;
  }
}