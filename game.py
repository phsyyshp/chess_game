import board as bd
import numpy as np
import algebraic_notation as an
from    utilities  import *
class game:

    def __init__(self):
        self.board_history = [bd.create_board().board_mat]  
        self.board = bd.create_board()

    def show(self):
       pass 
    def is_alg_not_valid(self,alg_not):
        return self.board.is_legal_unique_mov(alg_not, self.board_history) 
    def is_input_valid(self, user_input):
        alg_not = an.algebraic_notation(user_input, self.turn)

        if alg_not.is_alg_not():
            return self.is_alg_not_valid(alg_not)
        else:
            return False

    def get_mov_frm_usr(self):
        is_input_valid = False
        while not is_input_valid:
            user_input = input("movement:")
            is_input_valid = self.is_input_valid(user_input)
        return an.algebraic_notation(user_input, self.turn)

    def update_board(self, mov_alg_not):
        self.board.move(mov_alg_not)
        self.board_history.append(self.board.board_mat)


    def play_game(self, time_control):
        while not self.is_over():
           self.show()
           mov_alg_not = self.get_mov_frm_usr()
           self.update_time()
           self.update_board(mov_alg_not)
           self.show()
           