#include "search.hpp"

int main() {
  std::string FEN2 = "r1b1kb1r/1pqn1ppp/pn6/4P3/3NP1P1/2P1BQ2/PP5P/R3KB1R/";
  std::string FEN = "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR/";
  Position position(FENtoPieces(FEN), black);
  position.printBoard();
  Evaluation eval(position);
  std::cout << eval.evaluate() << std::endl;
  Search search(position);

  search.negaMax(3);
  Move bestMove = search.getBestMove();
  std::string toSqStr = chessSq[bestMove.getTo()];
  std::string fromSqStr = chessSq[bestMove.getFrom()];

  std::cout << fromSqStr << " " << toSqStr << std::endl;
  std::cout << bestMove.getColor() << std::endl;
  position.makeMove(bestMove);
  Position pos(FENtoPieces(FEN2), black);
  position = pos;
  std::cout << eval.evaluate() << std::endl;
  position.printBoard();
}