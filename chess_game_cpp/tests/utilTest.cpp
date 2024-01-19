#include "position.hpp"
#include "utilities.hpp"
#include <array>
int main() {
  std::string FENstring = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R/";
  FENstring = "rnbqkb1r/pp4pp/2p1pn2/3p1p2/2PP4/6PN/PP2PPBP/RNBQK2R/";
  std::array<std::string, 8> ranks = FENtoRanks(FENstring);
  for (auto rank : ranks) {
    std::cout << rank << "\n";
  }

  std::array<std::array<uint64_t, 6>, 2> pieces = FENtoPieces(FENstring);
  Position position(pieces, white);
  position.printBoard();
}