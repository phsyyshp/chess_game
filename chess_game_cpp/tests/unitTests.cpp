#include "move_generation.hpp"
#include "movelist.hpp"
#include "position.hpp"
#include "search.hpp"
#include <gtest/gtest.h>
TEST(MoveListTest, swapNonScoredNonCapture) {
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
TEST(MoveListTest, swapNonScoredCapture) {
  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  MoveList moves = movegen.getMoves();
  for (int i = 0; i < moves.size(); i++) {

    for (int j = 0; j < moves.size(); j++) {

      Move move_i = moves[i];
      Move move_j = moves[j];

      moves.swap(i, j);

      ASSERT_EQ(move_i.getTo(), moves[j].getTo());
      ASSERT_EQ(move_i.getFrom(), moves[j].getFrom());
      ASSERT_EQ(move_i.getPiece(), moves[j].getPiece());
      ASSERT_EQ(move_i.getColor(), moves[j].getColor());
      ASSERT_EQ(move_i.checkIsCapture(), moves[j].checkIsCapture());
      ASSERT_EQ(move_i.getScore(), moves[j].getScore());

      ASSERT_EQ(move_j.getTo(), moves[i].getTo());
      ASSERT_EQ(move_j.getFrom(), moves[i].getFrom());
      ASSERT_EQ(move_j.getPiece(), moves[i].getPiece());
      ASSERT_EQ(move_j.getColor(), moves[i].getColor());
      ASSERT_EQ(move_j.checkIsCapture(), moves[i].checkIsCapture());
      ASSERT_EQ(move_j.getScore(), moves[i].getScore());
    }
  }
};
TEST(MoveListTest, swapScoredCaptured) {
  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  MoveList moves = movegen.getMoves();
  Search s(position);
  s.scoreMoves(moves);

  for (int i = 0; i < moves.size(); i++) {

    for (int j = 0; j < moves.size(); j++) {

      Move move_i = moves[i];
      Move move_j = moves[j];

      moves.swap(i, j);

      ASSERT_EQ(move_i.getTo(), moves[j].getTo());
      ASSERT_EQ(move_i.getFrom(), moves[j].getFrom());
      ASSERT_EQ(move_i.getPiece(), moves[j].getPiece());
      ASSERT_EQ(move_i.getColor(), moves[j].getColor());
      ASSERT_EQ(move_i.checkIsCapture(), moves[j].checkIsCapture());
      ASSERT_EQ(move_i.getScore(), moves[j].getScore());

      ASSERT_EQ(move_j.getTo(), moves[i].getTo());
      ASSERT_EQ(move_j.getFrom(), moves[i].getFrom());
      ASSERT_EQ(move_j.getPiece(), moves[i].getPiece());
      ASSERT_EQ(move_j.getColor(), moves[i].getColor());
      ASSERT_EQ(move_j.checkIsCapture(), moves[i].checkIsCapture());
      ASSERT_EQ(move_j.getScore(), moves[i].getScore());
    }
  }
};
// Dont forget to test reference of movegen stuff
TEST(Search_pickMoveTest, pickMove_copy) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  // Copy
  MoveList moves = movegen.getMoves();
  Search s(position);
  s.scoreMoves(moves);
  for (int startingMove = 0; startingMove < moves.size(); startingMove++) {

    s.pickMove(moves, startingMove);

    for (int i = startingMove; i < moves.size(); i++) {
      ASSERT_GE(moves[startingMove].getScore(), moves[i].getScore());
    }
  }
}
TEST(Search_pickMoveTest, pickMove_nocopy) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  MoveGeneration movegen(position);
  movegen.generateAllMoves();
  Search s(position);
  // pass by reference;
  s.scoreMoves(movegen.getMoves());
  for (int startingMove = 0; startingMove < movegen.getMoves().size();
       startingMove++) {

    s.pickMove(movegen.getMoves(), startingMove);
    for (int i = startingMove; i < movegen.getMoves().size(); i++) {
      ASSERT_GE(movegen.getMoves()[startingMove].getScore(),
                movegen.getMoves()[i].getScore());
    }
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}