import numpy as np
import piece_types as pct
from utilities import  *


[
    WHITE_PAWN_PIECE_ID,
    WHITE_KNIGHT_PIECE_ID,
    WHITE_BISHOP_PIECE_ID,
    WHITE_ROOK_PIECE_ID,
    WHITE_QUEEN_PIECE_ID,
    WHITE_KING_PIECE_ID,
    EMPTY_SQUARE_ID,
] = range(7, -1, -1)

BLACK_PIECE_ID_OFFSET = 6


class board:
    def __init__(self):
        self.board_matrix = np.full((8, 8), EMPTY_SQUARE_ID)

    def set_board_to_initial_configuration(self):
        self.board_matrix[1][:] = WHITE_PAWN_PIECE_ID
        self.board_matrix[0][:] = np.array(
            [
                WHITE_ROOK_PIECE_ID,
                WHITE_KNIGHT_PIECE_ID,
                WHITE_BISHOP_PIECE_ID,
                WHITE_QUEEN_PIECE_ID,
                WHITE_KING_PIECE_ID,
                WHITE_BISHOP_PIECE_ID,
                WHITE_KNIGHT_PIECE_ID,
                WHITE_ROOK_PIECE_ID,
            ]
        )
        self.board_matrix[6][:] = BLACK_PIECE_ID_OFFSET + self.board_matrix[1][:]
        self.board_matrix[7][:] = BLACK_PIECE_ID_OFFSET + self.board_matrix[0][:]

    def get_piece_id_from_position(self, position_row_column):
        return self.board_matrix[tuple(position_row_column)]

    def is_square_empty(self, position_row_column):
        return self.get_piece_id_from_position(position_row_column) == 0

    def get_piece_object_from_position(self, position_row_column):
        piece_id = self.get_piece_id_from_position(position_row_column)
        piece_object = pct.piece_types(piece_id).create_piece(position_row_column)
        return piece_object

    def is_destination_occupied_by_same_color(
        self, piece_object, destination_row_column
    ):
        if self.is_square_empty(destination_row_column):
            return False
        piece_object_at_new_position = self.get_piece_object_from_position(
            destination_row_column
        )
        is_same_color = piece_object.color() == piece_object_at_new_position.color()
        return is_same_color

    def is_pawn_path_clear(self, piece_object, destination_row_column):
        if not self.is_square_empty(destination_row_column):
            return False
        if piece_object.is_capture_attempt(destination_row_column):
            return not self.is_destination_occupied_by_same_color(
                piece_object, destination_row_column
            )
        else:
            path = piece_object.get_path(destination_row_column)
            is_path_clear = 0 == sum(self.board_matrix * path)
            return is_path_clear

    def is_path_clear(self, piece_object, destination_row_column):
        if self.is_destination_occupied_by_same_color(
            piece_object, destination_row_column
        ):
            return False
        piece_type = piece_object.type()
        if piece_type in ["king", "knight"]:
            return True
        if piece_object.is_slider():
            path = piece_object.get_path(destination_row_column)
            is_path_clear = 0 == sum(self.board_matrix * path)
            return is_path_clear
        elif piece_object.type() == "pawn":
            return self.is_pawn_path_clear(piece_object, destination_row_column)
        else:
            return True

    # do it like is_attacked_by_slider etc

    def is_under_attack_by_specific_piece(
        self, position_row_column, attacker_piece_type, attacker_color
    ):
        attacker_piece_id = piece_type_to_piece_id(attacker_piece_type)
        positions_of_attacker_pieces_bin_mat = self.get_positions_of_piece_type(
            attacker_piece_id
        )
        attack_bool = [
            self.is_attack_possible(position_row_column, position_of_attacker_piece)
            for position_of_attacker_piece in positions_of_attacker_pieces
        ]

    def is_under_attack_by_any_piece(self, position_row_column, attacker_color):
        is_attacked_by_specific_piece_bool_list = [
            self.is_under_attack_by_specific_piece(
                position_row_column, attacker_piece_type, attacker_color
            )
            for attacker_piece_type in [
                "pawn",
                "queen",
                "king",
                "bishop",
                "rook",
                "knight",
            ]
        ]
        return any(is_attacked_by_specific_piece_bool_list)

    def is_castling_attempt(self, piece_object, destination_row_column):
        if piece_object.type() == "king":
            return (
                abs(piece_object.position_row_column[1] - destination_row_column[0])
            ) == 2 and not (
                does_rank_change(
                    piece_object.position_row_column, destination_row_column
                )
            )

    def is_castling_legal(self, piece_object, destination_row_column):
        castling_type = piece_object.get_castling_type(destination_row_column)
        match castling_type:
            case "queen side":
                return self.is_queen_side_castling_legal(piece_object)
            case "king side":
                return self.is_king_side_castling_legal(piece_object)

    # show castling by king movement

    def is_move_legal(self, source_row_column, destination_row_column):
        piece_object = self.get_piece_object_from_position(source_row_column)
        if self.is_square_empty(source_row_column):
            return False
        if self.is_castling_attempt(piece_object, destination_row_column):
            return self.is_castling_legal(piece_object, destination_row_column)
        if not self.is_in_range(piece_object, destination_row_column):
            return False
        if not self.is_safe(source_row_column, destination_row_column):
            return False

        return self.is_path_clear(piece_object, destination_row_column)

    def is_pawn_promotion_legal(self, source_row_column, destination_row_column):
        pass

    def move_general_input(self, movement_str):
        "movement_str is in format of file"
        match self.type_of_movement(movement_str):
            case "castle":
                self.castle(movement_str)
            case "pawn_promotion":
                self.pawn_promotion(movement_str)
            case "regular":
                (
                    source_row_column,
                    destination_row_column,
                ) = self.movement_str_to_old_destination_row_column(movement_str)
                self.move(source_row_column, destination_row_column)

    def move(self, source_row_column, destination_row_column):
        self.board_matrix[
            tuple(destination_row_column)
        ] = self.get_piece_id_from_position(source_row_column)
        self.board_matrix[tuple(source_row_column)] = 0

    def move_single_file_rank_input(self, old_new_file_rank):
        (
            source_row_column,
            destination_row_column,
        ) = split_single_file_rank_to_old_new_row_column(old_new_file_rank)
        self.move(source_row_column, destination_row_column)

    def show(self, point_of_view="white"):
        vectorized_chr = np.vectorize(chr)

        visual_board = self.board_matrix + 9810 * (self.board_matrix != 0)
        visual_board = (
            np.flipud(visual_board)
            if point_of_view == "white"
            else np.fliplr(visual_board)
        )
        visual_board = vectorized_chr(32 * (visual_board == 0) + visual_board).tolist()
        visual_board = [
            ["\x1b[26;36;40m" + str(8 - i) + " "]
            + [
                "\x1b[26;30;46m " + visual_board[i][j] + " \x1b[0m"
                if (i + j + 1) % 2 == 0
                else "\x1b[26;30;47m " + visual_board[i][j] + " \x1b[0m"
                for j in range(8)
            ]
            for i in range(8)
        ]
        visual_board_string = "\n".join(["".join(item) for item in visual_board])
        file_row = (
            "  ".join("bcdefgh") if point_of_view == "white" else "  ".join("gfedcba")
        )
        file_row = (
            "  a  " + file_row if point_of_view == "white" else "\n   h   " + file_row
        )
        visual_board_string += "\n\x1b[26;36;40m " + file_row + " \x1b[0m"
        print(visual_board_string)


gg = board()
gg.set_board_to_initial_configuration()
gg.show()
print(" ")
gg.move_single_file_rank_input("d2d4")
gg.show()
# print(gg.is_ambiguous(an.algebraic_notation("4.Qb2")))
