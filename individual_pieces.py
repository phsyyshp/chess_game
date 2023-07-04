from utilities import *
import itertools


class Pieces:
    def __init__(self, position_row_column, color, piece_type):
        self.position_row_column = position_row_column
        self.color = color
        self.type = piece_type

    def is_slider(self):
        return self.type in ["rook", "queen", "bishop"]

    def is_in_range(self, destination_row_column):
        return destination_row_column in self.moves_in_range()


class pawn(Pieces):
    def __init__(self, position_row_column, color):
        super().__init__(position_row_column, color, "pawn")

    def is_in_starting_position(self):
        if self.color == "white":
            return self.position_row_column[0] == 1
        else:
            return self.position_row_column[0] == 6

    def show_forward_squares(self, amount, direction):
        color_to_sign = {"white": 1, "black": -1}
        return (
            self.position_row_column[0] + color_to_sign[self.color] * amount,
            self.position_row_column[1] + direction * amount,
        )

    def is_capture_attempt(self, destination_row_column):
        return does_file_change(self.position_row_column, destination_row_column)

    def moves_in_range(self):
        amounts = [1, 2, 1, 1]
        directions = [0, 0, -1, 1]
        if not self.is_in_starting_position():
            amounts.pop(1)
            directions.pop(1)
        squares_in_range = [
            self.show_forward_squares(amount, direction)
            for amount, direction in zip(amounts, directions)
            if is_coordinate_in_board(self.show_forward_squares(amount, direction))
        ]
        return squares_in_range


class knight(Pieces):
    def __init__(self, position_row_column, color):
        super().__init__(position_row_column, color, "knight")

    def moves_in_range(self):
        return L_shaped_squares_mask(self.position_row_column)


class bishop(Pieces):
    def __init__(self, position_row_column, color):
        super().__init__(position_row_column, color, "bishop")

    def moves_in_range(self):
        return diagonal_squares_mask(
            self.position_row_column, amount=8, slope=1
        ) + diagonal_squares_mask(self.position_row_column, amount=8, slope=-1)

    def get_path(self, destination_row_column):
        return get_diagonal_path_mask(self.position_row_column, destination_row_column)


class rook(Pieces):
    def __init__(self, position_row_column, color):
        super().__init__(position_row_column, color, "rook")

    def moves_in_range(self):
        return horizontal_squares_mask(
            self.position_row_column
        ) + vertical_squares_mask(self.position_row_column)

    def get_path(self, destination_row_column):
        return get_straight_path_mask(self.position_row_column, destination_row_column)


class queen(Pieces):
    def __init__(self, position_row_column, color):
        super().__init__(position_row_column, color, "queen")

    def moves_in_range(self):
        return (
            horizontal_squares_mask(self.position_row_column)
            + vertical_squares_mask(self.position_row_column)
            + diagonal_squares_mask(self.position_row_column, amount=8, slope=1)
            + diagonal_squares_mask(self.position_row_column, amount=8, slope=-1)
        )


class king(Pieces):
    def __init__(self, position_row_column, color):
        super().__init__(position_row_column, color, "king")

    def moves_in_range(self):
        return (
            horizontal_squares_mask(self.position_row_column, amount=1)
            + vertical_squares_mask(self.position_row_column, amount=1)
            + diagonal_squares_mask(self.position_row_column, amount=1, slope=1)
            + diagonal_squares_mask(self.position_row_column, amount=1, slope=-1)
        )

    def get_castling_type(self, destination_row_column):
        match self.color:
            case "black":
                increment_sign = 1
            case "white":
                increment_sign = -1
        castling_type = (
            "short"
            if increment_horizontal(self.position_row_column, destination_row_column)
            == increment_sign
            else "long"
        )


pp = pawn("a7", "white")
print(pp.color)

print(pp.is_slider())
