#include "UCI.hpp"
#include "move.hpp"
#include "moveGeneration.hpp"
#include "moveList.hpp"
#include "moveToStr.hpp"
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
  Position position(FENtoPieces(FENstr), WHITE);
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
// TEST(MoveListTest, swapScoredCaptured) {
//   std::string FENstr =
//       "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
//   Position position(FENtoPieces(FENstr), white);
//   MoveGeneration movegen(position);
//   movegen.generateAllMoves();
//   MoveList moves = movegen.getMoves();
//   Search s(position);
//   s.scoreMoves(moves);

//   for (int i = 0; i < moves.size(); i++) {

//     for (int j = 0; j < moves.size(); j++) {

//       Move move_i = moves[i];
//       Move move_j = moves[j];

//       moves.swap(i, j);

//       ASSERT_EQ(move_i.getTo(), moves[j].getTo());
//       ASSERT_EQ(move_i.getFrom(), moves[j].getFrom());
//       ASSERT_EQ(move_i.getScore(), moves[j].getScore());

//       ASSERT_EQ(move_j.getTo(), moves[i].getTo());
//       ASSERT_EQ(move_j.getFrom(), moves[i].getFrom());
//       ASSERT_EQ(move_j.getScore(), moves[i].getScore());
//     }
//   }
// };

// Position
TEST(Position, makeMove_legality_individualMoves_Test_nocheck) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), WHITE);
  MoveGeneration movgen(position);
  movgen.generateAllMoves();
  MoveList ml = movgen.getMoves();
  bool isLegal = position.makeMove(ml[0]);
  ASSERT_EQ(true, isLegal);
}
TEST(Position, makeMove_legality_individualMoves_Test_check) {

  std::string FENstr = "8/q1k5/8/8/8/8/1K6/8/";
  Position position(FENtoPieces(FENstr), WHITE);
  moveToStr uci(position);

  bool isLegal = position.makeMove(uci.getMove("b2a2"));
  ASSERT_EQ(false, isLegal);
}

TEST(Position, makeMove_legality_Test) {

  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), WHITE);
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
  ASSERT_EQ(WHITE, turn);
  ASSERT_EQ(SQUARE_TO_FILE[H6], EP);
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
  // positionCorrect.printBoard();
  // position.printBoard();

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
  // positionCorrect.printBoard();
  // position.printBoard();

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
  uint64_t perftResult = test.perft(3, position);
  // perftDivideInterface("rn1qk2r/p4p1p/bp1p1n2/2P1p1p1/1b1p4/1PN1PNP1/PB1Q1PBP/"
  //  "R3K2R w KQkq - 0 12");
  ASSERT_EQ(57902, perftResult);
}
TEST(Perft, perft__pawn_promos) {
  Position position("4k3/6PP/P1P3P1/3P1P2/8/p7/8/2K5 w - - 0 1");
  perftTest test(position);
  uint64_t perftResult = test.perft(4, position);
  // perftDivideInterface("4k3/6PP/P1P3P1/3P1P2/8/p7/8/2K5 w - - 0 1");
  ASSERT_EQ(2771, perftResult);
}
TEST(Perft, perft_king_capture) {
  Position position("r6r/pb4pp/4Pp2/2k2P2/1pP5/4N3/2PB2PP/1R3K1R b - - 3 24");
  perftTest test(position);
  // perftDivideInterface(
  // "r6r/pb4pp/4Pp2/2k2P2/1pP5/4N3/2PB2PP/1R3K1R b - - 3 24");
  uint64_t perftResult = test.perft(4, position);
  ASSERT_EQ(525086, perftResult);
}
// TEST(Perft, perftFEN1) {
//   Position position(
//       "r1b1k2r/1pp1qpbp/p1n1p1p1/2n1P3/4NB2/3BQN2/PPP2PPP/3RK2R b Kkq - 5
//       12");
//   perftTest test(position);
//   uint64_t perftResult = test.perft(4);
//   ASSERT_EQ(2899828, perftResult);
// }
// TEST(Perft, perftFEN2) {
//   Position position(
//       "r1b2k1r/N2q2pp/2p3n1/3ppp2/3P4/1P1BPQP1/2PN1P1P/R3K2R w KQ - 2 16");
//   perftTest test(position);
//   uint64_t perftResult = test.perft(5);
//   ASSERT_EQ(68719694, perftResult);
// }
// TEST(Perft, perftFEN3) {
//   Position position(
//       "rnbqkb1r/pp5p/3p1np1/2pPpp2/2P5/2N2NP1/PP2PP1P/R1BQKB1R w KQkq - 2
//       7");
//   perftTest test(position);
//   uint64_t perftResult = test.perft(5);
//   ASSERT_EQ(44351570, perftResult);
// }
TEST(Perft, perftList) {
  std::vector<std::vector<std::string>> chessData = {
      {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "119060324"},
      {"4k3/8/8/8/8/8/8/4K2R w K - 0 1", "764643"},
      {"4k3/8/8/8/8/8/8/R3K3 w Q - 0 1", "846648"},
      {"4k2r/8/8/8/8/8/8/4K3 w k - 0 1", "899442"},
      {"r3k3/8/8/8/8/8/8/4K3 w q - 0 1", "1001523"},
      {"4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", "2788982"},
      {"r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1", "3517770"},
      {"8/8/8/8/8/8/6k1/4K2R w K - 0 1", "185867"},
      {"8/8/8/8/8/8/1k6/R3K3 w Q - 0 1", "413018"},
      {"4k2r/6K1/8/8/8/8/8/8 w k - 0 1", "179869"},
      {"r3k3/1K6/8/8/8/8/8/8 w q - 0 1", "367724"},
      {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "179862938"},
      {"r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1", "195629489"},
      {"r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1", "184411439"},
      {"r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1", "189224276"},
      {"1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", "198328929"},
      {"2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", "185959088"},
      {"r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1", "190755813"},
      {"4k3/8/8/8/8/8/8/4K2R b K - 0 1", "899442"},
      {"4k3/8/8/8/8/8/8/R3K3 b Q - 0 1", "1001523"},
      {"4k2r/8/8/8/8/8/8/4K3 b k - 0 1", "764643"},
      {"r3k3/8/8/8/8/8/8/4K3 b q - 0 1", "846648"},
      {"4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1", "3517770"},
      {"r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1", "2788982"},
      {"8/8/8/8/8/8/6k1/4K2R b K - 0 1", "179869"},
      {"8/8/8/8/8/8/1k6/R3K3 b Q - 0 1", "367724"},
      {"4k2r/6K1/8/8/8/8/8/8 b k - 0 1", "185867"},
      {"r3k3/1K6/8/8/8/8/8/8 b q - 0 1", "413018"},
      {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "179862938"},
      {"r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1", "198328929"},
      {"r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1", "185959088"},
      {"r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1", "190755813"},
      {"1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", "195629489"},
      {"2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", "184411439"},
      {"r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1", "189224276"},
      {"8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1", "8107539"},
      {"8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1", "2594412"},
      {"8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1", "19870403"},
      {"K7/8/2n5/1n6/8/8/8/k6N w - - 0 1", "588695"},
      {"k7/8/2N5/1N6/8/8/8/K6n w - - 0 1", "688780"},
      {"8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1", "8503277"},
      {"8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1", "3147566"},
      {"8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1", "4405103"},
      {"K7/8/2n5/1n6/8/8/8/k6N b - - 0 1", "688780"},
      {"k7/8/2N5/1N6/8/8/8/K6n b - - 0 1", "588695"},
      {"B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1", "22823890"},
      {"8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1", "28861171"},
      {"k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1", "7881673"},
      {"K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1", "7382896"},
      {"B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1", "9250746"},
      {"8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1", "29027891"},
      {"k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1", "7382896"},
      {"K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1", "7881673"},
      {"7k/RR6/8/8/8/8/rr6/7K w - - 0 1", "44956585"},
      {"R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1", "525169084"},
      {"7k/RR6/8/8/8/8/rr6/7K b - - 0 1", "44956585"},
      {"R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1", "524966748"},
      {"6kq/8/8/8/8/8/8/7K w - - 0 1", "391507"},
      {"6KQ/8/8/8/8/8/8/7k b - - 0 1", "391507"},
      {"K7/8/8/3Q4/4q3/8/8/7k w - - 0 1", "3370175"},
      {"6qk/8/8/8/8/8/8/7K b - - 0 1", "419369"},
      {"6KQ/8/8/8/8/8/8/7k b - - 0 1", "391507"},
      {"K7/8/8/3Q4/4q3/8/8/7k b - - 0 1", "3370175"},
      {"8/8/8/8/8/K7/P7/k7 w - - 0 1", "6249"},
      {"8/8/8/8/8/7K/7P/7k w - - 0 1", "6249"},
      {"K7/p7/k7/8/8/8/8/8 w - - 0 1", "2343"},
      {"7K/7p/7k/8/8/8/8/8 w - - 0 1", "2343"},
      {"8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1", "34834"},
      {"8/8/8/8/8/K7/P7/k7 b - - 0 1", "2343"},
      {"8/8/8/8/8/7K/7P/7k b - - 0 1", "2343"},
      {"K7/p7/k7/8/8/8/8/8 b - - 0 1", "6249"},
      {"7K/7p/7k/8/8/8/8/8 b - - 0 1", "6249"},
      {"8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1", "34822"},
      {"8/8/8/8/8/4k3/4P3/4K3 w - - 0 1", "11848"},
      {"4k3/4p3/4K3/8/8/8/8/8 b - - 0 1", "11848"},
      {"8/8/7k/7p/7P/7K/8/8 w - - 0 1", "10724"},
      {"8/8/k7/p7/P7/K7/8/8 w - - 0 1", "10724"},
      {"8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1", "53138"},
      {"8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1", "157093"},
      {"8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1", "158065"},
      {"k7/8/3p4/8/3P4/8/8/7K w - - 0 1", "20960"},
      {"8/8/7k/7p/7P/7K/8/8 b - - 0 1", "10724"},
      {"8/8/k7/p7/P7/K7/8/8 b - - 0 1", "10724"},
      {"8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1", "53138"},
      {"8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1", "158065"},
      {"8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1", "157093"},
      {"k7/8/3p4/8/3P4/8/8/7K b - - 0 1", "21104"},
      {"7k/3p4/8/8/3P4/8/8/K7 w - - 0 1", "32191"},
      {"7k/8/8/3p4/8/8/3P4/K7 w - - 0 1", "30980"},
      {"k7/8/8/7p/6P1/8/8/K7 w - - 0 1", "41874"},
      {"k7/8/7p/8/8/6P1/8/K7 w - - 0 1", "29679"},
      {"k7/8/8/6p1/7P/8/8/K7 w - - 0 1", "41874"},
      {"k7/8/6p1/8/8/7P/8/K7 w - - 0 1", "29679"},
      {"k7/8/8/3p4/4p3/8/8/7K w - - 0 1", "22886"},
      {"k7/8/3p4/8/8/4P3/8/7K w - - 0 1", "28662"},
      {"7k/3p4/8/8/3P4/8/8/K7 b - - 0 1", "32167"},
      {"7k/8/8/3p4/8/8/3P4/K7 b - - 0 1", "30749"},
      {"k7/8/8/7p/6P1/8/8/K7 b - - 0 1", "41874"},
      {"k7/8/7p/8/8/6P1/8/K7 b - - 0 1", "29679"},
      {"k7/8/8/6p1/7P/8/8/K7 b - - 0 1", "41874"},
      {"k7/8/6p1/8/8/7P/8/K7 b - - 0 1", "29679"},
      {"k7/8/8/3p4/4p3/8/8/7K b - - 0 1", "22579"},
      {"k7/8/3p4/8/8/4P3/8/7K b - - 0 1", "28662"},
      {"7k/8/8/p7/1P6/8/8/7K w - - 0 1", "41874"},
      {"7k/8/p7/8/8/1P6/8/7K w - - 0 1", "29679"},
      {"7k/8/8/1p6/P7/8/8/7K w - - 0 1", "41874"},
      {"7k/8/1p6/8/8/P7/8/7K w - - 0 1", "29679"},
      {"k7/7p/8/8/8/8/6P1/K7 w - - 0 1", "55338"},
      {"k7/6p1/8/8/8/8/7P/K7 w - - 0 1", "55338"},
      {"3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1", "199002"},
      {"7k/8/8/p7/1P6/8/8/7K b - - 0 1", "41874"},
      {"7k/8/p7/8/8/1P6/8/7K b - - 0 1", "29679"},
      {"7k/8/8/1p6/P7/8/8/7K b - - 0 1", "41874"},
      {"7k/8/1p6/8/8/P7/8/7K b - - 0 1", "29679"},
      {"k7/7p/8/8/8/8/6P1/K7 b - - 0 1", "55338"},
      {"k7/6p1/8/8/8/8/7P/K7 b - - 0 1", "55338"},
      {"3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1", "199002"},
      {"8/Pk6/8/8/8/8/6Kp/8 w - - 0 1", "1030499"},
      {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1", "37665329"},
      {"8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1", "28859283"},
      {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1", "71179139"},
      {"8/Pk6/8/8/8/8/6Kp/8 b - - 0 1", "1030499"},
      {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1", "37665329"},
      {"8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1", "28859283"},
      {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", "71179139"}};
  for (auto data : chessData) {
    Position position(data[0]);
    perftTest test(position);
    uint64_t perftResult;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 30; i++) {

      perftResult = test.perftBulk(5, position);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << data[0] << " " << data[1] << " nps "
              << perftResult * 1000 * 30 / (elapsed.count()) << '\n';
    ASSERT_EQ(stoull(data[1]), perftResult);
  }
}
// TEST(Perft, perftFEN6) {
//   std::string FEN = "n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1";
//   // , "28859283"
//   Position position(FEN);
//   perftTest pt(position);
//   perftDivideInterface(FEN);
// }
// TEST(Zobrist, RemainingPerft) {

//   std::vector<std::vector<std::string>> chessData = {
//       {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1", "37665329"},
//       {"8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1", "28859283"},
//       {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1", "71179139"},
//       {"8/Pk6/8/8/8/8/6Kp/8 b - - 0 1", "1030499"},
//       {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1", "37665329"},
//       {"8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1", "28859283"},
//       {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", "71179139"}};
//   for (auto data : chessData) {
//     Position position(data[0]);
//     perftTest test(position);
//     uint64_t perftResult = test.perftZobrist(6);
//     std::cout << data[0] << " " << data[1] << '\n';
//     ASSERT_EQ(stoull(data[1]), perftResult);
//   }
// }
// TEST(Zobrist, zobristPerft) {

//   std::vector<std::vector<std::string>> chessData = {
//       {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
//       "119060324"},
//       {"4k3/8/8/8/8/8/8/4K2R w K - 0 1", "764643"},
//       {"4k3/8/8/8/8/8/8/R3K3 w Q - 0 1", "846648"},
//       {"4k2r/8/8/8/8/8/8/4K3 w k - 0 1", "899442"},
//       {"r3k3/8/8/8/8/8/8/4K3 w q - 0 1", "1001523"},
//       {"4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", "2788982"},
//       {"r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1", "3517770"},
//       {"8/8/8/8/8/8/6k1/4K2R w K - 0 1", "185867"},
//       {"8/8/8/8/8/8/1k6/R3K3 w Q - 0 1", "413018"},
//       {"4k2r/6K1/8/8/8/8/8/8 w k - 0 1", "179869"},
//       {"r3k3/1K6/8/8/8/8/8/8 w q - 0 1", "367724"},
//       {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "179862938"},
//       {"r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1", "195629489"},
//       {"r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1", "184411439"},
//       {"r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1", "189224276"},
//       {"1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", "198328929"},
//       {"2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", "185959088"},
//       {"r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1", "190755813"},
//       {"4k3/8/8/8/8/8/8/4K2R b K - 0 1", "899442"},
//       {"4k3/8/8/8/8/8/8/R3K3 b Q - 0 1", "1001523"},
//       {"4k2r/8/8/8/8/8/8/4K3 b k - 0 1", "764643"},
//       {"r3k3/8/8/8/8/8/8/4K3 b q - 0 1", "846648"},
//       {"4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1", "3517770"},
//       {"r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1", "2788982"},
//       {"8/8/8/8/8/8/6k1/4K2R b K - 0 1", "179869"},
//       {"8/8/8/8/8/8/1k6/R3K3 b Q - 0 1", "367724"},
//       {"4k2r/6K1/8/8/8/8/8/8 b k - 0 1", "185867"},
//       {"r3k3/1K6/8/8/8/8/8/8 b q - 0 1", "413018"},
//       {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "179862938"},
//       {"r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1", "198328929"},
//       {"r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1", "185959088"},
//       {"r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1", "190755813"},
//       {"1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", "195629489"},
//       {"2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", "184411439"},
//       {"r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1", "189224276"},
//       {"8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1", "8107539"},
//       {"8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1", "2594412"},
//       {"8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1", "19870403"},
//       {"K7/8/2n5/1n6/8/8/8/k6N w - - 0 1", "588695"},
//       {"k7/8/2N5/1N6/8/8/8/K6n w - - 0 1", "688780"},
//       {"8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1", "8503277"},
//       {"8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1", "3147566"},
//       {"8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1", "4405103"},
//       {"K7/8/2n5/1n6/8/8/8/k6N b - - 0 1", "688780"},
//       {"k7/8/2N5/1N6/8/8/8/K6n b - - 0 1", "588695"},
//       {"B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1", "22823890"},
//       {"8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1", "28861171"},
//       {"k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1", "7881673"},
//       {"K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1", "7382896"},
//       {"B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1", "9250746"},
//       {"8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1", "29027891"},
//       {"k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1", "7382896"},
//       {"K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1", "7881673"},
//       {"7k/RR6/8/8/8/8/rr6/7K w - - 0 1", "44956585"},
//       {"R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1", "525169084"},
//       {"7k/RR6/8/8/8/8/rr6/7K b - - 0 1", "44956585"},
//       {"R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1", "524966748"},
//       {"6kq/8/8/8/8/8/8/7K w - - 0 1", "391507"},
//       {"6KQ/8/8/8/8/8/8/7k b - - 0 1", "391507"},
//       {"K7/8/8/3Q4/4q3/8/8/7k w - - 0 1", "3370175"},
//       {"6qk/8/8/8/8/8/8/7K b - - 0 1", "419369"},
//       {"6KQ/8/8/8/8/8/8/7k b - - 0 1", "391507"},
//       {"K7/8/8/3Q4/4q3/8/8/7k b - - 0 1", "3370175"},
//       {"8/8/8/8/8/K7/P7/k7 w - - 0 1", "6249"},
//       {"8/8/8/8/8/7K/7P/7k w - - 0 1", "6249"},
//       {"K7/p7/k7/8/8/8/8/8 w - - 0 1", "2343"},
//       {"7K/7p/7k/8/8/8/8/8 w - - 0 1", "2343"},
//       {"8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1", "34834"},
//       {"8/8/8/8/8/K7/P7/k7 b - - 0 1", "2343"},
//       {"8/8/8/8/8/7K/7P/7k b - - 0 1", "2343"},
//       {"K7/p7/k7/8/8/8/8/8 b - - 0 1", "6249"},
//       {"7K/7p/7k/8/8/8/8/8 b - - 0 1", "6249"},
//       {"8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1", "34822"},
//       {"8/8/8/8/8/4k3/4P3/4K3 w - - 0 1", "11848"},
//       {"4k3/4p3/4K3/8/8/8/8/8 b - - 0 1", "11848"},
//       {"8/8/7k/7p/7P/7K/8/8 w - - 0 1", "10724"},
//       {"8/8/k7/p7/P7/K7/8/8 w - - 0 1", "10724"},
//       {"8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1", "53138"},
//       {"8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1", "157093"},
//       {"8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1", "158065"},
//       {"k7/8/3p4/8/3P4/8/8/7K w - - 0 1", "20960"},
//       {"8/8/7k/7p/7P/7K/8/8 b - - 0 1", "10724"},
//       {"8/8/k7/p7/P7/K7/8/8 b - - 0 1", "10724"},
//       {"8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1", "53138"},
//       {"8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1", "158065"},
//       {"8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1", "157093"},
//       {"k7/8/3p4/8/3P4/8/8/7K b - - 0 1", "21104"},
//       {"7k/3p4/8/8/3P4/8/8/K7 w - - 0 1", "32191"},
//       {"7k/8/8/3p4/8/8/3P4/K7 w - - 0 1", "30980"},
//       {"k7/8/8/7p/6P1/8/8/K7 w - - 0 1", "41874"},
//       {"k7/8/7p/8/8/6P1/8/K7 w - - 0 1", "29679"},
//       {"k7/8/8/6p1/7P/8/8/K7 w - - 0 1", "41874"},
//       {"k7/8/6p1/8/8/7P/8/K7 w - - 0 1", "29679"},
//       {"k7/8/8/3p4/4p3/8/8/7K w - - 0 1", "22886"},
//       {"k7/8/3p4/8/8/4P3/8/7K w - - 0 1", "28662"},
//       {"7k/3p4/8/8/3P4/8/8/K7 b - - 0 1", "32167"},
//       {"7k/8/8/3p4/8/8/3P4/K7 b - - 0 1", "30749"},
//       {"k7/8/8/7p/6P1/8/8/K7 b - - 0 1", "41874"},
//       {"k7/8/7p/8/8/6P1/8/K7 b - - 0 1", "29679"},
//       {"k7/8/8/6p1/7P/8/8/K7 b - - 0 1", "41874"},
//       {"k7/8/6p1/8/8/7P/8/K7 b - - 0 1", "29679"},
//       {"k7/8/8/3p4/4p3/8/8/7K b - - 0 1", "22579"},
//       {"k7/8/3p4/8/8/4P3/8/7K b - - 0 1", "28662"},
//       {"7k/8/8/p7/1P6/8/8/7K w - - 0 1", "41874"},
//       {"7k/8/p7/8/8/1P6/8/7K w - - 0 1", "29679"},
//       {"7k/8/8/1p6/P7/8/8/7K w - - 0 1", "41874"},
//       {"7k/8/1p6/8/8/P7/8/7K w - - 0 1", "29679"},
//       {"k7/7p/8/8/8/8/6P1/K7 w - - 0 1", "55338"},
//       {"k7/6p1/8/8/8/8/7P/K7 w - - 0 1", "55338"},
//       {"3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1", "199002"},
//       {"7k/8/8/p7/1P6/8/8/7K b - - 0 1", "41874"},
//       {"7k/8/p7/8/8/1P6/8/7K b - - 0 1", "29679"},
//       {"7k/8/8/1p6/P7/8/8/7K b - - 0 1", "41874"},
//       {"7k/8/1p6/8/8/P7/8/7K b - - 0 1", "29679"},
//       {"k7/7p/8/8/8/8/6P1/K7 b - - 0 1", "55338"},
//       {"k7/6p1/8/8/8/8/7P/K7 b - - 0 1", "55338"},
//       {"3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1", "199002"},
//       {"8/Pk6/8/8/8/8/6Kp/8 w - - 0 1", "1030499"},
//       {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1", "37665329"},
//       {"8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1", "28859283"},
//       {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1", "71179139"},
//       {"8/Pk6/8/8/8/8/6Kp/8 b - - 0 1", "1030499"},
//       {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1", "37665329"},
//       {"8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1", "28859283"},
//       {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", "71179139"}};
//   for (auto data : chessData) {
//     Position position(data[0]);
//     perftTest test(position);
//     uint64_t perftResult = test.perftZobrist(6);
//     std::cout << data[0] << " " << data[1] << '\n';
//     ASSERT_EQ(stoull(data[1]), perftResult);
//   }
// }
// // TEST(Perft, perftFENMultipleQueens) {

//   Position position("8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1");
//   perftTest test(position);
//   // perftDivideInterface("8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1");
//   uint64_t perftResult = test.perft(6);
//   ASSERT_EQ(28859283, perftResult);
// }
// TEST(Perft, perftListRemainder) {
//   std::vector<std::vector<std::string>> chessData = {
//       {"8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1", "28859283"},
//       {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1", "71179139"},
//       {"8/Pk6/8/8/8/8/6Kp/8 b - - 0 1", "1030499"},
//       {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1", "37665329"},
//       {"8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1", "28859283"},
//       {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", "71179139"}};
//   for (auto data : chessData) {
//     Position position(data[0]);
//     perftTest test(position);
//     uint64_t perftResult = test.perft(6);
//     std::cout << data[0] << " " << data[1] << '\n';
//     ASSERT_EQ(stoull(data[1]), perftResult);
//   }
// }

// Eval
// TEST(Evaluation, eval) {
//   Position position(
//       "r1bqkb1r/ppp4p/3p1np1/3Ppp2/1nP1P3/2N2P2/PP2N1PP/R1BQKB1R w KQkq - 1
//       8");
//   perftTest test(position);
//   uint val = test.perft(4);
//   ASSERT_EQ(1217394, val);
//   Evaluation eval(position);
//   std::cout << eval.evaluate() << std::endl;
//   Search sc(position);
//   MoveGeneration mg(position);
//   mg.generateAllMoves();
//   MoveList ml = mg.getMoves();
//   sc.scoreMoves(ml);
//   for (Move move : ml) {
//     std::cout << move.getScore() << '\n';
//   }
//   sc.pickMove(ml, 0);

//   std::cout << ml[0].getScore() << '\n';
// }
TEST(Evaluation, king_evalTEST) {
  Position position(
      "r1bqk1nr/ppp2ppp/2np4/2b5/2BpP3/5N2/PPPN1PPP/R1BQK2R w KQkq - 0 6");
  perftTest test(position);
  uint val = test.perft(4, position);
  ASSERT_EQ(1511714, val);
  position.setPly(20);
  Evaluation eval(position);
  std::cout << eval.evaluate() << std::endl;
  Position position1(
      "r1bqk1nr/ppp2ppp/2np4/2b5/2BpP3/5N2/PPPNKPPP/R1BQ3R b kq - 1 6");
  position1.setPly(20);
  // perftTest test1(position);
  // uint val = test.perft(4);
  // ASSERT_EQ(1511714, val);
  Evaluation eval1(position1);
  std::cout << eval1.evaluate() << std::endl;
}
// Search tests Ply
// TEST(SearchTest, Ply_constructor) {

//   std::string FENstr =
//       "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";

//   Position position(FENtoPieces(FENstr), white);
//   Search s(position);
//   ASSERT_EQ(s.getPly(), 0);
// }
// TEST(SearchTest, Plycounter_SearchAB_5) {

//   std::string FENstr =
//       "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
//   Position position(FENtoPieces(FENstr), white);
//   Search s(position);
//   int initialPly = s.getPly();
//   auto start = std::chrono::high_resolution_clock::now();
//   s.searchAB(1, start, 1000, 10);
//   int finalPly = s.getPly();
//   ASSERT_EQ(finalPly, initialPly);
// }
// TEST(SearchTest, Plycounter_SearchAB_4) {

//   std::string FENstr =
//       "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
//   Position position(FENtoPieces(FENstr), white);
//   Search s(position);
//   int initialPly = s.getPly();
//   auto start = std::chrono::high_resolution_clock::now();
//   s.searchAB(2, start, 1000, 10);
//   int finalPly = s.getPly();
//   ASSERT_EQ(finalPly, initialPly);
// }

// Killer move test
// TEST(SearchTest, killerMove_Initilization) {
//   std::string FENstr =
//       "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
//   Position position(FENtoPieces(FENstr), white);
//   Search s(position);
//   std::array<std::array<Move, MAX_DEPTH>, MAX_KILLER_MOVES> killerMoves =
//       s.getKillerMoves();
//   Move invalidMove(a1, a1, 0);
//   ASSERT_EQ(killerMoves[1][63].getScore(), invalidMove.getScore());
// }
// TEST(SearchTest, SearchLegalityTest) {

//   Position position;
//   position.setBoardToInitialConfiguration();
//   int i = 0;
//   int wtime = 8080;
//   int winc = 80;
//   int btime = 8080;
//   int binc = 80;
//   int maxDepth = 500;
//   while (i < 2) {

//     MoveGeneration mg(position);
//     Search srch(position, wtime, winc, btime, binc);
//     Move bestMove = srch.searchIt(maxDepth, false);
//     ASSERT_TRUE(position.makeMove(bestMove));
//     // position.printBoard();
//     i++;
//   }
// }

// // pick move
// TEST(SearchTest, pickMove_copy) {

//   std::string FENstr =
//       "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
//   Position position(FENtoPieces(FENstr), white);
//   MoveGeneration movegen(position);
//   movegen.generateAllMoves();
//   // Copy
//   MoveList moves = movegen.getMoves();
//   Search s(position);
//   s.scoreMoves(moves);
//   for (int startingMove = 0; startingMove < moves.size(); startingMove++) {

//     s.pickMove(moves, startingMove);

//     for (int i = startingMove; i < moves.size(); i++) {
//       ASSERT_GE(moves[startingMove].getScore(), moves[i].getScore());
//     }
//   }
// }
// TEST(SearchTest, pickMove_nocopy) {

//   std::string FENstr =
//       "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
//   Position position(FENtoPieces(FENstr), white);
//   MoveGeneration movegen(position);
//   movegen.generateAllMoves();
//   Search s(position);
//   // pass by reference;
//   s.scoreMoves(movegen.getMoves());
//   for (int startingMove = 0; startingMove < movegen.getMoves().size();
//        startingMove++) {

//     s.pickMove(movegen.getMoves(), startingMove);
//     for (int i = startingMove; i < movegen.getMoves().size(); i++) {
//       ASSERT_GE(movegen.getMoves()[startingMove].getScore(),
//                 movegen.getMoves()[i].getScore());
//     }
//   }
// }

// UCI
TEST(UCI, uciLoopTEST) {
  UCITestHelper::SetUp("uci\nposition startpos\nisready\n");

  UCI uci;
  uci.loop(); // Run the loop with predefined commands

  // Add assertions to verify the behavior of your UCI class
  // based on the predefined commands

  UCITestHelper::TearDown();
}
// TEST(UCI, uciManualTESTSequenceOfMOves) {

//   UCI uci;
//   uci.manual("uci");
//   // uci.getPosition().printBoard();
//   uci.manual("position startpos");
//   uci.getPosition().printBoard();
//   uci.manual("isready");
//   uci.getPosition().printBoard();
//   uci.manual("go wtime 35939 btime 38039 winc 80 binc 80");
//   uci.getPosition().printBoard();
//   uci.manual("position startpos moves d2d4");
//   uci.getPosition().printBoard();
//   uci.manual("isready");
//   uci.getPosition().printBoard();
//   uci.manual("go wtime 35939 btime 38039 winc 80 binc 80");
//   uci.getPosition().printBoard();
//   uci.manual("position startpos moves d2d4 h7h6 e2e4");
//   uci.getPosition().printBoard();
// }
// TEST(UCI, uciManualTESTSingleLineManyMoves) {

//   UCI uci;
//   uci.manual("uci");
//   // uci.getPosition().printBoard();
//   uci.manual("position startpos");
//   uci.getPosition().printBoard();
//   uci.manual("isready");
//   uci.getPosition().printBoard();
//   uci.manual("position startpos moves d2d4 e7e5");
//   uci.getPosition().printBoard();
//   uci.manual("isready");
//   uci.getPosition().printBoard();
//   uci.manual("go wtime 35939 btime 38039 winc 80 binc 80");
//   uci.getPosition().printBoard();
//   uci.manual("position startpos moves d2d4 e7e5 a2a3");
//   uci.getPosition().printBoard();
//   uci.manual("isready");
//   uci.getPosition().printBoard();
// }
// TEST(UCI, timeControl) {
//   UCI uci;
//   uci.manual(
//       "position startpos moves e2e4 e7e5 g1f3 g8f6 d2d4 f6e4 d4e5 d7d5 f1b5 "
//       "c7c6 b5d3 f8c5 d3e4 d5e4 d1d8 e8d8 f3g5 d8c7 g5e4 b8d7 f2f4 f7f6 e5e6
//       " "c5b4 b1d2 d7c5 e4c5 b4c5 f4f5 c5b4 e1f1 c6c5 d2c4 c7c6 a2a3 b4c3
//       b2c3 " "b7b5 c4e3 c8b7 c3c4 b5b4 a3b4 c5b4 c1d2 c6c5 a1b1");
//   uci.getPosition().printBoard();
//   uci.manual("isready");
//   uci.getPosition().printBoard();

//   uci.manual("go wtime 2234 btime 2235 winc 80 binc 80");
// }
// comeToStr
TEST(moveToStr, strToMove_Nocatpure) {
  Position position;
  position.setBoardToInitialConfiguration();
  moveToStr uci(position);
  std::string moveStr = "d2d4";
  Move UCImove = uci.getMove(moveStr);
  Move handMade(D2, D4, doublePawnPush);
  ASSERT_EQ(handMade.getMoveInt(), UCImove.getMoveInt());
}
TEST(moveToStr, strToMove_capture) {
  std::string FENstr =
      "r2q1rk1/1p1nbppp/p2pbn2/4p3/4P3/1NN1BP2/PPPQ2PP/2KR1B1R/";
  Position position(FENtoPieces(FENstr), WHITE);
  moveToStr uci(position);
  std::string moveStr = "d2d6";
  Move UCImove = uci.getMove(moveStr);
  Move handMade(D2, D6, captures);
  ASSERT_EQ(handMade.getMoveInt(), UCImove.getMoveInt());
}
TEST(moveToStr, makecastling_white_king_side) {

  Position position(
      "rn3rk1/2q1bpp1/pp1pbn2/4p1Pp/4P3/1NNBBP2/PPPQ3P/R3K2R w KQ - 2 13");
  moveToStr uci(position);
  Move move = uci.getMove("e1g1");
  Move handMade(E1, G1, kingCastle);
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