import board as bd
import numpy as np
from    utilities  import *
class game:

    def __init__(self):
        self.board_history = [bd.create_board().board_mat]  
        self.board = bd.create_board()

    def show(self):
       pass 
    def is_alg_not_valid(self,alg_not):
        if self.board.is_alg_not_legal(alg_not):
            return self.board.is_legal_move(alg_not)
    def is_input_valid(self, user_input):
        
        if is_alg_not(user_input):
            return self.is_alg_not_valid(user_input)
        else:
            return False

    def get_mov_frm_usr(self):
        is_input_valid = False
        while not is_input_valid:
            user_input = input("movement:")
            is_input_valid = self.is_input_valid(user_input)
        return user_input

    def update_board(self, mov_alg_not):
        self.board.move(mov_alg_not)
        self.board_history.append(self.board.board_mat)


    def play_game(self, time_control):
        while not self.is_over():
           self.show()
           mov_alg_not = self.get_mov_frm_usr()
           self.update_time()
           mov_alg_not = self.add_color_inf_to_alg_not(mov_alg_not, self.turn)
           self.update_board(mov_alg_not)
           self.show()
           