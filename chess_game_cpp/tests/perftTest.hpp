#include "UCI.hpp"
#include "move_generation.hpp"
class perftTest {

public:
  perftTest() = default;
  perftTest(Position pos) : position(pos){};
  uint64_t perftPseudoLegal(int depth);
  uint64_t perft(int depth);
  // FIX IT: the exit node gives wrong number due to illegal moves;
  uint64_t perftBulk(int depth);

private:
  Position position;
};
MoveList perftDivide(Position position, int depth);
void perftDivideInterface();
void perftDivideInterface(std::string FENstr);
