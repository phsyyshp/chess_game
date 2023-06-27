import numpy as np
import itertools

def file_rank_to_row_column(position_file_rank):
    return (int(position_file_rank[1])-1,ord(position_file_rank[0])-97)

def is_coordinate_in_board(position_row_column):
    if position_row_column[0] in range(8) and position_row_column[1] in range(8):
        return True
    else:
        return False

def row_column_to_file_rank(position_row_column):
    return chr(position_row_column[1]+97)+str(position_row_column[0]+1)

def find_bounds(input_val, amount):
    if  input_val - amount < 0:
        lower_bound = 0
    else:
        lower_bound = input_val - amount 
    if amount + input_val > 7:
        upper_bound = 7
    else:
        upper_bound = amount + input_val
    return lower_bound, upper_bound

def fill_indices(binary_mat, numpy_array_of_row_indices, numpy_array_of_column_indices, value_to_fill):
    indices_to_raveled = np.stack([numpy_array_of_row_indices, numpy_array_of_column_indices])
    raveled_indices = np.ravel_multi_index(indices_to_raveled, (8,8))
    binary_mat =  binary_mat.reshape((1,64))
    binary_mat[0][raveled_indices] = value_to_fill
    binary_mat = binary_mat.reshape((8,8))
    return binary_mat

def get_diagonal_path(old_position_row_column, new_position_row_column):
    binary_mat = np.zeros((8,8))
    indices = np.arrage()

def diagonal_squares(position_row_column, amount = 8, slope = -1):
    offset = (position_row_column[0] - slope*position_row_column[1])
    numpy_array_of_row_indices = slope * np.arange(0  , 8  ) + offset 
    numpy_array_of_column_indices = np.arange(0  , 8 ) 
    numpy_array_of_column_indices = numpy_array_of_column_indices[(0<=numpy_array_of_row_indices) * (numpy_array_of_row_indices<8)]
    numpy_array_of_row_indices = numpy_array_of_row_indices[(0<=numpy_array_of_row_indices) * (numpy_array_of_row_indices<8)]
    #print(numpy_array_of_row_indices)
    #print(numpy_array_of_column_indices)
    binary_mat = np.zeros((8,8))
    binary_mat = fill_indices(binary_mat, numpy_array_of_row_indices, numpy_array_of_column_indices, value_to_fill=1)
    binary_mat[position_row_column[0]][position_row_column[1]] = 0
    return  binary_mat

def horizontal_squares(position_row_column, amount = 8):
    binary_mat = np.zeros((8,8))
    lower_bound, upper_bound = find_bounds(position_row_column[1], amount)
    binary_mat[position_row_column[0]][lower_bound:upper_bound+1] = 1 
    binary_mat[position_row_column[0]][position_row_column[1]] = 0
    return binary_mat

def vertical_squares(position_row_column, amount = 8):
    binary_mat = np.zeros((8,8))
    lower_bound, upper_bound = find_bounds(position_row_column[0], amount)
    
    binary_mat[lower_bound:upper_bound+1][position_row_column[1]]= 1 

    binary_mat[position_row_column[0]][position_row_column[1]] = 0
    return binary_mat

def L_shaped_squares(position_row_column):
    binary_mat = np.zeros((8,8))
    for row_jump, column_jump in itertools.product([-1, 1], [-2, 2]):
        if is_coordinate_in_board((position_row_column[0] + row_jump, position_row_column[1] + column_jump)):
            binary_mat[position_row_column[0] + row_jump][ position_row_column[1] + column_jump] = 1 
    return binary_mat
def piece_id_to_color(piece_id):
    color = "white" if piece_id <= 7 else   "black"
    return  color
def is_alg_not(input):
    pass
#print(horizontal_squares("d4",1))
print(diagonal_squares(file_rank_to_row_column("d7"),7))
print(L_shaped_squares(file_rank_to_row_column("d8")))