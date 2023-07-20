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
  void set_board_to_FEN(std::string FEN);
  void change_turn();
  void move_piece(int from_square, int to_square);
  void pawn_promotion(int square, char piece);
  void print_board();
  bool is_square_empty(int square);
  bool is_destination_occupied_by_same_color(int source, int destination);
  bool is_pawn_path_clear(int source, int destination);
  bool is_path_clear(int source, int destination);
  bool is_under_attack_by_slider(int position, int attacker_color);
  bool is_under_attack_by_pawn(int position, int attacker_color);
  bool is_under_attack_by_specific_piece_type(int position, int attacker_color,
                                              int piece_type);
  bool is_under_attack_by_any_piece_type(int position, int attacker_color);
  // bool is_pawn_promotion(self);
  // bool is_castling_attempt(self);
  bool is_queen_side_castling_legal();
  bool is_king_side_castling_legal();
  bool is_castling_legal();
  // bool is_new_position_check(self);
  bool is_safe();
  bool is_legal(int position, int destination);
  // bool is_pawn_promotion_legal(int position, int destination);
};
