import Board as bd
from utilities import *


import numpy as np
from Constants import *


class Visual:
    def __init__(self, orientation: str, board: bd.Board, color_of_board="cyan"):
        self.board = board
        self.orientation = orientation
        self.visual_board = []
        self.color_of_board = color_of_board

    def set_orientation(self):
        self.visual_board = (
            np.flipud(self.visual_board)
            if self.orientation == "white"
            else np.fliplr(self.visual_board)
        )

    def put_piece_icons(self):
        vectorized_chr = np.vectorize(chr)
        self.visual_board = self.board.board_matrix + UNICODE_OFFSET * (
            self.board.board_matrix != 0
        )
        self.visual_board = vectorized_chr(
            BLANK_UNICODE * (self.visual_board == 0) + self.visual_board
        ).tolist()
        self.set_orientation()

    def show(self):
        self.put_piece_icons()
        self.visual_board = [
            [colorize_string(str(8 - i), self.color_of_board, "black")]
            + [
                colorize_string(self.visual_board[i][j], "black", self.color_of_board)
                if (i + j + 1) % 2 == 0
                else colorize_string(self.visual_board[i][j], "black", "white")
                for j in range(8)
            ]
            for i in range(8)
        ]
        visual_board_string = "\n".join(["".join(item) for item in self.visual_board])
        file_row = (
            "  ".join("bcdefgh")
            if self.orientation == "white"
            else "  ".join("gfedcba")
        )
        file_row = (
            "  a  " + file_row
            if self.orientation == "white"
            else "\n   h   " + file_row
        )
        visual_board_string += "\n " + colorize_string(
            file_row, self.color_of_board, "black"
        )
        print(visual_board_string)
