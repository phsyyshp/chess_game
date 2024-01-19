#include "search.hpp"
int main() {

  std::string FEN = "r1b1kb1r/1pqn1ppp/pn6/4P3/3NP1P1/2P1BQ2/PP5P/R3KB1R/";
  Position position(FENtoPieces(FEN), white);
  position.printBoard();
  Evaluation eval(position);
  Search search(position);
  search.negaMax(4);
  Move bestMove = search.getBestMove();
  std::string toSqStr = chessSq[bestMove.getTo()];
  std::string fromSqStr = chessSq[bestMove.getFrom()];

  std::cout << fromSqStr << " " << toSqStr << std::endl;
  position.makeMove(bestMove);
  position.printBoard();
}