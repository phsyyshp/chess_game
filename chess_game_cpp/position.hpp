#include <cstdint>
#include <iostream>
#include <string>

class Position {
public:
  std::string turn;
  bool can_white_castle_kingside;
  bool can_white_castle_queenside;
  bool can_black_castle_kingside;
  bool can_black_castle_queenside;
  uint64_t rooks_white;
  uint64_t rooks_black;
  uint64_t knights_white;
  uint64_t knights_black;
  uint64_t bishops_white;
  uint64_t bishops_black;
  uint64_t queens_white;
  uint64_t queens_black;
  uint64_t king_white;
  uint64_t king_black;
  uint64_t pawns_white;
  uint64_t pawns_black;
  uint64_t white_pieces;
  uint64_t black_pieces;

public:
  Position();

public:
  void set_white_pieces_to_initial_configuration();
  void set_black_pieces_to_initial_configuration();
  void set_board_to_initial_configuration();
  void print_board();
};
