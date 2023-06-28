import numpy as np 
import piece_types as pct
import algebraic_notation as an
from utilities import  algebraic_to_numeric, is_coordinate_in_board, numeric_to_algebraic, piece_id_to_color
import itertools

class board:
    def __init__(self):
        self.board_matrix = np.full((8,8), 0)
    
    def set_to_init_conf(self):
        white_piece_id_to_name = {"pawn": 7, "knight": 6, "bishop": 5, "rook": 4, "queen": 3, "king": 2, "emp": 0}
        self.board_matrix[1][:] = white_piece_id_to_name["pawn"]
        self.board_matrix[0][:] = np.array([ white_piece_id_to_name["rook"],white_piece_id_to_name["knigth"],white_piece_id_to_name["bishop"],white_piece_id_to_name["queen"],white_piece_id_to_name["king"],white_piece_id_to_name["bishop"],white_piece_id_to_name["knigth"],white_piece_id_to_name["rook"]])
        self.board_matrix[6][:] = 6 + self.board_matrix[1][:]
        self.board_matrix[7][:] = 6 + self.board_matrix[0][:]

    def get_piece_id_from_position(self, position_row_column):
        return self.board_matrix[position_row_column[0]][position_row_column[1]] 

    def is_square_empty(self, position_row_column):
        return  self.get_piece_id_from_position(position_row_column) == 0

    #fix position types
    def is_path_clear(self, old_position_row_column, new_position_row_column):
        piece_id = self.get_piece_id_from_position(old_position_row_column)
        piece_object = pct.piece_types(piece_id).create_piece(old_position_row_column)
        is_destination_occupied_by_same_color = piece_object.color() == piece_id_to_color(self.get_piece_id_from_position(new_position_row_column)) if not self.is_square_empty(new_position_row_column) else False
        if   is_destination_occupied_by_same_color:
            return False
        elif piece_object.type() not in ["king", "knight"]:
            path = piece_object.get_path(new_position_row_column)
            is_path_clear = 0 == sum(self.board_matrix * path)
            return is_path_clear
        else:
            return True

    def possible_departure_squares(self, algebraic_notation):
        """ it multiplies the binary mask of the movement of the piece by the board matrix and returns the indices of the pieces that can move to the destination square
        """
        piece_id = algebraic_notation.piece_id()
        new_position_row_column = algebraic_notation.destination_cord()
        dummy_piece = pct.piece_types(piece_id).create_piece(new_position_row_column)
        movement_mask = dummy_piece.moves_in_range()
        masked_board = self.board_matrix * movement_mask
        indices_of_pieces = np.where(masked_board == piece_id)
        for index in indices_of_pieces:
            if not self.is_legal_move(tuple(index), new_position_row_column):
                indices_of_pieces.remove(index)
        return indices_of_pieces 

    def is_ambiguous(self, algebraic_notation, board_history=0):
        if algebraic_notation.is_special_move():
            return False
        else:
            return len(self.possible_departure_squares(algebraic_notation)) == 1

    def is_legal_castling(self ):
#        if algebraic_notation.is_castling():
#            color = algebraic_notation.color()
#        else:
#            return False
#
#
#        pass
        return True
    
    def is_legal_move(self, old_position_row_column, new_position_row_column):
        if  self.is_legal_castling():
            return True
        elif self.is_legal_pawn_promotion():
            return True
        
        else:
#            old_position, new_position = algebraic_notation_obj.num_coor()
        
            return  (not self.is_square_empty(old_position_row_column)) and self.is_path_clear(old_position_row_column, new_position_row_column) and self.is_safe(old_position_row_column, new_position_row_column) and (self.pc_type(old_position_row_column) == algebraic_notation_obj.piece_type_to_move())

    def move(self, algebraic_notation):
        if self.is_legal_castling():
            self.castle(algebraic_notation)
        elif self.is_legal_pawn_promotion():
            self.pawn_promotion(algebraic_notation)
        else: 
            old_position_row_column, new_position_row_column = self.alg_to_num_coor(algebraic_notation)
            self.board_matrix[new_position_row_column[0]][new_position_row_column[1]] = self.get_piece_id_frm_pos(old_position_row_column)
            self.board_matrix[old_position_row_column[0]][old_position_row_column[1]] = 0
            
    def show(self):
        vectorized_chr = np.vectorize(chr)
        visual_board = self.board_matrix + 9810 * (self.board_matrix != 0) 
        visual_board = vectorized_chr( 32*(visual_board == 0)  + visual_board).tolist()
        visual_board = [["\x1b[26;30;46m " + visual_board[i][j] + " \x1b[0m" if (i+j)%2 == 0 else "\x1b[26;30;47m " + visual_board[i][j] + " \x1b[0m"  for j in range(8)] for i in range(8)]
        print( "\n".join(["".join(item) for item in visual_board ]  ) ) 



gg=board()

gg.set_to_init_conf()
#gg.show()
print(gg.is_ambiguous(an.algebraic_notation("4.Qb2")))
