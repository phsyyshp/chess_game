import numpy as np


def remove_line_breaks(item: str):
    item = item.replace("\n", " ")
    return item


def is_contain_bracket(field: str):
    return '"' in field


def extract_str_between_apost(field: str):
    if '"' in field:
        return ">".join(field.rsplit('"', 2)[:2]).replace("'", "") + "$"
    return field


def shift(array, direction):
    if direction == -1:
        return np.append(array[1:], False)
    return np.append(False, array[:-1])


def get_first_occurence_index(array, element):
    return np.where(array == element)[0][0]


def get_last_occurence_index(array, element):
    return np.where(array == element)[0][-1]


def get_next_to_last_occurence_index(array, element):
    return np.where(array == element)[0][-2]


def get_next_to_first_occurence_index(array, element):
    return np.where(array == element)[0][1]


def get_field_tag(item: str):
    out = item.split(">")[0]
    return out[1:].strip()


def get_field_value(item: str):
    out = item.split(">")[1]
    return out


def game_info_to_dic(game_info: list):
    # print(game_info)
    field_tag_list = list(map(get_field_tag, game_info))
    field_value_list = list(map(get_field_value, game_info))
    return dict(zip(field_tag_list, field_value_list))


vec_does_contain_bracket = np.vectorize(is_contain_bracket)

# def is_same_as_neighbor(array, direction_of_neighbor:int):
#     shifted_array = shift(array,direction_of_neighbor)

#     last_game_info_lines_mask = game_info_lines_mask * (False == slided_game_info_lines_mask)
#     slided_last_game_info_lines_mask = shift(last_game_info_lines_mask, -direction)
