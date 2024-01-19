#include "evaluation.hpp"

int main() {
  std::string FEN = "rnbqkbnr/pppppppp/8/3P4/5P2/1P6/6PP/RNBQKBNR/";
  Position position(FENtoPieces(FEN), white);
  position.printBoard();
  Evaluation eval(position);
  std::cout << eval.evaluate(white);
}