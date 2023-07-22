#include "position.hpp"
#include <cstdint>
#include <iostream>

Position::Position() {}
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
  can_white_castle_kingside = true;
  can_white_castle_queenside = true;
  can_black_castle_kingside = true;
  can_black_castle_queenside = true;
}
void Position::print_board() {
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
// TODO implement those
//  void Position::move_piece(int from_square, int to_square) {}
//  void Position::pawn_promotion(int square, char piece) {}
//  void Position::print_board();
bool Position::is_square_empty(int square) {
  uint64_t pieces = white_pieces.all | black_pieces.all;
  uint64_t square_mask = 1ULL << square;
  return pieces & square_mask == 0;
}
bool Position::is_destination_occupied_by_same_color(int source,
                                                     int destination) {
  if (turn == "white") {
    return (white_pieces.all & (1ULL << destination)) != 0;
  } else {
    return (black_pieces.all & (1ULL << destination)) != 0;
  }
}

// TODO implement these
//  bool Position::is_pawn_path_clear(int source, int destination);
// bool Position::is_path_clear(int source, int destination) {
// if is_destination_occupied_by_same_color (source, destination) {
// return false;
// }
// if piece_type (source)
// }
// bool Position::is_under_attack_by_slider(int position, int attacker_color);
// bool Position::is_under_attack_by_pawn(int position, int attacker_color);
// bool Position::is_under_attack_by_specific_piece_type(int position,
//                                                       int attacker_color,
//                                                       int piece_type);
// bool Position::is_under_attack_by_any_piece_type(int position,
//  int attacker_color);
// bool Position::is_pawn_promotion(self);
// bool Position::is_castling_attempt(self);
// bool Position::is_queen_side_castling_legal();
// bool Position::is_king_side_castling_legal();
// bool Position::is_castling_legal();
// bool Position::is_new_position_check(self);
// bool Position::is_safe();
// bool Position::is_legal(int position, int destination);
// // bool is_pawn_promotion_legal(int position, int destination);
// void Position::print_board() {
//   std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
//   for (int i = 0; i < 8; i++) {
//     std::cout << 8 - i << " |";
//     for (int j = 0; j < 8; j++) {
//       if (white_pieces & (1ULL << (i * 8 + j))) {
//         std::cout << " W |";
//       } else if (black_pieces & (1ULL << (i * 8 + j))) {
//         std::cout << " B |";
//       } else {
//         std::cout << "   |";
//       }
//     }
//     std::cout << std::endl;
//     std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
//   }
//   std::cout << "    a   b   c   d   e   f   g   h  " << std::endl;
// }