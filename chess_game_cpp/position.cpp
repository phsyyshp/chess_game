#include "position.hpp"
#include <cstdint>
#include <iostream>

Position::Position() {}
void Position::set_white_pieces_to_initial_configuration() {
  rooks_white = 0b1ULL | 0b1ULL << 7;
  knights_white = 0b1ULL << 6 | 0b1ULL << 1;
  bishops_white = 0b1ULL << 5 | 0b1ULL << 2;
  queens_white = 0b1ULL << 4;
  king_white = 0b1ULL << 3;
  pawns_white = 0b11111111ULL << 8;
  white_pieces = rooks_white | knights_white | bishops_white | queens_white |
                 king_white | pawns_white;
}
void Position::set_black_pieces_to_initial_configuration() {
  rooks_black = 0b1ULL << (8 * 7) | 0b1ULL << (7 + 8 * 7);
  knights_black = 0b1ULL << (6 + 8 * 7) | 0b1ULL << (1 + 8 * 7);
  bishops_black = 0b1ULL << (5 + 8 * 7) | 0b1ULL << (2 + 8 * 7);
  queens_black = 0b1ULL << (4 + 8 * 7);
  king_black = 0b1ULL << (3 + 8 * 7);
  pawns_black = 0b11111111ULL << (8 * 6);
  black_pieces = rooks_black | knights_black | bishops_black | queens_black |
                 king_black | pawns_black;
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
  uint64_t n = white_pieces | black_pieces;
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
void Position::move_piece(int from_square, int to_square) {}
void Position::pawn_promotion(int square, char piece) {}
void Position::print_board();
bool Position::is_square_empty(int square) {
  pieces = white_pieces | black_pieces;
  square_mask = 1ULL << square;
  return pieces & square_mask == 0;
}
bool Position::is_destination_occupied_by_same_color(int source,
                                                     int destination) {
  if (turn == "white") {
    return white_pieces & (1ULL << destination) != 0;
  } else {
    return black_pieces & (1ULL << destination) != 0;
  }
}

bool Position::is_pawn_path_clear(int source, int destination);
bool Position::is_path_clear(int source, int destination) {
  if is_destination_occupied_by_same_color (source, destination) {
    return false;
  }
  if piece_type (source)
}
bool Position::is_under_attack_by_slider(int position, int attacker_color);
bool Position::is_under_attack_by_pawn(int position, int attacker_color);
bool Position::is_under_attack_by_specific_piece_type(int position,
                                                      int attacker_color,
                                                      int piece_type);
bool Position::is_under_attack_by_any_piece_type(int position,
                                                 int attacker_color);
// bool Position::is_pawn_promotion(self);
// bool Position::is_castling_attempt(self);
bool Position::is_queen_side_castling_legal();
bool Position::is_king_side_castling_legal();
bool Position::is_castling_legal();
// bool Position::is_new_position_check(self);
bool Position::is_safe();
bool Position::is_legal(int position, int destination);
// bool is_pawn_promotion_legal(int position, int destination);
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