#include "loader.hpp"
#include <cstdint>
#include <iostream>
#include <string>

enum class piece_types : int {
  quuens,
  bishops,
  knights,
  rooks,
  pawns,
  kings,
  all
};
enum class color : int { white, black };
class Position {
public:
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
  bool is_sliding_move(const piece_types &piecetype) const;

  std::string get_piece_color(const uint64_t &position) const;
  std::string get_piece_type(const uint64_t &position) const;
  bool is_pseudo_legal_move(const int &source, const int &destination) const;

  bool is_sliding_move_legal(const uint64_t &source_mask,
                             const uint64_t &destination_mask,
                             const uint64_t &all_pieces,
                             const piece_types &piece_type) const;

  std::vector<std::vector<piece_types>> get_pieces() const;
  // Pieces get_black_pieces() const;

private:
  void set_white_pieces_to_initial_configuration();
  void set_black_pieces_to_initial_configuration();
  static const std::vector<uint64_t> rook_magic_numbers;
  static const std::vector<std::vector<uint64_t>> rook_look_up_tables;

  static const std::vector<uint64_t> bishop_magic_numbers;
  static const std::vector<std::vector<uint64_t>> bishop_look_up_tables;

  std::string turn;
  std::vector<std::vector<u_int64_t>> pieces[2][7];

  // Pieces black_pieces;
  canCastle can_white_castle;
  canCastle can_black_castle;
};
