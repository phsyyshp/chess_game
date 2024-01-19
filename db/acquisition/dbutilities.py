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


def split_long_lines(input_list, line_len=90):
    # TODO fix spliting isue
    item_len = list(map(len, input_list))
    item_len = np.array(item_len)
    long_lines_mask = item_len > line_len
    long_lines_index = np.where(long_lines_mask)[0]
    counter = 0
    for index in long_lines_index:
        temp = input_list[index + counter]
        input_list[index + counter] = input_list[index + counter][:line_len]
        input_list.insert(index + counter + 1, input_list[index + counter][line_len:])
        counter += 1
    # print(input_list[:50])
    return input_list


# @njit
def split_long_lines_numba(input_list, len_trsh):
    # new_list = []
    # for word in input_list:
    #     new_list.extend([word[i : i + len_trsh] for i in range(0, len(word), len_trsh)])
    # return new_list
    new_list = []
    for word in input_list:
        i = 0
        while i < len(word) or i == 0:
            new_list.append(word[i : i + len_trsh])
            i += len_trsh
    return new_list


vec_does_contain_bracket = np.vectorize(is_contain_bracket)


def units(bytes):
    if bytes < 1024:
        return f"{bytes} B"
    elif bytes < 1024**2:
        return f"{bytes / 1024:.2f} KB"
    elif bytes < 1024**3:
        return f"{bytes / 1024 ** 2:.2f} MB"
    elif bytes < 1024**4:
        return f"{bytes / 1024 ** 3:.2f} GB"
    elif bytes < 1024**5:
        return f"{bytes / 1024 ** 4:.2f} TB"
    else:
        return f"{bytes / 1024 ** 5:.2f} PB"


# def is_same_as_neighbor(array, direction_of_neighbor:int):
#     shifted_array = shift(array,direction_of_neighbor)

#     last_game_info_lines_mask = game_info_lines_mask * (False == slided_game_info_lines_mask)
#     slided_last_game_info_lines_mask = shift(last_game_info_lines_mask, -direction)
