import numpy as np
from Constants import *


def file_rank_to_row_column(position_file_rank):
    return (int(position_file_rank[1]) - 1, ord(position_file_rank[0]) - 97)


def row_column_to_file_rank(position_row_column: list):
    return chr(position_row_column[1] + 97) + str(position_row_column[0] + 1)


def combine_old_new_row_column_to_rank_file(
    source_row_column,
    destination_row_column,
):
    return row_column_to_file_rank(source_row_column) + row_column_to_file_rank(
        destination_row_column
    )


def split_single_file_rank_to_old_new_row_column(old_new_file_rank):
    old_position_file_rank = old_new_file_rank[0:2]
    new_position_file_rank = old_new_file_rank[2:4]
    return file_rank_to_row_column(old_position_file_rank), file_rank_to_row_column(
        new_position_file_rank
    )


def is_coordinate_in_board(position_row_column: list):
    if position_row_column[0] in range(8) and position_row_column[1] in range(8):
        return True
    else:
        return False


def choose_in_board_indices(row_indices: np.ndarray, column_indices: np.ndarray):
    is_in_board_mask = (
        (0 <= row_indices)
        and (row_indices < 8)
        and (0 <= column_indices)
        and (column_indices < 8)
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
    source_row_column: list, destination_row_column: list
) -> np.ndarray:
    return np.arange(
        source_row_column[1],
        destination_row_column[1],
        increment_horizontal(source_row_column, destination_row_column),
    )


def get_diagonal_path_mask(
    source_row_column: list, destination_row_column: list
) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    row_indices = get_row_indices_of_shortest_path(
        source_row_column, destination_row_column
    )
    column_indices = get_column_indices_of_shortest_path(
        source_row_column, destination_row_column
    )
    binary_mat = fill_indices(binary_mat, row_indices, column_indices)
    binary_mat[tuple(destination_row_column)] = 0
    binary_mat[tuple(source_row_column)] = 0
    return binary_mat


def diagonal_squares_mask(position_row_column: list, amount=8, slope=-1) -> np.ndarray:
    "y = slope*x + offset"
    offset = position_row_column[0] - slope * position_row_column[1]
    row_indices = (
        slope
        * np.arange(
            -amount + position_row_column[1], amount + position_row_column[1] + 1
        )
        + offset
    )

    column_indices = np.arange(
        -amount + position_row_column[1], amount + position_row_column[1] + 1
    )

    row_indices, column_indices = choose_in_board_indices(row_indices, column_indices)

    binary_mat = np.zeros((8, 8), dtype=float)
    binary_mat = fill_indices(binary_mat, row_indices, column_indices)
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def horizontal_squares_mask(position_row_column: list, amount=8) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    lower_bound, upper_bound = find_bounds(position_row_column[1], amount)
    binary_mat[position_row_column[0]][lower_bound : upper_bound + 1] = 1
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def vertical_squares_mask(position_row_column: list, amount=8) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    lower_bound, upper_bound = find_bounds(position_row_column[0], amount)
    binary_mat[lower_bound : upper_bound + 1, position_row_column[1]] = 1
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def get_straight_path_mask(
    source_row_column: list, destination_row_column: list
) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    if does_rank_change(source_row_column, destination_row_column):
        row_indices = get_row_indices_of_shortest_path(
            source_row_column, destination_row_column
        )
        column_indices = source_row_column[1] * np.ones(row_indices.size, dtype="int64")
    elif does_file_change(source_row_column, destination_row_column):
        column_indices = get_column_indices_of_shortest_path(
            source_row_column, destination_row_column
        )
        row_indices = np.ones(column_indices.size, dtype="int64")
    else:
        pass
        # implement error
    binary_mat = fill_indices(binary_mat, row_indices, column_indices, 1)
    binary_mat[tuple(destination_row_column)] = 0
    binary_mat[tuple(source_row_column)] = 0
    return binary_mat


def L_shaped_squares_mask(position_row_column: list) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    # indices of possible knight jumps.
    row_jump = np.array(2 * [-1, 1])
    column_jump = np.array([-2, -2] + [2, 2])

    row_indices = row_jump + position_row_column[0]
    column_indices = column_jump + position_row_column[1]
    row_indices, column_indices = choose_in_board_indices(row_indices, column_indices)
    binary_mat[row_indices, column_indices] = 1
    return binary_mat


def piece_id_to_color(piece_id):
    color = "white" if piece_id <= 7 else "black"
    return color


def piece_id_to_type_color(piece_id):
    color = "white" if piece_id <= 7 else "black"
    normalized_piece_id = piece_id - (color == "black") * BLACK_PIECE_ID_OFFSET
    if normalized_piece_id == WHITE_PAWN_PIECE_ID:
        return "pawn", color
    if normalized_piece_id == WHITE_KNIGHT_PIECE_ID:
        return "knight", color
    if normalized_piece_id == WHITE_ROOK_PIECE_ID:
        return "rook", color
    if normalized_piece_id == WHITE_QUEEN_PIECE_ID:
        return "queen", color
    if normalized_piece_id == WHITE_KING_PIECE_ID:
        return "king", color
    if normalized_piece_id == WHITE_BISHOP_PIECE_ID:
        return "bishop", color
    if normalized_piece_id == EMPTY_SQUARE_ID:
        return "empty", color


def piece_type_to_id(piece_type, color):
    piece_ids = {
        "pawn": WHITE_PAWN_PIECE_ID,
        "knight": WHITE_KNIGHT_PIECE_ID,
        "rook": WHITE_ROOK_PIECE_ID,
        "queen": WHITE_QUEEN_PIECE_ID,
        "king": WHITE_KING_PIECE_ID,
        "bishop": WHITE_BISHOP_PIECE_ID,
        "empty": EMPTY_SQUARE_ID,
    }

    offset = 0 if color == "white" else BLACK_PIECE_ID_OFFSET
    return piece_ids.get(piece_type, EMPTY_SQUARE_ID) + offset


def fen_piece_letter_to_piece_id(fen_piece_code):
    fen_piece_code_to_piece_type_dictionary = {
        "P": WHITE_PAWN_PIECE_ID,
        "N": WHITE_KNIGHT_PIECE_ID,
        "B": WHITE_BISHOP_PIECE_ID,
        "R": WHITE_ROOK_PIECE_ID,
        "Q": WHITE_QUEEN_PIECE_ID,
        "K": WHITE_KING_PIECE_ID,
        "p": WHITE_PAWN_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "n": WHITE_KNIGHT_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "b": WHITE_BISHOP_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "r": WHITE_ROOK_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "q": WHITE_QUEEN_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "k": WHITE_KING_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "e": EMPTY_SQUARE_ID,
    }
    return fen_piece_code_to_piece_type_dictionary[fen_piece_code]


def mask_to_row_column(mask):
    return np.argwhere(mask == 1)


def FEN_to_board_matrix(FEN: str) -> np.ndarray:
    # refactor it
    FEN = FEN.partition(" ")
    FEN = FEN[0]
    rows = FEN.split("/")
    FEN_number_to_id_lambda = lambda x: int(x) * "e" if x.isdigit() else x
    FEN_row_to_piece_id_list = lambda x: list(map(fen_piece_letter_to_piece_id, x))
    normalized_FEN = "".join(list(map(FEN_number_to_id_lambda, FEN)))
    print(list(map(FEN_row_to_piece_id_list, normalized_FEN.split("/"))))
    board_matrix = np.array(
        list(map(FEN_row_to_piece_id_list, normalized_FEN.split("/")))
    )
    board_matrix = np.flipud(board_matrix)
    return board_matrix


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
