import numpy as np 
import piece_types as pc
from utilities import create_empty_board, algebraic_to_numeric, is_coordinate_in_board, numeric_to_algebraic, piece_id_to_color
import itertools

class board:

    def __init__(self):
        self.board = np.full((8,8), 0)
    
    def create_board(self):
        ''''creates a board with all the pieces in their starting positions'''
        white_piece_id_to_name = {"pawn": 7, "knigth": 6, "bishop": 5, "rook": 4, "queen": 3, "king": 2, "empty": 0}
        # add white pieces
        self.board[1][:] = white_piece_id_to_name["pawn"]
        self.board[0][:] = np.array([ white_piece_id_to_name["rook"],white_piece_id_to_name["knigth"],white_piece_id_to_name["bishop"],white_piece_id_to_name["queen"],white_piece_id_to_name["king"],white_piece_id_to_name["bishop"],white_piece_id_to_name["knigth"],white_piece_id_to_name["rook"]])
        # add black pieces black piece id is white piece id + 6
        self.board[6][:] = 6 + self.board[1][:]
        self.board[7][:] = 6 + self.board[0][:]

    def get_piece_id_from_position(self, position):
        return self.board[position[0]][position[1]] 

    def is_empty(self, position):
        return  self.get_piece_id_from_position(position) == 0

    def is_path_clear(self, old_position, new_position):
        id_of_piece_to_move = self.get_piece_id_from_position(old_position)
        piece_object = pc.piece_types(id_of_piece_to_move).create_piece(old_position)

        is_destination_occupied_by_same_color = piece_object.color() == piece_id_to_color(self.get_piece_id_from_position(new_position)) if not self.is_empty(new_position) else False
        if   is_destination_occupied_by_same_color:
            return False
        elif piece_object.type() not in ["king", "knigth"]:
            path = piece_object.get_path(new_position)
            is_path_clear = 0 == sum(self.board * path)
            return is_path_clear
        else:
            return True
    
    def is_legal_move(self, old_position, new_position):
        
        return  (not self.is_empty(old_position)) and self.is_path_clear(old_position, new_position) and self.is_safe(old_position, new_position)

    def move_piece(self, old_position, new_position):
        if self.is_legal_move(old_position, new_position):
            self.board[new_position[0]][new_position[1]] = self.board[old_position[0]][old_position[1]] 
            self.board[old_position[0]][old_position[1]] = 0
            return True
        else:
            return False 

    
            
    def show(self):
        vectorized_chr = np.vectorize(chr)
        visual_board = self.board + 9810 * (self.board != 0) 
        visual_board = vectorized_chr( 32*(visual_board == 0)  + visual_board).tolist()
        visual_board = [["\x1b[26;30;46m " + visual_board[i][j] + " \x1b[0m" if (i+j)%2 == 0 else "\x1b[26;30;47m " + visual_board[i][j] + " \x1b[0m"  for j in range(8)] for i in range(8)]
        print( "\n".join(["".join(item) for item in visual_board ]  ) ) 



gg=board()

gg.create_board()
gg.show()
old_pos = algebraic_to_numeric("e2") 
new_pos = algebraic_to_numeric("e3") 
gg.move_piece(old_pos,new_pos)
gg.show_board()