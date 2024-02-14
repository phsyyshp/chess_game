#include "Move.hpp"
#include "UCI.hpp"
#include "moveToStr.hpp"
#include "move_generation.hpp"
#include "movelist.hpp"
#include "perftTest.hpp"
#include "position.hpp"
#include "search.hpp"
#include <gtest/gtest.h>

class UCITestHelper {
public:
  static std::streambuf *originalCinBuffer;
  static std::istringstream testInput;

  static void SetUp(const std::string &input) {
    // Backup the original cin buffer
    originalCinBuffer = std::cin.rdbuf();

    // Set the test input
    testInput.str(input);
    // Redirect cin to read from the test input
    std::cin.rdbuf(testInput.rdbuf());
  }

  static void TearDown() {
    // Restore the original cin buffer
    std::cin.rdbuf(originalCinBuffer);
  }
};

// Define the static members of UCITestHelper
std::streambuf *UCITestHelper::originalCinBuffer = nullptr;
std::istringstream UCITestHelper::testInput;
// MoveList tests
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
  ASSERT_EQ(move_3.getScore(), moves[4].getScore());

  ASSERT_EQ(move_4.getTo(), moves[3].getTo());
  ASSERT_EQ(move_4.getFrom(), moves[3].getFrom());
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
      ASSERT_EQ(move_i.getScore(), moves[j].getScore());

      ASSERT_EQ(move_j.getTo(), moves[i].getTo());
      ASSERT_EQ(move_j.getFrom(), moves[i].getFrom());
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
      ASSERT_EQ(move_i.getScore(), moves[j].getScore());

      ASSERT_EQ(move_j.getTo(), moves[i].getTo());
      ASSERT_EQ(move_j.getFrom(), moves[i].getFrom());
      ASSERT_EQ(move_j.getScore(), moves[i].getScore());
    }
  }
};

// Position
TEST(Position, makeMove_legality_individualMoves_Test_nocheck) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  MoveGeneration movgen(position);
  movgen.generateAllMoves();
  MoveList ml = movgen.getMoves();
  bool isLegal = position.makeMove(ml[0]);
  ASSERT_EQ(true, isLegal);
}
TEST(Position, makeMove_legality_individualMoves_Test_check) {

  std::string FENstr = "8/q1k5/8/8/8/8/1K6/8/";
  Position position(FENtoPieces(FENstr), white);
  moveToStr uci(position);

  bool isLegal = position.makeMove(uci.getMove("b2a2"));
  ASSERT_EQ(false, isLegal);
}

TEST(Position, makeMove_legality_Test) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  MoveGeneration movgen(position);
  movgen.generateAllMoves();
  Position tempPosition;
  for (const Move &move : movgen.getMoves()) {
    tempPosition = position;
    bool isLegal = position.makeMove(move);
    position = tempPosition;
    ASSERT_EQ(true, isLegal);
  }
}
TEST(Position, gameState_initial_config_Test) {
  Position position;
  position.setBoardToInitialConfiguration();
  ASSERT_EQ(8, position.getGameState().getEnPassant());
}
TEST(Position, gameState_pawnPush_Test) {
  Position position;
  position.setBoardToInitialConfiguration();
  moveToStr uci(position);
  position.makeMove(uci.getMove("d2d4"));
  ASSERT_EQ(3, position.getGameState().getEnPassant());
}
TEST(Position, gameState_normalMove_after_pawnPush_Test) {
  Position position;
  position.setBoardToInitialConfiguration();
  moveToStr uci(position);
  position.makeMove(uci.getMove("d2d4"));
  position.makeMove(uci.getMove("d7d6"));

  ASSERT_EQ(8, position.getGameState().getEnPassant());
}
TEST(Position, constructor_FEN) {

  std::string FENstr =
      "rn1q1rk1/4bpp1/pp1pbn2/4p1Pp/4P3/1NN1BP2/PPPQ3P/R3KB1R w KQ h6 0 12";
  Position position(FENstr);
  GameState gs = position.getGameState();
  uint castling = gs.getCastlingRigths();
  uint turn = gs.getTurn();
  uint EP = gs.getEnPassant();
  ASSERT_EQ(0b1100, castling);
  ASSERT_EQ(white, turn);
  ASSERT_EQ(squareTofile[h6], EP);
}
TEST(Position, castling_rigths_update_rook_move) {

  std::string FENstr =
      "rn1q1rk1/4bpp1/pp1pbn2/4p1Pp/4P3/1NN1BP2/PPPQ3P/R3KB1R w KQ h6 0 12";
  Position position(FENstr);
  moveToStr uci(position);
  position.makeMove(uci.getMove("a1b1"));
  GameState gs = position.getGameState();

  uint castling = gs.getCastlingRigths();
  ASSERT_EQ(0b0100u, castling);
}
TEST(Position, castling_rigths_update_king_move) {

  std::string FENstr =
      "rn1q1rk1/4bpp1/pp1pbn2/4p1Pp/4P3/1NN1BP2/PPPQ3P/R3KB1R w KQ h6 0 12";
  Position position(FENstr);
  moveToStr uci(position);
  position.makeMove(uci.getMove("e1d1"));
  GameState gs = position.getGameState();

  uint castling = gs.getCastlingRigths();
  ASSERT_EQ(0b0000u, castling);
}
TEST(Position, makecastling_white_queen_side) {

  Position position(
      "rn1q1rk1/4bpp1/pp1pbn2/4p1Pp/4P3/1NN1BP2/PPPQ3P/R3KB1R w KQ h6 0 12");
  moveToStr uci(position);
  position.makeMove(uci.getMove("e1c1"));
  Position positionCorrect(
      "rn1q1rk1/4bpp1/pp1pbn2/4p1Pp/4P3/1NN1BP2/PPPQ3P/2KR1B1R b - - 1 12");
  positionCorrect.printBoard();
  position.printBoard();

  ASSERT_EQ(positionCorrect.getPieces(), position.getPieces());

  ASSERT_EQ(positionCorrect.getGameState().getGameStateNum(),
            position.getGameState().getGameStateNum());
}
TEST(Position, makecastling_white_king_side) {

  Position position(
      "rn3rk1/2q1bpp1/pp1pbn2/4p1Pp/4P3/1NNBBP2/PPPQ3P/R3K2R w KQ - 2 13");
  moveToStr uci(position);
  position.makeMove(uci.getMove("e1g1"));
  Position positionCorrect(
      "rn3rk1/2q1bpp1/pp1pbn2/4p1Pp/4P3/1NNBBP2/PPPQ3P/R4RK1 b - - 3 13");
  positionCorrect.printBoard();
  position.printBoard();

  ASSERT_EQ(positionCorrect.getPieces(), position.getPieces());

  ASSERT_EQ(positionCorrect.getGameState().getGameStateNum(),
            position.getGameState().getGameStateNum());
}
// Utilities
TEST(Utillities, FENtoRanks) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R";
  std::array<std::string, 8> ranksArr = FENtoRanks(FENstr);
  ASSERT_EQ("2KR1B1R", ranksArr[7]);
}
TEST(Utillities, FENtoFields) {

  std::string FENstr =
      "rnbqkbnr/p2p1ppp/1p6/2pPp3/4P3/8/PPP2PPP/RNBQKBNR w KQkq c6 0 4";
  std::vector<std::string> fieldVec = FENtoFields(FENstr);
  ASSERT_EQ("rnbqkbnr/p2p1ppp/1p6/2pPp3/4P3/8/PPP2PPP/RNBQKBNR", fieldVec[0]);
  ASSERT_EQ("4", fieldVec[5]);
  ASSERT_EQ("0", fieldVec[4]);
  ASSERT_EQ("c6", fieldVec[3]);
}
TEST(Perft, perft_castling) {
  Position position("rn1qk2r/p4p1p/bp1p1n2/2P1p1p1/1b1p4/1PN1PNP1/PB1Q1PBP/"
                    "R3K2R w KQkq - 0 12");
  perftTest test(position);
  uint64_t perftResult = test.perft(3);
  // perftDivideInterface("rn1qk2r/p4p1p/bp1p1n2/2P1p1p1/1b1p4/1PN1PNP1/PB1Q1PBP/"
  //  "R3K2R w KQkq - 0 12");
  ASSERT_EQ(57902, perftResult);
}
TEST(Perft, perft__pawn_promos) {
  Position position("4k3/6PP/P1P3P1/3P1P2/8/p7/8/2K5 w - - 0 1");
  perftTest test(position);
  uint64_t perftResult = test.perft(4);
  // perftDivideInterface("4k3/6PP/P1P3P1/3P1P2/8/p7/8/2K5 w - - 0 1");
  ASSERT_EQ(2771, perftResult);
}

// Search tests Ply
TEST(SearchTest, Ply_constructor) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";

  Position position(FENtoPieces(FENstr), white);
  Search s(position);
  ASSERT_EQ(s.getPly(), 0);
}
TEST(SearchTest, Plycounter_SearchAB_5) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  Search s(position);
  int initialPly = s.getPly();
  auto start = std::chrono::high_resolution_clock::now();
  s.searchAB(1, start, 1000, 10);
  int finalPly = s.getPly();
  ASSERT_EQ(finalPly, initialPly);
}
TEST(SearchTest, Plycounter_SearchAB_4) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  Search s(position);
  int initialPly = s.getPly();
  auto start = std::chrono::high_resolution_clock::now();
  s.searchAB(2, start, 1000, 10);
  int finalPly = s.getPly();
  ASSERT_EQ(finalPly, initialPly);
}

// Killer move test
TEST(SearchTest, killerMove_Initilization) {
  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  Search s(position);
  std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES> killerMoves =
      s.getKillerMoves();
  Move invalidMove(a1, a1, 0);
  ASSERT_EQ(killerMoves[1][63].getScore(), invalidMove.getScore());
}
TEST(SearchTest, SearchLegalityTest) {

  Position position;
  position.setBoardToInitialConfiguration();
  int i = 0;
  int wtime = 8080;
  int winc = 80;
  int btime = 8080;
  int binc = 80;
  int maxDepth = 500;
  while (i < 2) {

    MoveGeneration mg(position);
    Search srch(position, wtime, winc, btime, binc);
    Move bestMove = srch.searchIt(maxDepth, false);
    ASSERT_TRUE(position.makeMove(bestMove));
    // position.printBoard();
    i++;
  }
}

// pick move
TEST(SearchTest, pickMove_copy) {

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
TEST(SearchTest, pickMove_nocopy) {

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

// UCI
TEST(UCI, uciLoopTEST) {
  UCITestHelper::SetUp("uci\nposition startpos\nisready\n");

  UCI uci;
  uci.loop(); // Run the loop with predefined commands

  // Add assertions to verify the behavior of your UCI class
  // based on the predefined commands

  UCITestHelper::TearDown();
}
TEST(UCI, uciManualTESTSequenceOfMOves) {

  UCI uci;
  uci.manual("uci");
  // uci.getPosition().printBoard();
  uci.manual("position startpos");
  uci.getPosition().printBoard();
  uci.manual("isready");
  uci.getPosition().printBoard();
  uci.manual("go wtime 35939 btime 38039 winc 80 binc 80");
  uci.getPosition().printBoard();
  uci.manual("position startpos moves d2d4");
  uci.getPosition().printBoard();
  uci.manual("isready");
  uci.getPosition().printBoard();
  uci.manual("go wtime 35939 btime 38039 winc 80 binc 80");
  uci.getPosition().printBoard();
  uci.manual("position startpos moves d2d4 h7h6 e2e4");
  uci.getPosition().printBoard();
}
TEST(UCI, uciManualTESTSingleLineManyMoves) {

  UCI uci;
  uci.manual("uci");
  // uci.getPosition().printBoard();
  uci.manual("position startpos");
  uci.getPosition().printBoard();
  uci.manual("isready");
  uci.getPosition().printBoard();
  uci.manual("position startpos moves d2d4 e7e5");
  uci.getPosition().printBoard();
  uci.manual("isready");
  uci.getPosition().printBoard();
  uci.manual("go wtime 35939 btime 38039 winc 80 binc 80");
  uci.getPosition().printBoard();
  uci.manual("position startpos moves d2d4 e7e5 a2a3");
  uci.getPosition().printBoard();
  uci.manual("isready");
  uci.getPosition().printBoard();
}
// comeToStr
TEST(moveToStr, strToMove_Nocatpure) {
  Position position;
  position.setBoardToInitialConfiguration();
  moveToStr uci(position);
  std::string moveStr = "d2d4";
  Move UCImove = uci.getMove(moveStr);
  Move handMade(d2, d4, doublePawnPush);
  ASSERT_EQ(handMade.getMoveInt(), UCImove.getMoveInt());
}
TEST(moveToStr, strToMove_capture) {
  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), white);
  moveToStr uci(position);
  std::string moveStr = "d2d6";
  Move UCImove = uci.getMove(moveStr);
  Move handMade(d2, d6, captures);
  ASSERT_EQ(handMade.getMoveInt(), UCImove.getMoveInt());
}
TEST(moveToStr, makecastling_white_king_side) {

  Position position(
      "rn3rk1/2q1bpp1/pp1pbn2/4p1Pp/4P3/1NNBBP2/PPPQ3P/R3K2R w KQ - 2 13");
  moveToStr uci(position);
  Move move = uci.getMove("e1g1");
  Move handMade(e1, g1, kingCastle);
  ASSERT_EQ(handMade.getMoveInt(), move.getMoveInt());
}

// TEST(GameLoop, gameloopTEST) {
//   Position position;
//   position.setBoardToInitialConfiguration();
//   position.printBoard();
//   std::string moveStr;
//   moveToStr uci(position);
//   while (std::cin >> moveStr) {
//     Move move = uci.getMove(moveStr);
//     MoveGeneration mg(position);
//     if (!mg.isPseudoLegal(move)) {
//       std::cout << "illegal move try again." << std::endl;
//       continue;
//     }
//     position.makeMove(move);
//     position.printBoard();
//     Search srch(position);
//     Move bestMove = srch.searchAB(5);
//     position.makeMove(bestMove);
//     position.printBoard();
//   }
// }
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}