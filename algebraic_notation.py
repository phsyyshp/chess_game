from utilities import algebraic_to_numeric
class algebraic_notation:
    def __init__(self, alg_not ):
        self.alg_not =alg_not

    def is_alg_not(self):
        pass
    def color(self):
        color = "black" if self.alg_not[2] == "." else "white"
        #print(self.alg_not[2])
        
        return color
    def is_castling(self):
        pass
    def is_disamg_move(self):
        
        pass
    def is_draw_ofr(self):
        pass
    def is_pawn_promotion(self):
        pass
    def possible_dept_sqrs(self):
        pass
    def piece_id(self):
        color_code = 0 if self.color() == "white" else 6
        #print(color_code)
        num_to_let = {"N":6, "R":4, "K":2, "Q":3, "B":5}
        return num_to_let[self.alg_not[-3]] + color_code
    def destination_cord(self):
        return self.alg_not[-2] + self.alg_not[-1]       
    def dept_coor(self):
        pass
    def num_coor(self):
        return self.dept_coor() , self.dest_coor()


a=algebraic_notation("4.....Bb2")
#print(a.color())