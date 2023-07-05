import Board as bd
import numpy as np
from utilities import *


class Game:
    def __init__(self):
        self.board_history = [bd.Board().board_matrix]
        self.board = bd.Board()
        self.is_draw = False

    def show(self):
        self.board.show()

    def is_input_valid(self, user_input):
        (
            source_row_column,
            destination_row_column,
        ) = split_single_file_rank_to_old_new_row_column(user_input)
        return self.board.is_move_legal(source_row_column, destination_row_column)

    def is_game_over(self):
        return self.board.is_check_mate() or self.board.is_stale_mate() or self.is_draw

    def get_move_form_user(self):
        is_input_valid = False
        while not is_input_valid:
            user_input = input("movement:")
            is_input_valid = self.is_input_valid(user_input)
        return user_input

    def update_board(self, user_input):
        self.board.move_single_file_rank_input(user_input)
        self.board_history.append(self.board.board_matrix)

    def play_game(self, time_control):
        self.board.set_board_to_initial_configuration()
        while not self.is_game_over():
            self.show()
            move = self.get_move_form_user()
            # self.update_time()
            self.update_board(move)
            self.show()
