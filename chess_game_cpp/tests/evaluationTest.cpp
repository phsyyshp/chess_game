#include "evaluation.hpp"

int main() {
  std::string FEN = "r1b1kb1r/1pqn1ppp/pn6/4P3/3NP1P1/2P1BQ2/PP5P/R3KB1R/";
  Position position(FENtoPieces(FEN), white);
  position.printBoard();
  Evaluation eval(position);
  std::cout << "Evaluation for this position is" << eval.evaluate()
            << std::endl;
}