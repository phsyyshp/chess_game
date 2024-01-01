#include "position.hpp"
// #include "loader.hpp"
// #include <cstdint>
// #include <iostream>

const std::vector<uint64_t> rook_magic_numbers =
    read_magic_numbers_to_vec("rook");
const std::vector<std::vector<uint64_t>> rook_look_up_tables =
    read_look_up_tables("rook");

Position::Pieces Position::get_white_pieces() const { return white_pieces; }
Position::Pieces Position::get_black_pieces() const { return black_pieces; }

void Position::set_white_pieces_to_initial_configuration() {
  white_pieces.rooks = 0b1ULL | 0b1ULL << 7;
  white_pieces.knights = 0b1ULL << 6 | 0b1ULL << 1;
  white_pieces.bishops = 0b1ULL << 5 | 0b1ULL << 2;
  white_pieces.queens = 0b1ULL << 4;
  white_pieces.king = 0b1ULL << 3;
  white_pieces.pawns = 0b11111111ULL << 8;
  white_pieces.all = white_pieces.rooks | white_pieces.knights |
                     white_pieces.bishops | white_pieces.queens |
                     white_pieces.king | white_pieces.pawns;
}
void Position::set_black_pieces_to_initial_configuration() {
  black_pieces.rooks = 0b1ULL << (8 * 7) | 0b1ULL << (7 + 8 * 7);
  black_pieces.knights = 0b1ULL << (6 + 8 * 7) | 0b1ULL << (1 + 8 * 7);
  black_pieces.bishops = 0b1ULL << (5 + 8 * 7) | 0b1ULL << (2 + 8 * 7);
  black_pieces.queens = 0b1ULL << (4 + 8 * 7);
  black_pieces.king = 0b1ULL << (3 + 8 * 7);
  black_pieces.pawns = 0b11111111ULL << (8 * 6);
  black_pieces.all = black_pieces.rooks | black_pieces.knights |
                     black_pieces.bishops | black_pieces.queens |
                     black_pieces.king | black_pieces.pawns;
}
void Position::set_board_to_initial_configuration() {
  set_white_pieces_to_initial_configuration();
  set_black_pieces_to_initial_configuration();
  turn = "white";
  can_white_castle.king_side = true;
  can_white_castle.queen_side = true;
  can_black_castle.king_side = true;
  can_black_castle.queen_side = true;
}
void Position::print_board() const {
  uint64_t n = white_pieces.all | black_pieces.all;
  for (int i = 63; i >= 0; i--) {
    if (i % 8 == 7) {
      std::cout << "\n";
    } else {
      std::cout << ((n >> i) & 1);
    }
  }
  std::cout << "\n";
}
void Position::change_turn() {
  if (turn == "white") {
    turn = "black";
  } else {
    turn = "white";
  }
}
bool Position::is_square_empty(const int &square) const {
  uint64_t pieces = white_pieces.all | black_pieces.all;
  uint64_t square_mask = 1ULL << square;
  return pieces & square_mask == 0;
}
bool Position::is_destination_occupied_by_same_color(
    const int &source, const int &destination) const {
  if (turn == "white") {
    return (white_pieces.all & (1ULL << destination)) != 0;
  } else {
    return (black_pieces.all & (1ULL << destination)) != 0;
  }
}
bool Position::is_sliding_move(const std::string &piecetype) const {
  return piecetype == "bishop" || piecetype == "queen" || piecetype == "rook";
}
std::string Position ::get_piece_color(const uint64_t &position) const {

  if (position | black_pieces.all) {
    return "black";
  } else if (position | white_pieces.all) {
    return "white";
  } else {
    std::cerr << "empty square";
  }
}
std::string Position::get_piece_type(const uint64_t &position) const {
  std::string piece_color = get_piece_color(position);
  std::string piece_type;
  if (piece_color == "white") {
    if (position & white_pieces.rooks) {
      piece_type = "rook";
    } else if (position & white_pieces.pawns) {
      piece_type = "pawn";
    } else if (position & white_pieces.queens) {

      piece_type = "queen";
    } else if (position & white_pieces.knights) {

      piece_type = "knight";
    } else if (position & white_pieces.king) {

      piece_type = "king";
    } else if (position & white_pieces.bishops) {

      piece_type = "bishop";
    }
  } else if (piece_color == "black") {
    if (position & black_pieces.rooks) {
      piece_type = "rook";
    } else if (position & black_pieces.pawns) {
      piece_type = "pawn";
    } else if (position & black_pieces.queens) {

      piece_type = "queen";
    } else if (position & black_pieces.knights) {

      piece_type = "knight";
    } else if (position & black_pieces.king) {

      piece_type = "king";
    } else if (position & black_pieces.bishops) {

      piece_type = "bishop";
    }
  }
  return piece_type;
}
bool Position::is_pseudo_legal_move(const int &source,
                                    const int &destination) const {
  if (is_square_empty(source)) {
    return false;
  };
  uint64_t source_position = 0b1uLL << source;
  std::string piece_type = get_piece_type(source_position);
  std::string piece_color = get_piece_color(source_position);
  if (turn != piece_color) {
    return false;
  } else if (is_destination_occupied_by_same_color(source, destination)) {
    return false;
  } else if (is_sliding_move(piece_type)) {
    uint64_t all = white_pieces.all | black_pieces.all;
    if (piece_type == "bishop") {
      // /TODO fix it
      return get_attack_mask(source_position, all, bishop_magic_numbers,
                             bishop_look_up_tables, piece_type);
    } else if (piece == "rook") {
    }
  }
}
