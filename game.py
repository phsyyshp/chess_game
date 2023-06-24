import board as bd
import numpy as np
from    utilities  import *
class game:
    def __init__(self):
        self.board_history = [bd.create_board()]
    def show(self):
        graphic_of_board = self.board_history[-1].show(is_print = 0)
        graph_of_remaining_time = self.remaining_time.show()
        graphe_of_captured_pieces = self

        self.show)a
        self.board_history[-1].show()
        self.player1.show_captured_pieces()
    def start_game(self, time_control):
        board = self.board_history[-1]
        while not self.is_over():
           self.show()
           movement = self.get_movement_from_user()
           self.update_time()
           board.update(movement)
           self.show()
           self.board_history.append(board)