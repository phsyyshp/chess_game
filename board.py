import numpy as np 
import piece_types as pct
frm utilities import create_emp_board, algebraic_to_numeric, is_coordinate_in_board, numeric_to_algebraic, pc_id_to_color
import itertools

class board:
    def __init__(self):
        self.board_mat = np.full((8,8), 0)
    
    def set_to_init_conf(self):
        ''''creates a board_mat with all the pcs in their starting pos'''
        white_pc_id_to_name = {"pawn": 7, "knigth": 6, "bishop": 5, "rook": 4, "queen": 3, "king": 2, "emp": 0}
        # add white pcs
        self.board_mat[1][:] = white_pc_id_to_name["pawn"]
        self.board_mat[0][:] = np.array([ white_pc_id_to_name["rook"],white_pc_id_to_name["knigth"],white_pc_id_to_name["bishop"],white_pc_id_to_name["queen"],white_pc_id_to_name["king"],white_pc_id_to_name["bishop"],white_pc_id_to_name["knigth"],white_pc_id_to_name["rook"]])
        # add black pcs black pc id is white pc id + 6
        self.board_mat[6][:] = 6 + self.board_mat[1][:]
        self.board_mat[7][:] = 6 + self.board_mat[0][:]

    def get_pc_id_frm_pos(self, pos):
        return self.board_mat[pos[0]][pos[1]] 

    def is_sqr_emp(self, pos):
        return  self.get_pc_id_frm_pos(pos) == 0

    def is_path_clr(self, old_pos, new_pos):
        id_of_pc_to_move = self.get_pc_id_frm_pos(old_pos)
        pc_obj = pct.piece_types(id_of_pc_to_move).create_piece(old_pos)

        is_destination_occupied_by_same_color = pc_obj.color() == pc_id_to_color(self.get_pc_id_frm_pos(new_pos)) if not self.is_sqr_emp(new_pos) else False
        if   is_destination_occupied_by_same_color:
            return False
        elif pc_obj.type() not in ["king", "knigth"]:
            path = pc_obj.get_path(new_pos)
            is_path_clr = 0 == sum(self.board_mat * path)
            return is_path_clr
        else:
            return True

    def is_legal_castling(alg_not):
        if alg_not.is_castling():
            color = alg_not.color()
        else:
            return False


        pass
    
    def is_legal_move(self, alg_not_obj):
        if self.is_legal_castling():
            return True
        else:
            old_pos, new_pos = self.alg_to_num_coor(alg_not)
        
            return  (not self.is_sqr_emp(old_pos)) and self.is_path_clr(old_pos, new_pos) and self.is_safe(old_pos, new_pos)

    def move(self, alg_not):
        if alg_not.is_legal_castling():
            self.castle(alg_not)
        else:
            old_pos, new_pos = self.alg_to_num_coor(alg_not)
            self.board_mat[new_pos[0]][new_pos[1]] = self.get_pc_id_frm_pos(old_pos)
            self.board_mat[old_pos[0]][old_pos[1]] = 0
            
    def show(self):
        vectorized_chr = np.vectorize(chr)
        visual_board = self.board_mat + 9810 * (self.board_mat != 0) 
        visual_board = vectorized_chr( 32*(visual_board == 0)  + visual_board).tolist()
        visual_board = [["\x1b[26;30;46m " + visual_board[i][j] + " \x1b[0m" if (i+j)%2 == 0 else "\x1b[26;30;47m " + visual_board[i][j] + " \x1b[0m"  for j in range(8)] for i in range(8)]
        print( "\n".join(["".join(item) for item in visual_board ]  ) ) 



gg=board()

gg.create_board_mat()
gg.show()
old_pos = algebraic_to_numeric("e2") 
new_pos = algebraic_to_numeric("e3") 
gg.move_pc(old_pos,new_pos)
gg.show()