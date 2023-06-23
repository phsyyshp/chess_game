import numpy as np 
import individual_pieces as pc
from utilities import create_empty_board, algebraic_to_numeric, is_coordinate_in_board, numeric_to_algebraic
import itertools
class game:
    def __init__(self):
        self.board = np.full((8,8), 0)
        self.turn = "white"
        self.players = []
    def add_piece(self, piece):
        numeric_position = algebraic_to_numeric(piece.position)
        self.board[numeric_position[0]][numeric_position[1]] = piece
    def remove_piece(self, piece):
        numeric_position = algebraic_to_numeric(piece.position)
        self.board[numeric_position[0]][numeric_position[1]] = "  "
    def get_piece_from_position(self, position):
        numeric_position = algebraic_to_numeric(position)
        return self.board[numeric_position[0]][numeric_position[1]]
    def is_check(self):
        return True
    def is_new_position_check(self, piece, new_position):
        current_board = self.board
        self.move_piece(piece.position, new_position)
        if self.is_check():
            self.board = current_board
            return True
        else:
            self.board = current_board
            return False 
    def is_path_empty(self, piece, new_position):
        #if 
        return True
    def is_legal_move(self, old_position, new_position):
        piece = self.get_piece_from_position(old_position)
        if piece.is_in_range(new_position) and self.is_path_empty(piece, new_position) and not self.is_check( ) and not self.is_new_position_check(piece, new_position):
            return True
    def move_piece(self, old_position, new_position):
        piece = self.get_piece_from_position(old_position) 
        self.remove_piece(piece)
        piece.position = new_position
        self.add_piece(piece)
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

    def show_moves_in_range(self, position):
        '''prints the board in a visual format with the moves in range of the piece in the position highlighted'''
        piece = self.get_piece_from_position(position)
        moves_in_range = piece.moves_in_range()
        current_board = self.board
        
        visual_board = create_empty_board()

        for square in moves_in_range:
            visual_board[square[0]][square[1]] = "░░"
        for i, j in itertools.product(range(8), range(8)):
            if type(self.board[i][j]) != type(""):
                visual_board[i][j] = self.board[i][j].icon()
        for i in range(8):
            visual_board[i] = "".join(visual_board[i])
        visual_board = "\n".join(visual_board)
        print(visual_board)
        
        self.board = current_board


gg=game()

gg.create_board()
gg.show_board()
#print(gg.board)
#gg.create_board()
#gg.add_piece(pc.pawn("b3","white"))
#gg.remove_piece(pc.pawn("b2","white"))
#gg.move_piece("g1","f3")
#gg.show_board()
#print([numeric_to_algebraic(a) for a in gg.board[0][2].moves_in_range()])
#gg.add_piece(pc.bishop("d4","white"))
#gg.add_piece(pc.rook("b4","black"))
#gg.show_board()
#print([numeric_to_algebraic(a) for a in gg.board[3][3].moves_in_range()])
#gg.show_moves_in_range("b4")