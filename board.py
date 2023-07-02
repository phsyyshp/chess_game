import numpy as np 
import piece_types as pct
from utilities import  *

class board:

    def __init__(self):
        self.board_matrix = np.full((8,8), 0)
    
    def set_board_to_initial_configuration(self):
        white_piece_id_to_name = {"pawn": 7, "knight": 6, "bishop": 5, "rook": 4, "queen": 3, "king": 2, "emp": 0}
        self.board_matrix[1][:] = white_piece_id_to_name["pawn"]
        self.board_matrix[0][:] = np.array([ white_piece_id_to_name["rook"],white_piece_id_to_name["knight"],white_piece_id_to_name["bishop"],white_piece_id_to_name["queen"],white_piece_id_to_name["king"],white_piece_id_to_name["bishop"],white_piece_id_to_name["knight"],white_piece_id_to_name["rook"]])
        self.board_matrix[6][:] = 6 + self.board_matrix[1][:]
        self.board_matrix[7][:] = 6 + self.board_matrix[0][:]

    def get_piece_id_from_position(self, position_row_column):
        return self.board_matrix[position_row_column[0]][position_row_column[1]] 

    def is_square_empty(self, position_row_column):
        return  self.get_piece_id_from_position(position_row_column) == 0

    def get_piece_object_from_position(self, position_row_column):
        piece_id = self.get_piece_id_from_position(position_row_column)
        piece_object = pct.piece_types(piece_id).create_piece(position_row_column)
        return piece_object

    def is_destination_occupied_by_same_color(self, piece_object, new_position_row_column):
        return piece_object.color() == piece_id_to_color(self.get_piece_id_from_position(new_position_row_column)) if not self.is_square_empty(new_position_row_column) else False
    
    def is_pawn_path_clear(self, piece_object, new_position_row_column):
        if piece_object.is_capture_attempt(new_position_row_column):
            return not self.is_destination_occupied_by_same_color(piece_object, new_position_row_column) and not self.is_square_empty(new_position_row_column)
        else:
            path = piece_object.get_path(new_position_row_column)
            is_path_clear = 0 == sum(self.board_matrix * path)
            return is_path_clear
    
    def is_path_clear(self, piece_object, new_position_row_column):
        if self.is_destination_occupied_by_same_color(piece_object, new_position_row_column):
            return False
        elif piece_object.is_slider():
            path = piece_object.get_path(new_position_row_column)
            is_path_clear = 0 == sum(self.board_matrix * path)
            return is_path_clear
        elif piece_object.type() in ["king", "knight"]:
            return not self.is_destination_occupied_by_same_color(piece_object, new_position_row_column)
        elif piece_object.type() == "pawn":
            return self.is_pawn_path_clear(piece_object, new_position_row_column)
        else:
            return True
    # do it like is_attacked_by_slider etc
    def is_under_attack_by_specific_piece(self, position_row_column, attacker_piece_type, attacker_color):
        attacker_piece_id = piece_type_to_piece_id(attacker_piece_type)
        positions_of_attacker_pieces_bin_mat = self.get_positions_of_piece_type(attacker_piece_id)
        attack_bool = [self.is_attack_possible(position_row_column, position_of_attacker_piece) for position_of_attacker_piece in positions_of_attacker_pieces] 

    def is_under_attack_by_any_piece(self, position_row_column, attacker_color):
        is_attacked_by_specific_piece_bool_list = [self.is_under_attack_by_specific_piece(position_row_column, attacker_piece_type, attacker_color) for attacker_piece_type in ["pawn", "queen", "king", "bishop", "rook", "knight"]]
        return any(is_attacked_by_specific_piece_bool_list) 

    def is_castling_attempt(self, piece_object, new_position_row_column):
        if piece_object.type() == "king":
            return (abs(piece_object.position_row_column[1] - new_position_row_column[0])) == 2 and not (does_rank_change(piece_object.position_row_column, new_position_row_column))
    
    def is_castling_legal(self, piece_object, new_position_row_column):
        
        castling_type = piece_object.get_castling_type(new_position_row_column)
        match castling_type:
            case "queen side":
                return self.is_queen_side_castling_legal(piece_object)
            case "king side":
                return self.is_king_side_castling_legal(piece_object)
#show castling by king movement
    def is_move_legal(self, old_position_row_column, new_position_row_column):
        piece_object = self.get_piece_object_from_position(old_position_row_column)
        if self.is_square_empty(old_position_row_column):
            return False
        elif self.is_castling_attempt(piece_object, new_position_row_column):
            return self.is_castling_legal(piece_object, new_position_row_column)
        elif not self.is_in_range(piece_object, new_position_row_column):
            return False
        else:
            return self.is_path_clear(piece_object, new_position_row_column) and self.is_safe(old_position_row_column, new_position_row_column) 

    def is_pawn_promotion_legal(self, old_position_row_column, new_position_row_column):
        pass

    def move_general_input(self, movement_str):
        "movement_str is in format of file"
        match self.type_of_movement(movement_str):  
            case  "castle":
               self.castle(movement_str)
            case "pawn_promotion":
               self.pawn_promotion(movement_str)
            case "regular":
                old_position_row_column, new_position_row_column = self.movement_str_to_old_new_position_row_column(movement_str)
                self.move(old_position_row_column, new_position_row_column)
        
    def move(self, old_position_row_column, new_position_row_column):
        self.board_matrix[new_position_row_column[0]][new_position_row_column[1]] = self.get_piece_id_from_position(old_position_row_column)
        self.board_matrix[old_position_row_column[0]][old_position_row_column[1]] = 0

    def move_single_file_rank_input(self, old_new_file_rank):
        old_position_row_column, new_position_row_column = split_single_file_rank_to_old_new_row_column(old_new_file_rank)
        self.move(old_position_row_column, new_position_row_column)

    def show(self, point_of_view = "white"):
        vectorized_chr = np.vectorize(chr)

        visual_board = self.board_matrix + 9810 * (self.board_matrix != 0) 
        visual_board = np.flipud(visual_board) if point_of_view == "white" else np.fliplr(visual_board)
        visual_board = vectorized_chr( 32*(visual_board == 0)  + visual_board).tolist()
        visual_board = [["\x1b[26;36;40m" + str(8-i) + " "] + ["\x1b[26;30;46m " + visual_board[i][j] + " \x1b[0m" if (i+j + 1)%2 == 0 else "\x1b[26;30;47m " + visual_board[i][j] + " \x1b[0m"  for j in range(8)] for i in range(8)]
        visual_board_string = "\n".join(["".join(item) for item in visual_board ]  )
        rank_column = "\n".join(map(str,range(8,0,-1)))
        file_row = "  ".join("bcdefgh") if point_of_view == "white" else "  ".join("gfedcba")
        file_row = "  a  " + file_row if point_of_view == "white" else "\n   h   " + file_row
        visual_board_string += "\n\x1b[26;36;40m " + file_row +" \x1b[0m" 
        print(visual_board_string) 

gg=board()
gg.set_board_to_initial_configuration()
gg.show()
print(" ")
gg.move_single_file_rank_input('d2d4')
gg.show()
#print(gg.is_ambiguous(an.algebraic_notation("4.Qb2")))
