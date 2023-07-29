#include <cstdint>
#include <iostream>
#include <string>

class Position {
public:
  std::string turn;
  struct canWhiteCastle {
    bool king_side;
    bool queen_side;
  };
  struct canBlackCastle {
    bool king_side;
    bool queen_side;
  };
  struct whitePieces {
    uint64_t queens;
    uint64_t bishops;
    uint64_t knights;
    uint64_t rooks;
    uint64_t pawns;
    uint64_t king;
    uint64_t all;
  };
  struct blackPieces {
    uint64_t queens;
    uint64_t bishops;
    uint64_t knights;
    uint64_t rooks;
    uint64_t pawns;
    uint64_t king;
    uint64_t all;
  };
  whitePieces white_pieces;
  blackPieces black_pieces;
  canWhiteCastle can_white_castle;
  canBlackCastle can_black_castle;

public:
  Position();

public:
  void set_white_pieces_to_initial_configuration();
  void set_black_pieces_to_initial_configuration();
  void set_board_to_initial_configuration();
  void change_turn();
  void print_board();
  bool is_square_empty(int square);
  bool is_destination_occupied_by_same_color(int source, int destination);
};
