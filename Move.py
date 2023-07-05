from utilities import *
from Constants import *
import numpy as np
import Board as bd
import Piece as pc


class Move:
    def __init__(self, source_row_column, destination_row_column, board: bd.Board):
        self.board = board
        self.piece_object_to_move = board.get_piece_object_from_position(
            source_row_column
        )
        self.destination_row_column = destination_row_column
        self.type = self.get_type()
        if self.type == "empty":
            raise ValueError("Can not move an empty square")

    def is_destination_occupied_by_same_color(self):
        if self.board.is_square_empty(self.destination_row_column):
            return False
        piece_object_at_destination = self.board.get_piece_object_from_position(
            self.destination_row_column
        )
        is_same_color = (
            self.piece_object_to_move.color() == piece_object_at_destination.color()
        )
        return is_same_color

    def get_type(self):
        if self.piece_object_to_move.type == "empty":
            return "empty"
        if self.is_castling_attempt():
            return "castling"
        if self.is_pawn_promotion():
            return "pawn_promotion"
        # think about what to add more.

    def is_pawn_path_clear(self):
        # fix bugs here
        if not self.board.is_square_empty(self.destination_row_column):
            return False
        if self.piece_object_to_move.is_capture_attempt(self.destination_row_column):
            return not self.is_destination_occupied_by_same_color()
        else:
            path = self.piece_object_to_move.get_path(self.destination_row_column)
            is_path_clear = not any(self.board.board_matrix * path)
            return is_path_clear

    def is_path_clear(self):
        if self.is_destination_occupied_by_same_color():
            return False
        piece_type = self.piece_object_to_move.type()
        if piece_type in ["king", "knight"]:
            return True
        if self.piece_object_to_move.is_slider():
            path = self.piece_object_to_move.get_path(self.destination_row_column)
            is_path_clear = 0 == sum(self.board.board_matrix * path)
            return is_path_clear
        if piece_type == "pawn":
            return self.is_pawn_path_clear()
        return True

    def is_threat(self, attacker_position_index, position_row_column, piece_id):
        piece_type, piece_color = piece_id_to_type_color(piece_id)
        dummy_piece_object = self.board.create_piece_object(
            attacker_position_index, piece_type, piece_color
        )
        if not dummy_piece_object.is_in_range(position_row_column):
            return False
        return self.is_path_clear(dummy_piece_object, position_row_column)

    # do it like is_attacked_by_slider etc

    def is_under_attack_by_specific_piece(
        self, position_row_column, attacker_piece_type, attacker_color
    ):
        attacker_piece_id = piece_type_to_id(attacker_piece_type, attacker_color)
        attacker_positions_indices = np.argwhere(
            self.board.board_matrix == attacker_piece_id
        )
        is_under_attack_boolean = [
            self.is_threat(
                attacker_position_index, position_row_column, attacker_piece_id
            )
            for attacker_position_index in attacker_positions_indices
        ]

        return any(is_under_attack_boolean)

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

    def is_castling_attempt(self, destination_row_column):
        if self.piece_object_to_move.type() != "king":
            return False
        if does_rank_change(
            self.piece_object_to_move.position_row_column, destination_row_column
        ):
            return False
        return (
            abs(
                self.piece_object_to_move.position_row_column[1]
                - destination_row_column[0]
            )
            == 2
        )

    def is_castling_legal(self, destination_row_column):
        castling_type = self.piece_object_to_move.get_castling_type(
            destination_row_column
        )
        match castling_type:
            case "queen side":
                return self.is_queen_side_castling_legal(self.piece_object_to_move)
            case "king side":
                return self.is_king_side_castling_legal(self.piece_object_to_move)

    # show castling by king movement
    def is_check(self, color):
        anti_color = {"white": "black", "black": "white"}
        king_position_row_column = self.board.get_piece_positions("king", color)
        return self.is_under_attack_by_any_piece(
            king_position_row_column, anti_color[color]
        )

    def is_safe(self, source_row_column, destination_row_column):
        if self.is_check(self.board.board_matrix):
            return False
        # add second condition

    def can_king_be_saved(self, color):
        pass

    def is_check_mate(self, color):
        if not self.is_check(color):
            return False
        return self.can_king_be_saved(color)

    def is_move_legal(self, source_row_column, destination_row_column):
        piece_object = self.board.get_piece_object_from_position(source_row_column)
        if self.board.is_square_empty(source_row_column):
            return False
        if self.is_castling_attempt(piece_object, destination_row_column):
            return self.is_castling_legal(piece_object, destination_row_column)
        if not piece_object.is_in_range(destination_row_column):
            return False
        if not self.is_safe(source_row_column, destination_row_column):
            return False

        return self.is_path_clear(piece_object, destination_row_column)

    def is_pawn_promotion_legal(self, source_row_column, destination_row_column):
        pass

    def castle(self, side, color):
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
                ) = split_single_file_rank_to_old_new_row_column(movement_str)
                self.board.move(source_row_column, destination_row_column)


gg = bd.Board()
gg.set_board_to_initial_configuration()
gg.show()
Move([4, 0], [5, 0], gg)
