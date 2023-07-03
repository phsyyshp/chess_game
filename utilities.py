import numpy as np

import itertools


def file_rank_to_row_column(position_file_rank):
    return (int(position_file_rank[1]) - 1, ord(position_file_rank[0]) - 97)


def row_column_to_file_rank(position_row_column):
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


def is_coordinate_in_board(position_row_column):
    if position_row_column[0] in range(8) and position_row_column[1] in range(8):
        return True
    else:
        return False


def does_file_change(source_row_column, destination_row_column):
    return source_row_column[1] != destination_row_column[1]


def does_rank_change(source_row_column, destination_row_column):
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
    numpy_array_of_row_indices,
    numpy_array_of_column_indices,
    value_to_fill=1,
) -> np.ndarray:
    binary_mat[
        numpy_array_of_row_indices, numpy_array_of_column_indices
    ] = value_to_fill
    return binary_mat


def increment_horizontal(source_row_column, destination_row_column):
    "returns +1 if new_pos_col>old_pos_col"
    return np.sign((destination_row_column[1] - source_row_column[1]))


def increment_vertical(source_row_column, destination_row_column):
    "returns +1 if new_pos_row>old_pos_row"
    return np.sign((destination_row_column[0] - source_row_column[0]))


def get_row_indices_of_shortest_path(
    source_row_column, destination_row_column
) -> np.ndarray:
    return np.arange(
        source_row_column[0],
        destination_row_column[0],
        increment_vertical(source_row_column, destination_row_column),
    )


def get_column_indices_of_shortest_path(
    source_row_column, destination_row_column
) -> np.ndarray:
    return np.arange(
        source_row_column[1],
        destination_row_column[1],
        increment_horizontal(source_row_column, destination_row_column),
    )


def get_diagonal_path_mask(source_row_column, destination_row_column) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    numpy_array_of_row_indices = get_row_indices_of_shortest_path(
        source_row_column, destination_row_column
    )
    numpy_array_of_column_indices = get_column_indices_of_shortest_path(
        source_row_column, destination_row_column
    )
    binary_mat = fill_indices(
        binary_mat, numpy_array_of_row_indices, numpy_array_of_column_indices
    )
    binary_mat[tuple(destination_row_column)] = 0
    binary_mat[tuple(source_row_column)] = 0
    return binary_mat


def diagonal_squares_mask(position_row_column, amount=8, slope=-1) -> np.ndarray:
    "y = slope*x + offset"

    offset = position_row_column[0] - slope * position_row_column[1]

    numpy_array_of_row_indices = (
        slope
        * np.arange(
            -amount + position_row_column[1], amount + position_row_column[1] + 1
        )
        + offset
    )

    numpy_array_of_column_indices = np.arange(
        -amount + position_row_column[1], amount + position_row_column[1] + 1
    )

    is_in_board_mask = (
        (0 <= numpy_array_of_row_indices)
        and (numpy_array_of_row_indices < 8)
        and (0 <= numpy_array_of_column_indices)
        and (numpy_array_of_column_indices < 8)
    )

    numpy_array_of_column_indices = numpy_array_of_column_indices[is_in_board_mask]
    numpy_array_of_row_indices = numpy_array_of_row_indices[is_in_board_mask]

    binary_mat = np.zeros((8, 8), dtype=float)
    binary_mat = fill_indices(
        binary_mat, numpy_array_of_row_indices, numpy_array_of_column_indices
    )
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def horizontal_squares_mask(position_row_column, amount=8) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    lower_bound, upper_bound = find_bounds(position_row_column[1], amount)
    binary_mat[position_row_column[0]][lower_bound : upper_bound + 1] = 1
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def vertical_squares_mask(position_row_column, amount=8) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    lower_bound, upper_bound = find_bounds(position_row_column[0], amount)
    binary_mat[lower_bound : upper_bound + 1][position_row_column[1]] = 1
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def get_straight_path_mask(source_row_column, destination_row_column) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    if does_rank_change(source_row_column, destination_row_column):
        numpy_array_of_row_indices = get_row_indices_of_shortest_path(
            source_row_column, destination_row_column
        )
        numpy_array_of_column_indices = source_row_column[1] * np.ones(
            numpy_array_of_row_indices.size, dtype="int64"
        )
    elif does_file_change(source_row_column, destination_row_column):
        numpy_array_of_column_indices = get_column_indices_of_shortest_path(
            source_row_column, destination_row_column
        )
        numpy_array_of_row_indices = np.ones(
            numpy_array_of_column_indices.size, dtype="int64"
        )
    else:
        pass
        # implement error
    binary_mat = fill_indices(
        binary_mat, numpy_array_of_row_indices, numpy_array_of_column_indices, 1
    )
    binary_mat[tuple(destination_row_column)] = 0
    binary_mat[tuple(source_row_column)] = 0
    return binary_mat


def L_shaped_squares_mask(position_row_column) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=float)
    for row_jump, column_jump in itertools.product([-1, 1], [-2, 2]):
        if is_coordinate_in_board(
            (position_row_column[0] + row_jump, position_row_column[1] + column_jump)
        ):
            binary_mat[position_row_column[0] + row_jump][
                position_row_column[1] + column_jump
            ] = 1
    return binary_mat


def piece_id_to_color(piece_id):
    color = "white" if piece_id <= 7 else "black"
    return color


# def piece_type_to_id(piece_type, color):

# def is_alg_not(input):
# pass


# print(horizontal_squares(file_rank_to_row_column("d4"), 1))
# print(diagonal_squares(file_rank_to_row_column("d7"), 1, 1))
# print(L_shaped_squares(file_rank_to_row_column("d8")))
# print(get_diagonal_path_mask([2, 3], [5, 6]))
# print(get_straight_path_mask([2, 3], [5, 3]))
