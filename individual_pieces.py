from utilities import *
import itertools

class pawn:
    def __init__(self, position_row_column, color):
        self.position_row_column = position_row_column
        self.color = color
        self.type = 'pawn'

    def is_in_starting_position(self):
        if self.color == 'white':
            return self.position_row_column[0] == 1
        else:
            return self.position_row_column[0] == 6

    def show_forward_squares(self, amount, direction):
        color_to_sign = {'white': 1, 'black': -1}
        return (self.position_row_column[0] + color_to_sign[self.color] * amount, self.position_row_column[1] + direction * amount)

    def is_capture_attempt(self, new_position_row_column):
        return does_file_change(self.position_row_column, new_position_row_column)
        
    def moves_in_range(self):
        amounts = [1,2,1,1]
        directions = [0,0,-1, 1]
        if not self.is_in_starting_position():
            amounts.pop(1)
            directions.pop(1)
        squares_in_range = [self.show_forward_squares(amount, direction) for amount, direction in zip(amounts, directions) if is_coordinate_in_board(self.show_forward_squares(amount, direction) )  ]
        return squares_in_range

    def is_slider(self):
        return False
        
class knight:
    def __init__(self, position_row_column, color):
        self.position_row_column = position_row_column
        self.color = color
        self.type = 'knight'
    
    def moves_in_range(self):
        return L_shaped_squares(self.position_row_column)

    def is_slider(self):
        return False

class bishop:
    def __init__(self, position_row_column, color):
        self.position_row_column = position_row_column
        self.color = color
        self.type = 'bishop'

    def moves_in_range(self):
        return diagonal_squares(self.position_row_column,amount=8, slope=1) + diagonal_squares(self.position_row_column,amount=8, slope=-1) 

    def get_path(self, new_position_row_column):
        return get_diagonal_path(self.position_row_column, new_position_row_column)

    def is_slider(self):
        return True
        
class rook:
    def __init__(self, position_row_column, color):
        self.position_row_column = position_row_column
        self.color = color
        self.type = 'rook'

    def moves_in_range(self):
        return horizontal_squares(self.position_row_column) + vertical_squares(self.position_row_column)

    def get_path(self, new_position_row_column):
        return get_straight_path(self.position_row_column, new_position_row_column)

    def is_slider(self):
        return True

class queen:
    def __init__(self, position_row_column, color):
        self.position_row_column = position_row_column
        self.color = color
        self.type = 'queen'

    def moves_in_range(self):
        return horizontal_squares(self.position_row_column) + vertical_squares(self.position_row_column) +  diagonal_squares(self.position_row_column,amount=8, slope=1) + diagonal_squares(self.position_row_column,amount=8, slope=-1) 

    def is_slider(self):
        return True

class king:
    def __init__(self, position_row_column, color):
        self.position_row_column = position_row_column
        self.color = color
        self.type = 'king'

    def moves_in_range(self):
        return horizontal_squares(self.position_row_column, amount= 1) + vertical_squares(self.position_row_column, amount= 1) +  diagonal_squares(self.position_row_column,amount=1, slope=1) + diagonal_squares(self.position_row_column,amount=1, slope=-1) 

    def is_slider(self):
        return False

    def get_castling_type(self, new_position_row_column):
        match self.color:
            case "black":
               increment_sign = 1
            case "white":
                increment_sign = -1
        castling_type = "short" if increment_horizontal(self.position_row_column, new_position_row_column) == increment_sign else "long"

pp = pawn('a7','white')