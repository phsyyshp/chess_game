
from utilities.PieceIdentifier import *
from utilities.BoardUtilities import *
from utilities.BoardMaskGenerator import *
import Board as bd


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

        # if self.type == "empty":
        #     raise ValueError("Can not move an empty square")

    def is_destination_occupied_by_same_color(self):
        if self.board.is_square_empty(self.destination_row_column):
            return False
        piece_object_at_destination = self.board.get_piece_object_from_position(
            self.destination_row_column
        )
        is_same_color = (
            self.piece_object_to_move.color == piece_object_at_destination.color
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

    # def is_pawn_path_clear(self, piece_object_to_move=[], destination_row_column=[]):

    def is_pawn_path_clear(self):
        # TODO refactor it into more elegant form
        # if piece_object_to_move == []:
        #     piece_object_to_move = self.piece_object_to_move
        # if destination_row_column == []:
        #     destination_row_column = self.destination_row_column

        if self.piece_object_to_move.is_capture_attempt(self.destination_row_column):
            return (
                not self.board.is_square_empty(self.destination_row_column)
            ) and not self.is_destination_occupied_by_same_color()
        if not self.board.is_square_empty(self.destination_row_column):
            return False
        return True

    # def is_path_clear(self, piece_object_to_move=[], destination_row_column=[]):

    def is_path_clear(self):
        #     piece_object_to_move = self.piece_object_to_move
        # if destination_row_column == []:
        #     destination_row_column = self.destination_row_column

        if self.is_destination_occupied_by_same_color():
            return False
        piece_type = self.piece_object_to_move.type
        if piece_type in ["king", "knight"]:
            return True
        if self.piece_object_to_move.is_slider():
            path = self.piece_object_to_move.get_path_mask(self.destination_row_column)
            #TODO be sure this is correct
            is_path_clear = ( not  (self.board.board_matrix * path).any())
            return is_path_clear
        if piece_type == "pawn":
            return self.is_pawn_path_clear()
        return True

    # do it like is_attacked_by_slider etc

    def is_under_attack_by_slider(self, position_row_column, attacker_color):
        slider_mask = diagonal_squares_mask(position_row_column, -1) + diagonal_squares_mask(position_row_column, 1) + horizontal_squares_mask(position_row_column) + vertical_squares_mask(position_row_column)
        is_slider_in_range = sum(sum(self.board.board_matrix * slider_mask == piece_type_to_id("bishop", attacker_color)) + sum(self.board.board_matrix * slider_mask == piece_type_to_id("rook", attacker_color)) + sum(self.board.board_matrix * slider_mask == piece_type_to_id("queen", attacker_color))  )
        
        if not is_slider_in_range:
            return False
        slider_positions = mask_to_row_column(slider_mask)
        for position in slider_positions:
            if Move(position, position_row_column, self.board).is_path_clear():
                return True
        return False

    def is_under_attack_by_pawn(self, position_row_column, attacker_color):
        get_possible_pawn_attack_positions = diagonal_squares_mask(position_row_column, 1, 1) + diagonal_squares_mask(position_row_column, 1, -1)
        is_pawn_in_range = sum(sum(self.board.board_matrix * get_possible_pawn_attack_positions == piece_type_to_id("pawn", attacker_color)))
        if not is_pawn_in_range:
            return False
        pawn_positions = mask_to_row_column(get_possible_pawn_attack_positions)
        for position in pawn_positions:
            if Move(position, position_row_column, self.board).is_pawn_path_clear():
                return True
        return False
    def is_under_attack_by_specific_piece_type(
        self, position_row_column, attacker_piece_type, attacker_color
    ):
    #TODO wont work for pawn
        dummy_piece_object = self.board.create_piece_object(position_row_column, attacker_piece_type, attacker_color)
        move_ranges_mask = dummy_piece_object.squares_in_range_mask()
        return sum(sum(
        move_ranges_mask * self.board.board_matrix == piece_type_to_id(attacker_piece_type, attacker_color)))

    def is_under_attack_by_any_piece_type(self, position_row_column, attacker_color):
        if self.is_under_attack_by_specific_piece_type(
            position_row_column, "knight", attacker_color
        ):
            # print("nayt")
            return True
        if self.is_under_attack_by_slider(position_row_column,attacker_color):
            # print("slider")
            return True
        if self.is_under_attack_by_pawn(
            position_row_column,  attacker_color
        ):
            # print("pawn")
            return True
        if self.is_under_attack_by_specific_piece_type(
            position_row_column, "king", attacker_color
        ):
            return True
        return False

    def is_pawn_promotion(self):
        if self.piece_object_to_move.type != "pawn":
            return False
        if self.piece_object_to_move.color == "white":
            return self.destination_row_column[0] == 0
        else:
            return self.destination_row_column[0] == 7

    def is_castling_attempt(self):
        if self.piece_object_to_move.type != "king":
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
            self.is_under_attack_by_any_piece_type(
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
            self.is_under_attack_by_any_piece_type(
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

    def is_new_position_check(self):
        anti_color = {"white": "black", "black": "white"}

        king_position_row_column = self.board.get_piece_positions(
            "king", self.piece_object_to_move.color
        )
        # TODO fix [[1,2]] error now it si temporarly fixed by king_position_row_column[0]
        out = self.is_under_attack_by_any_piece_type(
            king_position_row_column[0], anti_color[self.piece_object_to_move.color]
        )
        return out

    def is_safe(self):
        # if self.is_new_position_check():
            # print("lala")
            # return False
        # TODO add second condition
        return True

    def is_legal(self):
        if self.type == "empty":
            return False
        if not self.piece_object_to_move.color == self.board.turn:
            return False
        if self.type == "castling":
            # TODO implement castling check
            return False
        if not self.is_safe():
            return False
        if not self.piece_object_to_move.is_in_range(self.destination_row_column):
            return False

        return self.is_path_clear()

    def is_pawn_promotion_legal(self):
        pass


# gg = bd.Board()
# gg.set_board_to_initial_configuration()
# gg.show()
# Move([4, 0], [5, 0], gg)
