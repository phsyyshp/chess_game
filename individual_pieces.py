from utilities import algebraic_to_numeric, is_coordinate_in_board, numeric_to_algebraic, diagonal_squares, horizontal_squares, vertical_squares
import itertools
color_dictionary ={"black": "<", "white": ">"}
class pawn:
    def __init__(self, position, color):
        self.position = position
        self.color = color
        self.type = 'pawn'
        self.letter = "p" + color_dictionary[color]
    def icon(self):
        if self.color == 'white':
            return '♟ '
        else:
            return '♙ '
    def is_in_starting_position(self):
        numeric_position = algebraic_to_numeric(self.position)
        if self.color == 'white':
            return numeric_position[0] == 1
        else:
            return numeric_position[0] == 6

    def show_forward_squares(self, amount, direction):
        color_to_sign = {'white': 1, 'black': -1}
        numeric_position = algebraic_to_numeric(self.position)
        return (numeric_position[0] + color_to_sign[self.color] * amount, numeric_position[1] + direction * amount)

        
    def moves_in_range(self):
        numeric_position = algebraic_to_numeric(self.position)

        amounts = [1,2,1,1]
        directions = [0,0,-1, 1]
        if not self.is_in_starting_position():
            amounts.pop(1)
            directions.pop(1)
        squares_in_range = [self.show_forward_squares(amount, direction) for amount, direction in zip(amounts, directions) if is_coordinate_in_board(self.show_forward_squares(amount, direction) )  ]
        return squares_in_range
    def is_in_range(self, new_position):
        return algebraic_to_numeric(new_position) in self.moves_in_range()
        


class knight:
    def __init__(self, position, color):
        self.position = position
        self.color = color
        self.type = 'knight'
        self.letter = "N"+ color_dictionary[color]
    def icon(self):
        if self.color == 'white':
            return '♞ '
        else:
            return '♘ '
class bishop:
    def __init__(self, position, color):
        self.position = position
        self.color = color
        self.type = 'bishop'
        self.letter = "B"+ color_dictionary[color]
    def icon(self):
        if self.color == 'white':
            return '♝ '
        else:
            return '♗ '
    def moves_in_range(self):
        return diagonal_squares(self.position, slope = 1) + diagonal_squares(self.position, slope = -1)
        
class rook:
    def __init__(self, position, color):
        self.position = position
        self.color = color
        self.type = 'rook'
        self.letter = "R"+ color_dictionary[color]
    def icon(self):
        if self.color == 'white':
            return '♜ '
        else:
            return '♖ '
    def moves_in_range(self):
        return horizontal_squares(self.position) + vertical_squares(self.position)
class queen:
    def __init__(self, position, color):
        self.position = position
        self.color = color
        self.type = 'queen'
        self.letter = "Q"+ color_dictionary[color]
    def icon(self):
        if self.color == 'white':
            return '♛ '
        else:
            return '♕ '
    def moves_in_range(self):
        return horizontal_squares(self.position) + vertical_squares(self.position) + diagonal_squares(self.position, slope = 1) + diagonal_squares(self.position, slope = -1)
class king:
    def __init__(self, position, color):
        self.position = position
        self.color = color
        self.type = 'king'
        self.letter = "K"+ color_dictionary[color]
    def icon(self):
        if self.color == 'white':
            return '♚ '
        else:
            return '♔ '
    def moves_in_range(self):
        numeric_position = algebraic_to_numeric(self.position)
        return horizontal_squares(self.position, amount= 1) + vertical_squares(self.position, amount= 1) + diagonal_squares(self.position, slope = 1, amount= 1) + diagonal_squares(self.position, slope = -1, amount= 1)
pp = pawn('a7','white')