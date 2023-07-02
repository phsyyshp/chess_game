from utilities import *
import board as bd


class algebraic_notation:
    def __init__(self, algebraic_notation):
        self.algebraic_notation = algebraic_notation

    def is_algebraic_notation(self):
        pass

    def color(self):
        color = "black" if self.algebraic_notation[2] == "." else "white"
        # print(self.algebraic_notation[2])

        return color

    def is_castling(self):
        pass

    def is_disambigious_move(self):
        pass

    def is_draw_ofr(self):
        pass

    def is_pawn_promotion(self):
        pass

    def possible_departue_squares(self):
        pass

    def piece_id(self):
        color_code = 0 if self.color() == "white" else 6
        # print(color_code)
        num_to_let = {"N": 6, "R": 4, "K": 2, "Q": 3, "B": 5}
        return num_to_let[self.algebraic_notation[-3]] + color_code

    def destination_cord(self):
        return self.algebraic_notation[-2] + self.algebraic_notation[-1]

    def departue_coor(self):
        pass

    def num_coor(self):
        return self.departue_coor(), self.dest_coor()

    def possible_departure_squares(self):
        """it multiplies the binary mask of the movement of the piece by the board matrix and returns the indices of the pieces that can move to the destination square"""
        piece_id = algebraic_notation.piece_id()
        new_position_row_column = algebraic_notation.destination_cord()
        dummy_piece = pct.piece_types(piece_id).create_piece(new_position_row_column)
        movement_mask = dummy_piece.moves_in_range()
        masked_board = bd.board_matrix * movement_mask
        indices_of_pieces = np.where(masked_board == piece_id)
        for index in indices_of_pieces:
            if not bd.is_legal_move(tuple(index), new_position_row_column):
                indices_of_pieces.remove(index)
        return indices_of_pieces

    def is_ambiguous(self):
        if self.is_special_move():
            return False
        else:
            return len(self.possible_departure_squares(algebraic_notation)) == 1


a = algebraic_notation("4.....Bb2")
# print(a.color())
