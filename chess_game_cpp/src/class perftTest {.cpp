class perftTest {

public:
  int perftRec(int depth) {
    Position tempPosition;
    if (depth == 0) {
      return 1;
    }
    int nodes = 0;
    MoveGeneration movGen;
    movGen.generateAllMoves(position, position.getTurn());
    std::vector<Move> allMoves = movGen.getMoves();
    for (const auto &move : allMoves) {
      // FIX IT: do undo;
      tempPosition = position;
      position.makeMove(move);

      nodes += perftRec(depth - 1);
      position = tempPosition;

      // position.undoMove(move);
      
    }
    return nodes;
  }

  Position position;
};