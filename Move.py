from utilities import *
from Constants import *
import numpy as np
import Board as bd
import Piece as pc


class Move:
    def __init__(
        self, source_row_column: list, destination_row_column: list, board: bd.Board
    ):
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
        # TODO think about what to add more.

    def is_pawn_path_clear(self):
        if self.piece_object_to_move.is_capture_attempt(self.destination_row_column):
            return not self.is_destination_occupied_by_same_color()
        if not self.board.is_square_empty(self.destination_row_column):
            return False

    def is_path_clear(self):
        if self.is_destination_occupied_by_same_color():
            return False
        piece_type = self.piece_object_to_move.type()
        if piece_type in ["king", "knight"]:
            return True
        if self.piece_object_to_move.is_slider():
            path = self.piece_object_to_move.get_path_mask(self.destination_row_column)
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
        attacker_positions_indices = self.board.get_piece_positions(
            attacker_piece_type, attacker_color
        )
        attacker_piece_id = piece_type_to_id(attacker_piece_type, attacker_color)
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

    def is_pawn_promotion(self):
        if self.piece_object_to_move.type() != "pawn":
            return False
        if self.piece_object_to_move.color() == "white":
            return self.destination_row_column[0] == 0
        else:
            return self.destination_row_column[0] == 7

    def is_castling_attempt(self):
        if self.piece_object_to_move.type() != "king":
            return False
        if does_rank_change(
            self.piece_object_to_move.position_row_column, self.destination_row_column
        ):
            return False
        return (
            abs(
                self.piece_object_to_move.position_row_column[1]
                - self.destination_row_column[0]
            )
            == 2
        )

    def is_queen_side_castling_legal(self):
        color = self.piece_object_to_move.color()
        if self.is_check():
            return False
        is_castling_pieces_moved = (
            self.board.can_black_castle_queenside
            if color == "black"
            else self.board.can_white_castle_queenside
        )
        if is_castling_pieces_moved:
            return False
        if any(
            self.is_under_attack_by_any_piece(
                (self.piece_object_to_move.position_row_column[0], 2), color
            )
        ):
            return False
        return True

    def is_king_side_castling_legal(self):
        color = self.piece_object_to_move.color()
        if self.is_check():
            return False
        is_castling_pieces_moved = (
            self.board.can_black_castle_kingside
            if color == "black"
            else self.board.can_white_castle_kingside
        )
        if is_castling_pieces_moved:
            return False
        if any(
            self.is_under_attack_by_any_piece(
                (self.piece_object_to_move.position_row_column[0], 6), color
            )
        ):
            return False
        return True

    def is_castling_legal(self):
        castling_type = self.piece_object_to_move.get_castling_type(
            self.destination_row_column
        )
        match castling_type:
            case "queen side":
                return self.is_queen_side_castling_legal()
            case "king side":
                return self.is_king_side_castling_legal()

    # show castling by king movement
    def is_check(self):
        anti_color = {"white": "black", "black": "white"}
        king_position_row_column = self.board.get_piece_positions(
            "king", self.piece_object_to_move.color
        )
        return self.is_under_attack_by_any_piece(
            king_position_row_column, anti_color[self.piece_object_to_move.color]
        )

    def is_safe(self):
        if self.is_check(self.board.board_matrix):
            return False
        # add second condition

    def get_legal_moves(self):
        positions_of_pieces = self.board.get_all_same_color_piece_positions(
            self.piece_object_to_move.color
        )

    def can_king_be_saved(self):
        all_possible_moves = self.get_legal_moves()
        pass

    def is_check_mate(self):
        if not self.is_check():
            return False
        return self.can_king_be_saved()

    def is_move_legal(self):
        if self.type == "empty":
            return False
        if self.type == "castling":
            return self.is_castling_legal()
        if not self.piece_object_to_move.is_in_range(self.destination_row_column):
            return False
        if not self.is_safe():
            return False

        return self.is_path_clear()

    def is_pawn_promotion_legal(self):
        pass


gg = bd.Board()
gg.set_board_to_initial_configuration()
gg.show()
Move([4, 0], [5, 0], gg)
