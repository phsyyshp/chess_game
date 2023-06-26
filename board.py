import numpy as np 
import piece_types as pct
import algebraic_notation as an
from utilities import  algebraic_to_numeric, is_coordinate_in_board, numeric_to_algebraic, piece_id_to_color
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
    def possible_dept(self, alg_not):
        pc_id = alg_not.pc_id_num()
        new_pos = alg_not.dest_coor()
        dum_pc = pct.piece_types(pc_id).create_piece(new_pos)
        sqrs_in_rng = dum_pc.moves_in_range()
        i=[]
        for sqr in sqrs_in_rng:
            pc_id_of_sqrs_in_rng = self.get_pc_id_frm_pos(sqr)
            #print(pc_id_of_sqrs_in_rng)
            if pc_id == pc_id_of_sqrs_in_rng:
                i.append(sqr) if self.is_legal_move(sqr, new_pos)
        return i
           

    def is_legal_unique_mov(self, alg_not, board_history):
        if alg_not.is_castling():
            return False
        elif alg_not.is_pawn_prom():
            return False
        elif alg_not.is_disamg_move():
            return False
        else:
            return len(self.possible_dept(alg_not)) == 1

    def is_legal_castling(self, alg_not):
        if alg_not.is_castling():
            color = alg_not.color()
        else:
            return False


        pass
    
    def is_legal_move(self, alg_not_obj):
        if  self.is_legal_castling():
            return True
        elif self.is_legal_pawn_prom():
            return True
        
        else:
            old_pos, new_pos = alg_not_obj.num_coor()
        
            return  (not self.is_sqr_emp(old_pos)) and self.is_path_clr(old_pos, new_pos) and self.is_safe(old_pos, new_pos) and (self.pc_type(old_pos) == alg_not_obj.piece_type_to_move())

    def move(self, alg_not):
        if self.is_legal_castling():
            self.castle(alg_not)
        elif self.is_legal_pawn_prom():
            self.pawn_prom(alg_not)
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

gg.set_to_init_conf()
gg.show()
print(gg.is_ambigious(an.algebraic_notation("4.Bb2")))
