#include "move_generation.hpp"
#include "movelist.hpp"
#include "position.hpp"
#include <gtest/gtest.h>
TEST(MoveListTest, swapNonCapture) {
  Position position;
  position.setBoardToInitialConfiguration();
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  MoveList moves = movegen.getMoves();
  Move move_3 = moves[3];
  Move move_4 = moves[4];
  moves.swap(3, 4);
  ASSERT_EQ(move_3.getTo(), moves[4].getTo());
  ASSERT_EQ(move_3.getFrom(), moves[4].getFrom());
  ASSERT_EQ(move_3.getPiece(), moves[4].getPiece());
  ASSERT_EQ(move_3.getColor(), moves[4].getColor());
  ASSERT_EQ(move_3.checkIsCapture(), moves[4].checkIsCapture());
  ASSERT_EQ(move_3.getScore(), moves[4].getScore());

  ASSERT_EQ(move_4.getTo(), moves[3].getTo());
  ASSERT_EQ(move_4.getFrom(), moves[3].getFrom());
  ASSERT_EQ(move_4.getPiece(), moves[3].getPiece());
  ASSERT_EQ(move_4.getColor(), moves[3].getColor());
  ASSERT_EQ(move_4.checkIsCapture(), moves[3].checkIsCapture());
  ASSERT_EQ(move_4.getScore(), moves[3].getScore());
};
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}