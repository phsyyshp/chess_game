from utilities.Constants import *
import numpy as np
from utilities.PieceIdentifier import *


def FEN_to_board_matrix(FEN: str) -> np.ndarray:
    # refactor it
    FEN = FEN.partition(" ")
    FEN = FEN[0]
    # rows = FEN.split("/")
    FEN_number_to_id_lambda = lambda x: int(x) * "e" if x.isdigit() else x
    FEN_row_to_piece_id_list = lambda x: list(map(fen_piece_letter_to_piece_id, x))
    normalized_FEN = "".join(list(map(FEN_number_to_id_lambda, FEN)))
    print(list(map(FEN_row_to_piece_id_list, normalized_FEN.split("/"))))
    board_matrix = np.array(
        list(map(FEN_row_to_piece_id_list, normalized_FEN.split("/")))
    )
    board_matrix = np.flipud(board_matrix)
    return board_matrix
