#include "moveGeneration.hpp"
#include "moveToStr.hpp"
#include "tt.hpp"
#include "zobrist.hpp"
#include <cassert>
struct perftTTentry {
  perftTTentry() : zobristKey(0), nodes(0){};
  perftTTentry(uint64_t z, uint64_t n) : zobristKey(z), nodes(n){};

  uint64_t zobristKey;
  uint64_t nodes;
};

class perftTest {

public:
  perftTest(){

  };

  perftTest(Position pos) : position(pos) {
    tt.resize(TT_SIZE);
    std::fill(tt.begin(), tt.end(), perftTTentry{});
  };
  uint64_t perftPseudoLegal(int depth);
  uint64_t perft(int depth, const Position &position);
  uint64_t perftZobrist(uint64_t depth, const Position &position);

  // FIX IT: the exit node gives wrong number due to illegal moves;
  uint64_t perftBulk(int depth, const Position &position);

private:
  Position position;
  std::vector<perftTTentry> tt;
};
MoveList perftDivide(const Position &position, int depth);
void perftDivideInterface();
void perftDivideInterface(std::string FENstr);
