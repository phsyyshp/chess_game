import numpy as np

def algebraic_to_numeric(algebraic_position):
    print(algebraic_position)
    return (int(algebraic_position[1])-1,ord(algebraic_position[0])-97)
def is_coordinate_in_board(numeric_position):
    if numeric_position[0] in range(8) and numeric_position[1] in range(8):
        return True
    else:
        return False
def numeric_to_algebraic(numeric_position):
    return chr(numeric_position[1]+97)+str(numeric_position[0]+1)
def diagonal_squares(position, amount = 8):
    numeric_position = algebraic_to_numeric(position)
    min_coor = min(numeric_position[0], numeric_position[1])
    if  min_coor-amount < 0:
        lower_bound = 0
    else:
        lower_bound = min_coor - amount 
    if amount + min_coor > 7:
        upper_bound = 7
    else:
        upper_bound = amount + min_coor
    indices = np.arange(lower_bound, upper_bound+1)
    indices_to_raveled = np.stack([indices, indices])
    raveled_indices = np.ravel_multi_index(indices_to_raveled, (8,8))
    binary_mat = np.zeros((1,64))
    binary_mat[0][raveled_indices] = 1
    binary_mat = binary_mat.reshape((8,8))
    binary_mat[numeric_position[0]][numeric_position[1]] = 0
    return  binary_mat

def horizontal_squares(position, amount = 8):
    numeric_position = algebraic_to_numeric(position)
    binary_mat = np.zeros((8,8))

    if  numeric_position[1]-amount < 0:
        lower_bound = 0
    else:
        lower_bound = numeric_position[1] - amount 
    if amount + numeric_position[1] > 7:
        upper_bound = 7
    else:
        upper_bound = amount + numeric_position[1]


    binary_mat[numeric_position[0]][lower_bound:upper_bound+1] = 1 

    binary_mat[numeric_position[0]][numeric_position[1]] = 0
    return binary_mat


def vertical_squares(position, amount = 8):
    numeric_position = algebraic_to_numeric(position)
    binary_mat = np.zeros((8,8))

    if  numeric_position[0]-amount < 0:
        lower_bound = 0
    else:
        lower_bound = numeric_position[0] - amount 
    if amount + numeric_position[0] > 7:
        upper_bound = 7
    else:
        upper_bound = amount + numeric_position[0]


    binary_mat[lower_bound:upper_bound+1][numeric_position[1]]= 1 

    binary_mat[numeric_position[0]][numeric_position[1]] = 0
    return binary_mat

def piece_id_to_color(piece_id):
    color = "white" if piece_id <= 7 else   "black"
    return  color
def is_alg_not(input):
    pass
#print(horizontal_squares("d4",1))
#print(diagonal_squares("d4",7))