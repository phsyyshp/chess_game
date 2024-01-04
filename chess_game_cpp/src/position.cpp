#include "position.hpp"
// #include "loader.hpp"
// #include <cstdint>
// #include <iostream>

// Initilazers;
const std::vector<uint64_t> Position::rook_magic_numbers =
    read_magic_numbers_to_vec("rook");
const std::vector<std::vector<uint64_t>> Position::rook_look_up_tables =
    read_look_up_tables("rook");

const std::vector<uint64_t> Position::bishop_magic_numbers =
    read_magic_numbers_to_vec("bishop");
const std::vector<std::vector<uint64_t>> Position::bishop_look_up_tables =
    read_look_up_tables("bishop");
const std::vector<uint64_t> Position::knight_look_up_table =
    read_knight_look_up_table();
const std::vector<uint64_t> Position::white_pawn_look_up_table =
    read_white_pawn_look_up_table();

const std::vector<uint64_t> Position::black_pawn_look_up_table =
    read_white_pawn_look_up_table();

// Setters
void Position::set_white_pieces_to_initial_configuration() {
  pieces.rooks.white = 0b1ULL | 0b1ULL << 7;
  pieces.knights.white = 0b1ULL << 6 | 0b1ULL << 1;
  pieces.bishops.white = 0b1ULL << 5 | 0b1ULL << 2;
  pieces.queens.white = 0b1ULL << 4;
  pieces.king.white = 0b1ULL << 3;
  pieces.pawns.white = 0b11111111ULL << 8;
  pieces.all.white = pieces.rooks.white | pieces.knights.white |
                     pieces.bishops.white | pieces.queens.white |
                     pieces.king.white | pieces.pawns.white;
}
void Position::set_black_pieces_to_initial_configuration() {
  pieces.rooks.black = 0b1ULL << (8 * 7) | 0b1ULL << (7 + 8 * 7);
  pieces.knights.black = 0b1ULL << (6 + 8 * 7) | 0b1ULL << (1 + 8 * 7);
  pieces.bishops.black = 0b1ULL << (5 + 8 * 7) | 0b1ULL << (2 + 8 * 7);
  pieces.queens.black = 0b1ULL << (4 + 8 * 7);
  pieces.king.black = 0b1ULL << (3 + 8 * 7);
  pieces.pawns.black = 0b11111111ULL << (8 * 6);
  pieces.all.black = pieces.rooks.black | pieces.knights.black |
                     pieces.bishops.black | pieces.queens.black |
                     pieces.king.black | pieces.pawns.black;
}
void Position::set_board_to_initial_configuration() {
  set_white_pieces_to_initial_configuration();
  set_black_pieces_to_initial_configuration();
  turn = color::white;
  can_white_castle.king_side = true;
  can_white_castle.queen_side = true;
  can_black_castle.king_side = true;
  can_black_castle.queen_side = true;
}
// Getters
Position::Pieces Position::get_pieces() const { return pieces; }
// Position::Pieces Position::get_black_pieces() const { return pieces; }.black

color Position ::get_piece_color(const uint64_t &position_mask) const {

  if (position_mask | pieces.all.black) {
    return color::black;
  } else if (position_mask | pieces.all.white) {
    return color::white;
  } else {
    std::cerr << "empty square";
  }
}
std::string Position::get_piece_type(const uint64_t &position_mask) const {
  std::string piece_type;
  if (position_mask & (pieces.rooks.white | pieces.rooks.black)) {
    piece_type = "rook";
  } else if (position_mask & (pieces.pawns.white | pieces.pawns.black)) {
    piece_type = "pawn";
  } else if (position_mask & (pieces.queens.white | pieces.queens.black)) {

    piece_type = "queen";
  } else if (position_mask & (pieces.knights.white | pieces.knights.black)) {

    piece_type = "knight";
  } else if (position_mask & (pieces.king.white | pieces.king.black)) {

    piece_type = "king";
  } else if (position_mask & (pieces.bishops.white | pieces.bishops.black)) {

    piece_type = "bishop";
  }
  return piece_type;
}
// misc
void Position::print_board() const {
  uint64_t n = pieces.all.white | pieces.all.black;
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
  if (turn == color::white) {
    turn = color::black;
  } else {
    turn = color::white;
  }
}
// boleans
bool Position::is_square_empty(const uint64_t &square_mask) const {
  uint64_t all_pieces = pieces.all.white | pieces.all.black;
  return all_pieces & square_mask == 0;
}
bool Position::is_destination_occupied_by_same_color(
    const int &source, const int &destination) const {
  if (turn == color::white) {
    return (pieces.all.white & (1ULL << destination)) != 0;
  } else {
    return (pieces.all.black & (1ULL << destination)) != 0;
  }
}
bool Position::is_sliding_move(const std::string &piecetype) const {
  return piecetype == "bishop" || piecetype == "queen" || piecetype == "rook";
}
bool Position::is_sliding_move_legal(const uint64_t &source_mask,
                                     const uint64_t &destination_mask,
                                     const uint64_t &all_pieces,
                                     const std::string &piece_type) const {

  if (piece_type == "bishop") {
    return destination_mask &
           get_attack_mask(source_mask, all_pieces, bishop_magic_numbers,
                           bishop_look_up_tables, piece_type);
  } else if (piece_type == "rook") {

    return destination_mask & get_attack_mask(source_mask, all_pieces,
                                              rook_magic_numbers,
                                              rook_look_up_tables, piece_type);
  } else if (piece_type == "queen") {
    return destination_mask &
           (get_attack_mask(source_mask, all_pieces, bishop_magic_numbers,
                            bishop_look_up_tables, piece_type) |
            get_attack_mask(source_mask, all_pieces, rook_magic_numbers,
                            rook_look_up_tables, piece_type));
  } else {
    std::cerr << "it's not a sliding move";
    return false;
  }
}
bool Position::is_knight_move_legal(const int &source,
                                    const uint64_t &destination_mask) const {

  return destination_mask & knight_look_up_table[source];
};
// bool Position::is_en_passant(const uint64_t &source_mask,
//                              const uint64_t &destination_mask) const {

// };
bool Position::is_single_pawn_move_legal(
    const uint64_t &source_mask, const uint64_t &destination_mask) const {
  bool is_legal_push;
  uint64_t legal_captures;
  // TODO: there may be potential bugs regarding out of bound
  if (turn == color::white) {
    if (pieces.pawns.black &
        white_pawn_look_up_table[__builtin_ctzll(source_mask)]) {
      return true;
    }
    is_legal_push = (source_mask == (destination_mask >> 8));
  } else if (turn == color::black) {
    if (pieces.pawns.white &
        black_pawn_look_up_table[__builtin_ctzll(source_mask)]) {
      return true;
    }

    is_legal_push = (source_mask == (destination_mask << 8));
  }
  return is_legal_push && is_square_empty(destination_mask);
};
bool Position::is_double_pawn_move_legal(
    const uint64_t &source_mask, const uint64_t &destination_mask) const {
  // TODO: there may be potential bugs regarding out of bound
  bool is_legal_double_push;
  if (turn == color::white) {
    is_legal_double_push = is_square_empty(destination_mask << 8) &&
                           (source_mask == (destination_mask >> 16)) &&
                           (source_mask / 8 == 1);
  } else if (turn == color::black) {
    is_legal_double_push = (source_mask == (destination_mask << 16)) &&
                           (source_mask / 8 == 6) &&
                           is_square_empty(destination_mask >> 8);
  }
  return is_legal_double_push && is_square_empty(destination_mask);
};

bool Position::is_pawn_move_legal(const uint64_t &source_mask,
                                  const uint64_t &destination_mask,
                                  const u_int64_t &all_pieces) const {
  if (is_en_passant(source_mask, destination_mask)) {
    return is_en_passant_legal(source_mask, destination_mask, all_pieces);
  }
  return is_single_pawn_move_legal(source_mask, destination_mask) ||
         is_double_pawn_move_legal(source_mask, destination_mask);
}
bool Position::is_pseudo_legal_move(const int &source,
                                    const int &destination) const {
  uint64_t source_mask = 0b1uLL << source;
  if (is_square_empty(source_mask)) {
    return false;
  };
  uint64_t destination_mask = 0b1uLL << destination;
  std::string piece_type = get_piece_type(source_mask);
  color piece_color = get_piece_color(source_mask);
  uint64_t all_pieces = pieces.all.white | pieces.all.black;

  if (turn != piece_color) {
    return false;
  };
  if (piece_type == "pawn") {
    return is_pawn_move_legal(source_mask, destination_mask, all_pieces);
  }
  if (is_destination_occupied_by_same_color(source, destination)) {
    return false;
  };
  if (is_sliding_move(piece_type)) {
    return is_sliding_move_legal(source_mask, destination_mask, all_pieces,
                                 piece_type);
  };
  if (piece_type == "knight") {
    return is_knight_move_legal(source_mask, destination_mask);
  };
  // } else if (is_pawn_move(piece_type)) {
  // return is_pawn_move_legal(piece_type);
  // } else if (is_king_move())
}
