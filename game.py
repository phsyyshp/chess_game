import numpy as np 
import piece_types as pc
from utilities import create_empty_board, algebraic_to_numeric, is_coordinate_in_board, numeric_to_algebraic
import itertools

class game:

    def __init__(self):
        self.board = np.full((8,8), 0)
        self.turn = "white"
        self.players = []
    
    def is_path_clear(self, piece_object, new_position):
        if  self.board[new_position[0]][new_position[1]] != 0:
            return False
        elif piece_object.type() in ["king", "knigth"]:
            path = piece_object.get_path(new_position)
            is_path_clear = 0 == sum(self.board * path)
            return is_path_clear
        else:
            return True
    
    def is_legal_move(self, old_position, new_position):
        piece_to_move = self.board[new_position[0]][new_position[1]] 
        piece_object = pc.piece_types(piece_to_move).create_piece(old_position)
        return piece_to_move != 0 and self.is_path_clear(piece_object, new_position) and self.is_safe(old_position, new_position)

    def move_piece(self, old_position, new_position):
        if self.is_legal_move(old_position, new_position):
            self.board[new_position[0]][new_position[1]] = self.board[old_position[0]][old_position[1]] 
            self.board[old_position[0]][old_position[1]] = 0
        else:
            return False 

    def create_board(self):
        ''''creates a board with all the pieces in their starting positions'''
        white_piece_id_to_name = {"pawn": 7, "knigth": 6, "bishop": 5, "rook": 4, "queen": 3, "king": 2}
        # add white pieces
        self.board[1][:] = white_piece_id_to_name["pawn"]
        self.board[0][:] = np.array([ white_piece_id_to_name["rook"],white_piece_id_to_name["knigth"],white_piece_id_to_name["bishop"],white_piece_id_to_name["queen"],white_piece_id_to_name["king"],white_piece_id_to_name["bishop"],white_piece_id_to_name["knigth"],white_piece_id_to_name["rook"]])
        # add black pieces
        self.board[6][:] = 6 + self.board[1][:]
        self.board[7][:] = 6 + self.board[0][:]
            
    def show_board(self):
        vectorized_chr = np.vectorize(chr)
        visual_board = self.board + 9810 * (self.board != 0) 
        visual_board = vectorized_chr( 32*(visual_board == 0)  + visual_board).tolist()
        visual_board = [["\x1b[26;30;46m " + visual_board[i][j] + " \x1b[0m" if (i+j)%2 == 0 else "\x1b[26;30;47m " + visual_board[i][j] + " \x1b[0m"  for j in range(8)] for i in range(8)]
        print( "\n".join(["".join(item) for item in visual_board ]  ) ) 



gg=game()

gg.create_board()
gg.show_board()
old_pos = algebraic_to_numeric("e2") 
new_pos = algebraic_to_numeric("e3") 
gg.move_piece(old_pos,new_pos)
gg.show_board()