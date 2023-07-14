import numpy as np


def remove_line_breaks(item: str):
    item = item.replace("\n", " ")
    return item


def is_contain_bracket(field: str):
    return "[" in field


def extract_str_between_apost(field: str):
    if '"' in field:
        return field.rsplit('"', 2)[1].replace("'", "") + "$"
    return field


def shift(array, direction):
    if direction == -1:
        return np.append(array[1:], False)
    return np.append(False, array[:-1])


vec_does_contain_bracket = np.vectorize(is_contain_bracket)

# def is_same_as_neighbor(array, direction_of_neighbor:int):
#     shifted_array = shift(array,direction_of_neighbor)

#     last_game_info_lines_mask = game_info_lines_mask * (False == slided_game_info_lines_mask)
#     slided_last_game_info_lines_mask = shift(last_game_info_lines_mask, -direction)
