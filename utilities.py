import numpy as np

def file_rank_to_row_col(position_file_rank):
    print(position_file_rank)
    return (int(position_file_rank[1])-1,ord(position_file_rank[0])-97)

def is_coordinate_in_board(position_row_column):
    if position_row_column[0] in range(8) and position_row_column[1] in range(8):
        return True
    else:
        return False

def row_col_to_file_rank(position_row_column):
    return chr(position_row_column[1]+97)+str(position_row_column[0]+1)

def find_bounds(input_val, amount):
    if  input_val-amount < 0:
        lower_bound = 0
    else:
        lower_bound = input_val - amount 
    if amount + input_val > 7:
        upper_bound = 7
    else:
        upper_bound = amount + input_val
    return lower_bound, upper_bound

def diagonal_squares(position_file_rank, amount = 8):
    position_row_column = file_rank_to_row_col(position_file_rank)
    min_cord = min(position_row_column[0], position_row_column[1])
    lower_bound, upper_bound = find_bounds(min_cord, amount)
    
    indices = np.arange(lower_bound, upper_bound+1)
    indices_to_raveled = np.stack([indices, indices])
    raveled_indices = np.ravel_multi_index(indices_to_raveled, (8,8))
    binary_mat = np.zeros((1,64))
    binary_mat[0][raveled_indices] = 1
    binary_mat = binary_mat.reshape((8,8))
    binary_mat[position_row_column[0]][position_row_column[1]] = 0
    return  binary_mat

def horizontal_squares(position_file_rank, amount = 8):
    position_row_column = file_rank_to_row_col(position_file_rank)
    binary_mat = np.zeros((8,8))
    lower_bound, upper_bound = find_bounds(position_row_column[1], amount)
    binary_mat[position_row_column[0]][lower_bound:upper_bound+1] = 1 
    binary_mat[position_row_column[0]][position_row_column[1]] = 0
    return binary_mat

def vertical_squares(position_file_rank, amount = 8):
    position_row_column = file_rank_to_row_col(position_file_rank)
    binary_mat = np.zeros((8,8))
    lower_bound, upper_bound = find_bounds(position_row_column[0], amount)
    
    binary_mat[lower_bound:upper_bound+1][position_row_column[1]]= 1 

    binary_mat[position_row_column[0]][position_row_column[1]] = 0
    return binary_mat

def piece_id_to_color(piece_id):
    color = "white" if piece_id <= 7 else   "black"
    return  color
def is_alg_not(input):
    pass
#print(horizontal_squares("d4",1))
#print(diagonal_squares("d4",7))