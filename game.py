import Board as bd
import Move as mv
import GameState as gs
from utilities.PositionConverter import *
from utilities.BoardUtilities import *
import Visual as vs


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
            gs.GameState(self.board).is_check_mate()
            or gs.GameState(self.board).is_stale_mate()
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
        return source_row_column, destination_row_column

    def update_board(self, source_row_column, destination_row_column):
        self.board.move(source_row_column, destination_row_column)
        self.board_history.append(self.board.board_matrix)
        self.board.turn = change_turn(self.board.turn)

    def play_game(self):
        self.board.set_board_to_initial_configuration()
        self.board.turn = "white"
        while not self.is_game_over():
            vs.Visual("white", self.board, "cyan").show()
            source_row_column, destination_row_column = self.get_move_form_user()
            self.update_board(source_row_column, destination_row_column)
