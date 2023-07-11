import Board as bd
import numpy as np
import Move as mv
import Attack as at
from utilities import *


class Game:
    def __init__(self):
        self.board = bd.Board()
        self.board_history = [self.board.board_matrix]
        self.is_draw = False

    def is_input_valid(self, user_input):
        (
            source_row_column,
            destination_row_column,
        ) = split_single_file_rank_to_old_new_row_column(user_input)
        move = mv.Move(source_row_column, destination_row_column, self.board)
        return move.is_legal()

    def is_game_over(self):
        return (
            at.Attack(self.board).is_check_mate()
            or at.Attack(self.board).is_stale_mate()
            or self.is_draw
        )

    def get_move_form_user(self):
        is_input_valid = False
        while not is_input_valid:
            user_input = input("movement:")
            (
                source_row_column,
                destination_row_column,
            ) = split_single_file_rank_to_old_new_row_column(user_input)
            move = mv.Move(source_row_column, destination_row_column, self.board)
            is_input_valid = move.is_legal()
        return move

    def update_board(self, user_input):
        self.board.move_single_file_rank_input(user_input)
        self.board_history.append(self.board.board_matrix)

    def play_game(self):
        self.board.set_board_to_initial_configuration()
        while not self.is_game_over():
            self.board.show()
            move = self.get_move_form_user()
            self.update_board(move)
            self.board.show()
