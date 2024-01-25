#include "evaluation.hpp"

int main() {
  // std::string FEN = "r1b1kb1r/1pqn1ppp/pn6/4P3/3NP1P1/2P1BQ2/PP5P/R3KB1R/";
  // std::string FEN = "rnbqkbnr/1ppppppp/p7/8/3P4/8/PPP1PPPP/RNBQKBNR/";
  // Position position(FENtoPieces(FEN), white);
  Position position;
  Position tempPosition;
  position.setBoardToInitialConfiguration();
  position.printBoard();
  MoveGeneration movgen(position);
  movgen.generateAllMoves();
  for (Move move : movgen.getMoves()) {
    tempPosition = position;
    position.makeMove(move);
    Evaluation eval(position);

    std::cout << "-----------------------a-----------------------------"
              << std::endl;
    std::cout << "white eval" << std::endl;
    std::cout << eval.evaluate(white) << std::endl;
    std::cout << "black eval" << std::endl;
    std::cout << eval.evaluate(black) << std::endl;

    std::cout << chessSq[move.getFrom()] << chessSq[move.getTo()] << std::endl;
    position.printBoard();
    std::cout << "----------------------------------------------------"
              << std::endl;
    position = tempPosition;
  };

  // Evaluation eval(position);
  // std::cout << "Evaluation for this position is " << eval.evaluate()
  //           << std::endl;
}