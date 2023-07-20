import numpy as np
import Piece as pc
from utilities.FENinterpreter import *
from utilities.PositionConverter import *
from utilities.Constants import *


class Board:
    def __init__(self):
        self.pawns_white = 1
        self.knight_white = 1
        self.rooks_white = 1
        self.bishops_white = 1
        self.queen_white = 1
        self.king_white = 1
        self.pawns_black = 1
        self.rooks_black = 1
        self.knight_black = 1
        self.queen_black = 1
        self.bishops_black = 1
        self.king_black = 1

        self.turn = "white"
        self.can_black_castle_kingside = True
        self.can_black_castle_queenside = True
        self.can_white_castle_kingside = True
        self.can_white_castle_queenside = True

    def change_turn(self):
        if self.turn == "white":
            self.turn = "black"
        else:
            self.turn = "white"

    def copy(self):
        board_copy = Board()
        board_copy.board_matrix = self.board_matrix.copy()
        board_copy.turn = self.turn
        board_copy.can_black_castle_kingside = self.can_black_castle_kingside
        board_copy.can_black_castle_queenside = self.can_black_castle_queenside
        board_copy.can_white_castle_kingside = self.can_white_castle_kingside
        board_copy.can_white_castle_queenside = self.can_white_castle_queenside
        # print(board_copy)
        return board_copy

    def set_white_pieces_to_initial_configurations(self):
        self.rooks_white = 0b1 | 0b1 << 7
        self.knight_white = 0b1 << 6 | 0b1 << 1
        self.bishops_white = 0b1 << 5 | 0b1 << 2
        self.queen_white = 0b1 << 4
        self.king_white = 0b1 << 3

        self.pawns_white = 0b11111111 << 8
        # self.pawns_white |= 1 << (8 + i)
        self.white_pieces = (
            self.rooks_white
            | self.knight_white
            | self.bishops_white
            | self.queen_white
            | self.king_white
            | self.pawns_white
        )

    def set_black_pieces_to_initial_configurations(self):
        self.rooks_black = 0b1 << 8 * 7 | 0b1 << 7 + 8 * 7
        self.knight_black = 0b1 << 6 + 8 * 7 | 0b1 << 1 + 8 * 7
        self.bishops_black = 0b1 << 5 + 8 * 7 | 0b1 << 2 + 8 * 7
        self.queen_black = 0b1 << 4 + 8 * 7
        self.king_black = 0b1 << 3 + 8 * 7

        self.pawns_black |= 0b11111111 << 8 * 6
        self.black_pieces = (
            self.rooks_black
            | self.knight_black
            | self.bishops_black
            | self.queen_black
            | self.king_black
            | self.pawns_black
        )

    def set_board_to_initial_configuration(self):
        self.set_black_pieces_to_initial_configurations()
        self.set_white_pieces_to_initial_configurations()
        self.all_pieces = self.white_pieces | self.black_pieces

    def set_board_to_fen_configuration(self, FEN_string):
        self.board_matrix = FEN_to_board_matrix(FEN_string)

    def get_piece_id_from_position(self, position_row_column: list):
        return self.board_matrix[tuple(position_row_column)]

    def is_square_empty(self, linear_position):
        return (self.all_pieces & (1 << linear_position)) == 0

    def create_piece_object(
        self, position_row_column: list, piece_type, color
    ) -> pc.Pawn | pc.Bishop | pc.Knight | pc.King | pc.Queen | pc.Rook:
        piece_types = {
            "pawn": pc.Pawn,
            "king": pc.King,
            "queen": pc.Queen,
            "knight": pc.Knight,
            "bishop": pc.Bishop,
            "rook": pc.Rook,
            "empty": pc.Empty,
        }
        return piece_types[piece_type](position_row_column, color)

    def get_piece_object_from_position(
        self, position_row_column: list
    ) -> pc.Pawn | pc.Bishop | pc.Knight | pc.King | pc.Queen | pc.Rook:
        piece_id = self.get_piece_id_from_position(position_row_column)
        piece_type, piece_color = piece_id_to_type_color(piece_id)
        piece_object = self.create_piece_object(
            position_row_column, piece_type, piece_color
        )
        return piece_object

    def get_all_same_color_piece_positions(self, piece_color):
        piece_color_mask = (
            (self.board_matrix <= 7) * (self.board_matrix > 0)
            if piece_color == "white"
            else self.board_matrix > 7
        )
        return np.argwhere(piece_color_mask)

    def get_piece_positions(self, piece_type, piece_color):
        piece_id = piece_type_to_id(piece_type, piece_color)
        return np.argwhere(self.board_matrix == piece_id)

    def move(self, source_row_column: list, destination_row_column: list):
        self.board_matrix[
            tuple(destination_row_column)
        ] = self.get_piece_id_from_position(source_row_column)
        self.board_matrix[tuple(source_row_column)] = 0

    def pawn_promotion(self, position_row_column: list, promote_to, color):
        self.board_matrix[tuple(position_row_column)] = piece_type_to_id(
            promote_to, color
        )

    def castle(self, side, color):
        if side == "kingside":
            if color == "white":
                self.move((0, 4), (0, 6))
                self.move((0, 7), (0, 5))
                self.can_white_castle_kingside = False
                self.can_white_castle_queenside = False
            else:
                self.move((7, 4), (7, 6))
                self.move((7, 7), (7, 5))
                self.can_black_castle_kingside = False
                self.can_black_castle_queenside = False
        else:
            if color == "white":
                self.move((0, 4), (0, 2))
                self.move((0, 0), (0, 3))
                self.can_white_castle_kingside = False
                self.can_white_castle_queenside = False
            else:
                self.move((7, 4), (7, 2))
                self.move((7, 0), (7, 3))
                self.can_black_castle_kingside = False
                self.can_black_castle_queenside = False

    # def get_fen(self):
    #     white_pov_board_matrix = np.flipud(self.board_matrix)
    #     board_matrix_list = white_pov_board_matrix.tolist()

    def move_single_file_rank_input(self, old_new_file_rank: str):
        (
            source_row_column,
            destination_row_column,
        ) = split_single_file_rank_to_old_new_row_column(old_new_file_rank)
        self.move(source_row_column, destination_row_column)


gg = Board()
gg.set_board_to_initial_configuration()
# gg.show()
# print(gg.get_fen())
# print(" ")
# gg.move_single_file_rank_input("d2d4")
# gg.show()
# # print(gg.is_ambiguous(an.algebraic_notation("4.Qb2")))
# print(gg.get_all_same_color_piece_positions("black"))
print(f"{gg.rooks_white:064b}")
print(f"{gg.white_pieces:064b}")
print(f"{gg.black_pieces:064b}")
print(f"{gg.rooks_white:064b}")
bit_board_str = bin(gg.black_pieces | gg.white_pieces)
print(bit_board_str)
b = ""
for i, char in enumerate(bit_board_str):
    line = char if (i - 1) % 8 != 0 else char + "\n"
    b += line

print(b)
