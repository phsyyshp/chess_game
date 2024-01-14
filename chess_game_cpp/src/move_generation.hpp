#include "Move.hpp"
#include "position.hpp"
class MoveGeneration {

public:
  MoveGeneration() = default;
  std::vector<uint64_t> factor_mask(uint64_t mask);
  void generateSinglePawnPushes(const Position &position, const color &coloIn);
  void generateDoublePawnPushes(const Position &position, const color &coloIn);
  void generateLeftPawnCaptures(const Position &position, const color &colorIn);
  void generateRightPawnCaptures(const Position &position,
                                 const color &colorIn);
  void generateKnightMoves(const Position &position, const color &coloIn);
  void generateBishopMoves(const Position &position, const color &colorIn);
  void generateRookMoves(const Position &position, const color &colorIn);
  void generateQueenMoves(const Position &position, const color &colorIn);
  void generateKingMoves(const Position &position, const color &colorIn);
  void generateAllMoves(const Position &position, const color &colorIn);
  // getters
  std::vector<Move> getMoves() const;

private:
  std::vector<Move> moveList;
};
