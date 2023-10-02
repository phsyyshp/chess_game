import numpy as np
from utilities.Constants import *


def is_coordinate_in_board(position_row_column: list):
    return position_row_column[0] in range(8) and position_row_column[1] in range(8)


def choose_in_board_indices(row_indices: np.ndarray, column_indices: np.ndarray):
    is_in_board_mask = (
        (0 <= row_indices)
        * (row_indices < 8)
        * (0 <= column_indices)
        * (column_indices < 8)
    )
    column_indices = column_indices[is_in_board_mask]
    row_indices = row_indices[is_in_board_mask]
    return row_indices, column_indices


def does_file_change(source_row_column: list, destination_row_column: list):
    return source_row_column[1] != destination_row_column[1]


def does_rank_change(source_row_column: list, destination_row_column: list):
    return source_row_column[0] != destination_row_column[0]


def find_bounds(input_val, amount):
    if input_val - amount < 0:
        lower_bound = 0
    else:
        lower_bound = input_val - amount
    if amount + input_val > 7:
        upper_bound = 7
    else:
        upper_bound = amount + input_val
    return lower_bound, upper_bound


def fill_indices(
    binary_mat,
    row_indices: np.ndarray,
    column_indices: np.ndarray,
    value_to_fill=1,
) -> np.ndarray:
    binary_mat[row_indices, column_indices] = value_to_fill
    return binary_mat


def increment_horizontal(source_row_column: list, destination_row_column: list):
    "returns +1 if new_pos_col>old_pos_col"
    return np.sign((destination_row_column[1] - source_row_column[1]))


def increment_vertical(source_row_column: list, destination_row_column: list):
    "returns +1 if new_pos_row>old_pos_row"
    return np.sign((destination_row_column[0] - source_row_column[0]))


def get_row_indices_of_shortest_path(
    source_row_column: list, destination_row_column: list
) -> np.ndarray:
    return np.arange(
        source_row_column[0],
        destination_row_column[0],
        increment_vertical(source_row_column, destination_row_column),
    )


def get_column_indices_of_shortest_path(
    source_row_column: list,
    destination_row_column: list,
) -> np.ndarray:
    return np.arange(
        source_row_column[1],
        destination_row_column[1],
        increment_horizontal(source_row_column, destination_row_column),
    )


def mask_to_row_column(mask):
    return np.argwhere(mask == 1)


def change_turn(turn):
    if turn == "white":
        return "black"
    return "white"


def colorize_string(string, fg_color, bg_color):
    color_prefix = (
        "\x1b[26;"
        + FOREGROUND_COLOR_TO_ANSII[fg_color]
        + ";"
        + BACKGROUND_COLOR_TO_ANSII[bg_color]
        + "m"
    )
    colorized_string = color_prefix + " " + string + " " + "\x1b[0m"

    return colorized_string


def show_board_matrix(board_matrix, point_of_view="white"):
    vectorized_chr = np.vectorize(chr)

    visual_board = board_matrix + 9810 * (board_matrix != 0)
    visual_board = (
        np.flipud(visual_board) if point_of_view == "white" else np.fliplr(visual_board)
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


# def is_alg_not(input):
# pass


# print(horizontal_squares(file_rank_to_row_column("d4"), 1))
# print(diagonal_squares(file_rank_to_row_column("d7"), 1, 1))
# print(L_shaped_squares(file_rank_to_row_column("d8")))
# print(get_diagonal_path_mask([2, 3], [5, 6]))
# print(get_straight_path_mask([2, 3], [5, 3]))
# show_board_matrix(
# FEN_to_board_matrix(
# "r3k1nr/pp2pp1p/nq1p1bpP/2pP4/4P1B1/2P5/PP3PP1/RNBQK2R w KQkq - 1 12"
# )
# )
# print(vertical_squares_mask([3, 2], amount=8))
