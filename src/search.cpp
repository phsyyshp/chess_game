#include "search.hpp"

// Constructors;

Search::Search(std::array<uint64_t, 100> &pH) : positionHistory(pH) {
  clearKillerMoves();
};

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
  bestMove = invalidMove;
  pvScore = 0;
  isTimeExeeded = false;
  start = std::chrono::high_resolution_clock::now();
  maxMoveDuration =
      getMaxMoveDuration(position.getTurn(), wtime, winc, btime, binc);
  isInfoOn = isInfoOn_;
  maxDepth = maxDepth_;
  iterativeDeepening(position);
  return bestMove;
}
void Search::iterativeDeepening(const Position &position) {
  int timeSpent = 0;
  int depth = 1;
  int16_t score;
  while ((depth <= maxDepth) && (timeSpent < maxMoveDuration)) {
    int16_t alpha = -MAX_SCORE;
    int16_t beta = MAX_SCORE;

    score = search(alpha, beta, depth, position, true);
    pvScore = score;
    timeSpent = countTime(start);
    if (isInfoOn) {
      std::cout << "info "
                << "depth " << depth << " time " << timeSpent << " nodes "
                << nodes << " nps " << nodes / (timeSpent + 1) * 1000
                << " hashfull " << tt.fullness() << " score cp " << score
                << " pv " << bestMove.toStr() << '\n';
    }
    depth++;
  }
}

int16_t Search::search(int16_t alpha, int16_t beta, int depthLeft,
                       const Position &position, bool isRoot) {
  if (!isRoot && position.isThreeFoldRep(positionHistory)) {
    return 1 - (nodes & 2);
  }
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
  uint64_t tempZobristHash = positionHistory[(position.getPly() + 1) % 100];

  MoveGeneration movegen(position);
  int16_t bestScore = -MAX_SCORE, value = -MAX_SCORE;
  Move bestMoveOfNode(A1, A1, 0); // invalid move;
  movegen.generateAllMoves();
  int moveCounter = 0;
  scoreMoves(movegen.getMoves(), position);
  for (int j = 0; j < movegen.getMoves().size(); j++) {
    // if (nodes % 4096 == 0) {
    if (isRoot) {
      if (isTimeExeeded) {
        // isTimeExeeded can not be true before getting first pv because at
        // depth 0 alpha beta only runs qsearch and it can not  switch this
        // time flag.
        break;
      }
    } else if (countTime(start) > maxMoveDuration) {
      isTimeExeeded = true; // data member;
      break;
    }
    // }
    pickMove(movegen.getMoves(), j);
    tempPosition = position;
    if (tempPosition.makeMove(movegen.getMoves()[j], positionHistory)) {
      moveCounter++;
      ply++;
      nodes++;
      value = static_cast<int16_t>(
          -search(-beta, -alpha, depthLeft - 1, tempPosition, false));
      ply--;
      if (bestScore < value) {

        bestScore = value;
        bestMoveOfNode = movegen.getMoves()[j];

        if (value > alpha) {
          alpha = value;

          if (alpha >= beta) {
            break;
          }
        }
      }
    }
  }
  if (bestScore >= beta && !bestMoveOfNode.isCapture() &&
      !bestMoveOfNode.isPromo()) {
    storeKillerMove(bestMoveOfNode, ply);
  }
  positionHistory[(position.getPly() + 1) % 100] = tempZobristHash;
  // If time is up we need to quit the search before filling the tt table
  // with incomplete search results.
  if (isTimeExeeded) {
    return pvScore;
  }

  if (!isRoot && moveCounter == 0) {
    if (position.isInCheck()) {
      return -MAX_SCORE + ply;
    } else {
      return 0;
    }
  }
  if (bestScore <= originalAlpha) {
    tt.add(hashEntry{position.getZobrist(), depthLeft, bestScore,
                     nodeType::UPPERBOUND, bestMoveOfNode});
  } else if (bestScore >= beta) {
    tt.add(hashEntry{position.getZobrist(), depthLeft, bestScore,
                     nodeType::LOWERBOUND, bestMoveOfNode});
  } else {
    tt.add(hashEntry{position.getZobrist(), depthLeft, bestScore,
                     nodeType::EXACT, bestMoveOfNode});
  }
  if (isRoot) {
    bestMove = bestMoveOfNode;
  }
  return bestScore;
}

int16_t Search::quiesce(int16_t alpha, int16_t beta, const Position &position) {
  hashEntry entry = tt.get(position.getZobrist());

  if (entry.zobristKey == position.getZobrist()) {
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
  uint16_t originalAlpha = alpha;
  bool isComplete = true;
  int moveCounter = 0;
  Evaluation eval(position);
  Position tempPosition;
  uint64_t tempZobristHash = positionHistory[(position.getPly() + 1) % 100];
  Move bestMoveOfNode(A1, A1, 0);
  MoveGeneration movegen(position);
  // int16_t bestScore = -MAX_SCORE;
  int16_t value;
  int16_t bestScore = eval.evaluate();
  if (alpha < bestScore) {
    alpha = bestScore;
  }
  if (alpha >= beta) {
    return bestScore;
  }

  movegen.generateAllMoves();
  MoveList capturedOrPromoMoves = movegen.getMoves().getCapturedOrPromoMoves();
  scoreMoves(capturedOrPromoMoves, position);
  for (int j = 0; j < capturedOrPromoMoves.size(); j++) {
    // if (nodes % 4096 == 0) {
    // FIX IT:  think about this time managment better;
    if (countTime(start) > maxMoveDuration) {
      isComplete = false;
      break;
    }
    // }
    pickMove(capturedOrPromoMoves, j);
    tempPosition = position;
    if (tempPosition.makeMove(capturedOrPromoMoves[j], positionHistory)) {
      moveCounter++;
      ply++;
      value = -quiesce(-beta, -alpha, tempPosition);
      nodes++;
      ply--;
      if (value > bestScore) {
        bestScore = value;
        bestMoveOfNode = capturedOrPromoMoves[j];

        if (value > alpha) {
          alpha = value;
        }

        if (alpha >= beta) {
          break;
        }
      }
    }
  }
  positionHistory[(position.getPly() + 1) % 100] = tempZobristHash;
  if (isComplete && (moveCounter != 0)) {
    if (bestScore <= originalAlpha) {
      tt.add(hashEntry{position.getZobrist(), 0, bestScore,
                       nodeType::UPPERBOUND, bestMoveOfNode});
    } else if (bestScore >= beta) {
      tt.add(hashEntry{position.getZobrist(), 0, bestScore,
                       nodeType::LOWERBOUND, bestMoveOfNode});
    } else {
      tt.add(hashEntry{position.getZobrist(), 0, bestScore, nodeType::EXACT,
                       bestMoveOfNode});
    }
  }
  return bestScore;
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