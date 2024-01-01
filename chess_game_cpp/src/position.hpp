#include "loader.hpp"
#include <cstdint>
#include <iostream>
#include <string>

class Position {
public:
  struct Pieces {
    uint64_t queens;
    uint64_t bishops;
    uint64_t knights;
    uint64_t rooks;
    uint64_t pawns;
    uint64_t king;
    uint64_t all;
  };
  struct canCastle {
    bool king_side;
    bool queen_side;
  };

  Position() = default;

  void set_board_to_initial_configuration();
  void change_turn();
  void print_board() const;
  bool is_square_empty(const int &square) const;
  bool is_destination_occupied_by_same_color(const int &source,
                                             const int &destination) const;
  bool is_sliding_move(const std::string &piecetype) const;

  std::string get_piece_color(const uint64_t &position) const;
  std::string get_piece_type(const uint64_t &position) const;
  bool is_pseudo_legal_move(const int &source, const int &destination) const;

  Pieces get_white_pieces() const;
  Pieces get_black_pieces() const;

private:
  void set_white_pieces_to_initial_configuration();
  void set_black_pieces_to_initial_configuration();
  static const std::vector<uint64_t> rook_magic_numbers;
  static const std::vector<std::vector<uint64_t>> rook_look_up_tables;

  static const std::vector<uint64_t> bishop_magic_numbers;
  static const std::vector<std::vector<uint64_t>> bishop_look_up_tables;

  std::string turn;
  Pieces white_pieces;
  Pieces black_pieces;
  canCastle can_white_castle;
  canCastle can_black_castle;
};
