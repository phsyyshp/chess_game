import board as bd
import numpy as np
from    utilities  import *
class game:

    def __init__(self):
        self.board_history = [bd.create_board().board]  
        self.board = bd.create_board()

    def show(self):
        graphic_of_board = self.board_history[-1].show(is_print = 0)
        graph_of_remaining_time = self.remaining_time.show()
        graphe_of_captured_pieces = self

#        self.showa
        self.board_history[-1].show()
        self.player1.show_captured_pieces()

    def is_input_valid(self, user_input):
        if is_alg_not(user_input):
            return self.board.is_movement_valid(algebraic_notation)
        else:
            return False

    def get_mov_frm_usr(self):
        is_input_valid = False
        while not is_input_valid:
            movement = input("movement:")
            is_input_valid = self.is_input_valid()

    def update_board(self, movement):
        old_position, new_position = self.board.algebric_input_to_num(movement)
        self.board.move_piece(old_position, new_position )
        self.board_history.append(self.board.board)


    def play_game(self, time_control):
        while not self.is_over():
           self.show()
           movement = self.get_movement_from_user()
           self.update_time()
           self.update_board(movement)
           self.show()
           