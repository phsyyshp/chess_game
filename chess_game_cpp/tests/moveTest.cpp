#include "Move.hpp"
int main() {
  Move move = Move(a1, a5, piece::rook, color::white, false);

  std::cout << move.getTo() << std::endl;
  std::cout << move.getFrom() << std::endl;
  std::cout << move.getColor() << std::endl;
  std::cout << move.getPiece() << std::endl;
};
