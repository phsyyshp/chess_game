import numpy as np
from utilities.BoardUtilities import *


def get_diagonal_path_mask(
    source_row_column: list, destination_row_column: list
) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=int)
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
    column_indices =  np.arange(
        -amount + position_row_column[1], amount + position_row_column[1] + 1
    )
    row_indices = slope * column_indices + offset
    row_indices, column_indices = choose_in_board_indices(row_indices, column_indices)

    binary_mat = np.zeros((8, 8), dtype=int)
    binary_mat = fill_indices(binary_mat, row_indices, column_indices)
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def horizontal_squares_mask(position_row_column: list, amount=8) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=int)
    lower_bound, upper_bound = find_bounds(position_row_column[1], amount)
    binary_mat[position_row_column[0]][lower_bound : upper_bound + 1] = 1
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def vertical_squares_mask(position_row_column: list, amount=8) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=int)
    lower_bound, upper_bound = find_bounds(position_row_column[0], amount)
    binary_mat[lower_bound : upper_bound + 1, position_row_column[1]] = 1
    binary_mat[tuple(position_row_column)] = 0
    return binary_mat


def get_straight_path_mask(
    source_row_column: list, destination_row_column: list
) -> np.ndarray:
    binary_mat = np.zeros((8, 8), dtype=int)
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
    binary_mat = np.zeros((8, 8), dtype=int)
    # indices of possible knight jumps.
    row_jump = np.array(2 * [-1, 1] + 2 * [-2, 2])
    column_jump = np.array([-2, -2] + [2, 2] + [-1, -1] + [1, 1])

    row_indices = row_jump + position_row_column[0]
    column_indices = column_jump + position_row_column[1]
    row_indices, column_indices = choose_in_board_indices(row_indices, column_indices)
    binary_mat[row_indices, column_indices] = 1
    return binary_mat
