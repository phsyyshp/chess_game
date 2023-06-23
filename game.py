import numpy as np 
import individual_pieces as pc
from utilities import create_empty_board, algebraic_to_numeric, is_coordinate_in_board, numeric_to_algebraic
import itertools

class game:

    def __init__(self):
        self.board = np.full((8,8), 0)
        self.turn = "white"
        self.players = []
    
    def move_piece(self, old_position, new_position):
        pass

    def create_board(self):
        ''''creates a board with all the pieces in their starting positions'''
        self.board[1][:] = 7
        self.board[0][:] = np.array([4,6,5,3,2,5,6,4])
        self.board[6][:] = 6 + self.board[1][:]
        self.board[7][:] = 6 + self.board[0][:]
            
    def show_board(self):
        vectorized_chr = np.vectorize(chr)
        self.board = self.board + 9810 * (self.board != 0) 
        visual_board = vectorized_chr( 32*(self.board == 0)  + self.board).tolist()
        visual_board = [["\x1b[26;30;46m " + visual_board[i][j] + " \x1b[0m" if (i+j)%2 == 0 else "\x1b[26;30;47m " + visual_board[i][j] + " \x1b[0m"  for j in range(8)] for i in range(8)]
        print( "\n".join(["".join(item) for item in visual_board ]  ) ) 



gg=game()

gg.create_board()
gg.show_board()