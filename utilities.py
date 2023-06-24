import numpy as np

def create_empty_board():
    visual_board = np.full((8,8),0)
    for i in range(8):
        for j in range(8):
            if (i+j)%2==0:
               visual_board[i][j] = ord("█")
            else:
               visual_board[i][j] =32 
    return visual_board
def algebraic_to_numeric(algebraic_position):
    return (int(algebraic_position[1])-1,ord(algebraic_position[0])-97)
def is_coordinate_in_board(numeric_position):
    if numeric_position[0] in range(8) and numeric_position[1] in range(8):
        return True
    else:
        return False
def numeric_to_algebraic(numeric_position):
    return chr(numeric_position[1]+97)+str(numeric_position[0]+1)
def diagonal_squares(position, slope, amount = 8):
    numeric_position = algebraic_to_numeric(position)
    squares_in_range = [(numeric_position[0] + slope*i, numeric_position[1]+i) for i in range(-amount,amount + 1)  if is_coordinate_in_board((numeric_position[0] + slope*i, numeric_position[1]+i))]
    squares_in_range.remove(numeric_position)
    return squares_in_range

def horizontal_squares(position, amount = 8):
    numeric_position = algebraic_to_numeric(position)
    squares_in_range = [(numeric_position[0], numeric_position[1] + i) for i in range(-amount, amount+1) if is_coordinate_in_board((numeric_position[0], numeric_position[1] + i))]
    squares_in_range.remove(numeric_position)
    return squares_in_range

def vertical_squares(position, amount = 8):
    numeric_position = algebraic_to_numeric(position)
    squares_in_range = [(i + numeric_position[0], numeric_position[1]) for i in range(-amount, amount+1) if is_coordinate_in_board((i + numeric_position[0], numeric_position[1]))]
    squares_in_range.remove(numeric_position)
    return squares_in_range 
def piece_id_to_color(piece_id):
    color = "white" if piece_id_number <= 7 else   "black"
    return  color