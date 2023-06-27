from utilities import file_rank_to_row_col, is_coordinate_in_board, numeric_to_algebraic, diagonal_squares, horizontal_squares, vertical_squares
import itertools
class pawn:
    def __init__(self, position_file_rank, color):
        self.position_file_rank = position_file_rank
        self.color = color
        self.type = 'pawn'
    def is_in_starting_position(self):
        position_row_column = file_rank_to_row_col(self.position_file_rank)
        if self.color == 'white':
            return position_row_column[0] == 1
        else:
            return position_row_column[0] == 6

    def show_forward_squares(self, amount, direction):
        color_to_sign = {'white': 1, 'black': -1}
        position_row_column = file_rank_to_row_col(self.position_file_rank)
        return (position_row_column[0] + color_to_sign[self.color] * amount, position_row_column[1] + direction * amount)
    def moves_in_range(self):
        position_row_column = file_rank_to_row_col(self.position_file_rank)
        amounts = [1,2,1,1]
        directions = [0,0,-1, 1]
        if not self.is_in_starting_position():
            amounts.pop(1)
            directions.pop(1)
        squares_in_range = [self.show_forward_squares(amount, direction) for amount, direction in zip(amounts, directions) if is_coordinate_in_board(self.show_forward_squares(amount, direction) )  ]
        return squares_in_range
    def is_in_range(self, new_position_file_rank):
        return file_rank_to_row_col(new_position_file_rank) in self.moves_in_range()
        
class knight:
    def __init__(self, position_file_rank, color):
        self.position_file_rank = position_file_rank
        self.color = color
        self.type = 'knight'
    def moves_in_range(self):
        position_row_column = algebraic_position(self.position_file_rank)
        
class bishop:
    def __init__(self, position_file_rank, color):
        self.position_file_rank = position_file_rank
        self.color = color
        self.type = 'bishop'
    def moves_in_range(self):
        print(self.position_file_rank)
        return diagonal_squares(self.position_file_rank ) 
        
class rook:
    def __init__(self, position_file_rank, color):
        self.position_file_rank = position_file_rank
        self.color = color
        self.type = 'rook'
    def moves_in_range(self):
        return horizontal_squares(self.position_file_rank) + vertical_squares(self.position_file_rank)
class queen:
    def __init__(self, position_file_rank, color):
        self.position_file_rank = position_file_rank
        self.color = color
        self.type = 'queen'
    def moves_in_range(self):
        return horizontal_squares(self.position_file_rank) + vertical_squares(self.position_file_rank) +  diagonal_squares(self.position_file_rank)
class king:
    def __init__(self, position_file_rank, color):
        self.position_file_rank = position_file_rank
        self.color = color
        self.type = 'king'
    def moves_in_range(self):
        position_row_column = file_rank_to_row_col(self.position_file_rank)
        return horizontal_squares(self.position_file_rank, amount= 1) + vertical_squares(self.position_file_rank, amount= 1) +  diagonal_squares(self.position_file_rank, amount= 1)
pp = pawn('a7','white')