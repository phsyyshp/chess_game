#include "loader.hpp"
#include <cstdint>
#include <iostream>
#include <string>

enum class color {
  white,
  black

};
class Position {
public:
  struct colored {
    uint64_t white;
    uint64_t black;
  };

  struct Pieces {
    colored queens;
    colored bishops;
    colored knights;
    colored rooks;
    colored pawns;
    colored king;
    colored all;
  };
  struct canCastle {
    bool king_side;
    bool queen_side;
  };

  Position() = default;

  void set_board_to_initial_configuration();
  Pieces get_pieces() const;
  color get_piece_color(const uint64_t &position) const;
  std::string get_piece_type(const uint64_t &position) const;

  void change_turn();
  void print_board() const;

  bool is_square_empty(const uint64_t &square_mask) const;
  bool is_destination_occupied_by_same_color(const int &source,
                                             const int &destination) const;
  bool is_sliding_move(const std::string &piecetype) const;

  bool is_knight_move_legal(const int &source,
                            const uint64_t &destination_mask) const;
  bool is_single_pawn_move_legal(const uint64_t &source_mask,
                                 const uint64_t &destination_mask) const;
  bool is_double_pawn_move_legal(const uint64_t &source_mask,
                                 const uint64_t &destination_mask) const;
  bool is_pawn_move_legal(const uint64_t &source_mask,
                          const uint64_t &destination_mask,
                          const u_int64_t &all_pieces) const;
  bool is_pseudo_legal_move(const int &source, const int &destination) const;

  bool is_sliding_move_legal(const uint64_t &source_mask,
                             const uint64_t &destination_mask,
                             const uint64_t &all_pieces,
                             const std::string &piece_type) const;

  // Pieces get_black_pieces() const;

private:
  void set_white_pieces_to_initial_configuration();
  void set_black_pieces_to_initial_configuration();
  // slider cache
  static const std::vector<uint64_t> rook_magic_numbers;
  static const std::vector<std::vector<uint64_t>> rook_look_up_tables;
  static const std::vector<uint64_t> bishop_magic_numbers;
  static const std::vector<std::vector<uint64_t>> bishop_look_up_tables;
  // rest cache
  static const std::vector<uint64_t> knight_look_up_table;
  static const std::vector<uint64_t> white_pawn_look_up_table;
  static const std::vector<uint64_t> black_pawn_look_up_table;

  color turn;
  Pieces pieces;
  canCastle can_white_castle;
  canCastle can_black_castle;
};
