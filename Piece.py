from utilities import *


class Piece:
    def __init__(self, position_row_column: list, color: str, piece_type: str):
        self.position_row_column = position_row_column
        self.color = color
        self.type = piece_type

    def is_slider(self):
        return self.type in ["rook", "queen", "bishop"]

    def is_in_range(self, destination_row_column):
        # print(self.squares_in_range_mask())
        squares_in_range_mask = self.squares_in_range_mask()

        print(destination_row_column)
        print(
            squares_in_range_mask[destination_row_column[0]][destination_row_column[1]]
        )
        return 1 == self.squares_in_range_mask()[tuple(destination_row_column)]


class Pawn(Piece):
    def __init__(self, position_row_column: list, color):
        super().__init__(position_row_column, color, "pawn")

    def is_in_starting_position(self):
        if self.color == "white":
            return self.position_row_column[0] == 1
        else:
            return self.position_row_column[0] == 6

    def forward_squares(self, amount, direction) -> list:
        color_to_sign = {"white": 1, "black": -1}
        return [
            self.position_row_column[0] + color_to_sign[self.color] * amount,
            self.position_row_column[1] + direction * amount,
        ]

    def is_capture_attempt(self, destination_row_column: list) -> bool:
        return does_file_change(self.position_row_column, destination_row_column)

    def squares_in_range_mask(self) -> np.ndarray:
        amounts = [1, 2, 1, 1]
        directions = [0, 0, -1, 1]
        if not self.is_in_starting_position():
            amounts.pop(1)
            directions.pop(1)

        squares_in_range_mask = [
            self.forward_squares(amount, direction)
            for amount, direction in zip(amounts, directions)
            if is_coordinate_in_board(self.forward_squares(amount, direction))
        ]
        binary_mat = np.zeros((8, 8))
        for row_col in np.array(squares_in_range_mask):
            binary_mat[tuple(row_col)] = 1
        return binary_mat


class Knight(Piece):
    def __init__(self, position_row_column: list, color):
        super().__init__(position_row_column, color, "knight")

    def squares_in_range_mask(self):
        return L_shaped_squares_mask(self.position_row_column)


class Bishop(Piece):
    def __init__(self, position_row_column: list, color):
        super().__init__(position_row_column, color, "bishop")

    def squares_in_range_mask(self):
        return diagonal_squares_mask(
            self.position_row_column, amount=8, slope=1
        ) + diagonal_squares_mask(self.position_row_column, amount=8, slope=-1)

    def get_path_mask(self, destination_row_column: list):
        return get_diagonal_path_mask(self.position_row_column, destination_row_column)


class Rook(Piece):
    def __init__(self, position_row_column: list, color):
        super().__init__(position_row_column, color, "rook")

    def squares_in_range_mask(self):
        return horizontal_squares_mask(
            self.position_row_column
        ) + vertical_squares_mask(self.position_row_column)

    def get_path_mask(self, destination_row_column: list):
        return get_straight_path_mask(self.position_row_column, destination_row_column)


class Queen(Piece):
    def __init__(self, position_row_column: list, color):
        super().__init__(position_row_column, color, "queen")

    def squares_in_range_mask(self):
        return (
            horizontal_squares_mask(self.position_row_column)
            + vertical_squares_mask(self.position_row_column)
            + diagonal_squares_mask(self.position_row_column, amount=8, slope=1)
            + diagonal_squares_mask(self.position_row_column, amount=8, slope=-1)
        )

    def get_path_mask(self, destination_row_column: list):
        return get_diagonal_path_mask(
            self.position_row_column, destination_row_column
        ) + get_straight_path_mask(self.position_row_column, destination_row_column)


class King(Piece):
    def __init__(self, position_row_column: list, color):
        super().__init__(position_row_column, color, "king")

    def squares_in_range_mask(self):
        return (
            horizontal_squares_mask(self.position_row_column, amount=1)
            + vertical_squares_mask(self.position_row_column, amount=1)
            + diagonal_squares_mask(self.position_row_column, amount=1, slope=1)
            + diagonal_squares_mask(self.position_row_column, amount=1, slope=-1)
        )

    def get_castling_type(self, destination_row_column: list):
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
        return castling_type


class Empty(Piece):
    def __init__(self, position_row_column: list, color):
        super().__init__(position_row_column, color, "empty")


# pp = Pawn([1, 2], "white")
# rr = Rook([3, 2], "white")
# # print(pp.color)

# # print(pp.is_slider())
# print(pp.squares_in_range_mask())
# # print(rr.squares_in_range_mask())
