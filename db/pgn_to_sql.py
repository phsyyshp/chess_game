import sqlite3
import numpy as np
import os

sqliteConnection = sqlite3.connect("master_games.db")
cursor = sqliteConnection.cursor()


def remove_line_breaks(current_game: str):
    current_game = current_game.replace("\n", " ")
    return current_game


def extract_str_between_apost(field: str):
    # print(field)
    if '"' in field:
        return field.rsplit('"', 2)[1].replace("'", "") + "$"
    return field


def is_contain_bracket(field: str):
    return "[" in field


vec_does_contain_bracket = np.vectorize(is_contain_bracket)


def add_blank_lines_if_missing(arrayed_pgn):
    bracket_mask = vec_does_contain_bracket(arrayed_pgn)
    slided_bracket_mask = np.append(bracket_mask[1:], False)
    last_bracket_mask = bracket_mask * (False == slided_bracket_mask)
    # add False in front of array
    slided_last_bracket_mask = np.append(False, last_bracket_mask[:-1])
    missing_line_mask = (arrayed_pgn != "\n") * slided_last_bracket_mask

    slided_missing_line_mask = np.append(missing_line_mask[1:], False)
    indices = np.nonzero(slided_missing_line_mask == True)[0].tolist()
    if len(indices) == 0:
        return arrayed_pgn
    return np.insert(arrayed_pgn, indices, "\n")
    # arrayed_pgn[slided_missing_line_mask ] = "\n"


def add_blank_lines_if_missing_ab(arrayed_pgn):
    bracket_mask = vec_does_contain_bracket(arrayed_pgn)
    slided_bracket_mask = np.append(False, bracket_mask[:-1])
    last_bracket_mask = bracket_mask * (False == slided_bracket_mask)
    # add False in front of array
    slided_last_bracket_mask = np.append(last_bracket_mask[1:], False)
    missing_line_mask = (arrayed_pgn != "\n") * slided_last_bracket_mask

    slided_missing_line_mask = np.append(False, missing_line_mask[:-1])
    indices = np.nonzero(slided_missing_line_mask == True)[0].tolist()
    # print(indices)
    if len(indices) == 0:
        return arrayed_pgn
    return np.insert(arrayed_pgn, indices, "\n")
    # arrayed_pgn[slided_missing_line_mask ] = "\n"


def tidy_up(item):
    item = remove_line_breaks(item)
    return extract_str_between_apost(item)


def remove_double_empty_lines(arrayed_pgn):
    empty_line_mask = arrayed_pgn == "\n"
    empty_line_mask_slided = np.append(empty_line_mask[1:], False)
    double_empty_line_mask = empty_line_mask_slided * empty_line_mask
    return arrayed_pgn[0 == double_empty_line_mask]


def replace_game_seperator(arrayed_pgn):
    empty_line_mask = arrayed_pgn == "\n"
    arrayed_pgn[empty_line_mask] = "SEPERATOR"
    return arrayed_pgn


def get_sql_command(game_info: str, move):
    game_info_list = game_info.split("$")

    values = "VALUES('" + "', '".join(game_info_list[:10]) + "', '" + move + "')" + ";"
    # print(game_info_list)
    sql_command = (
        """INSERT INTO games(event, site, date, round, white, black, result, WhiteElo, BlackElo, ECO, moves)\n"""
        + values
    )
    cursor.execute(sql_command)


def get_sql_command_lichess(game_info: str, move):
    game_info_list = game_info.split("$")

    # print(game_info_list)
    values = "VALUES('" + "', '".join(game_info_list[:15]) + "', '" + move + "')" + ";"
    # print(game_info)
    # print(move)
    sql_command = (
        """INSERT INTO lichess_games(event, site, white, black, result, utcdate, utctime, WhiteElo, BlackElo, whiteRatingDiff, blackRatingDiff, ECO, opening, timeControl, termination, moves)\n"""
        + values
    )
    cursor.execute(sql_command)


def pgn_to_sql(pgn_file):
    """
    Converts a pgn file to a sql file.
    """
    with open(pgn_file, "r", encoding="latin-1") as f:
        listed_pgn_all = f.readlines()
    print(len(listed_pgn_all))
    for i in range(500):
        # partition listed pgn into 10 parts and create listed_pgn for i th partition
        listed_pgn = listed_pgn_all[
            i * len(listed_pgn_all) // 500 : (i + 1) * len(listed_pgn_all) // 500
        ]

        arrayed_pgn = np.array(listed_pgn, dtype=str)
        arrayed_pgn = remove_double_empty_lines(arrayed_pgn)
        arrayed_pgn = add_blank_lines_if_missing(arrayed_pgn)
        arrayed_pgn = add_blank_lines_if_missing_ab(arrayed_pgn)
        arrayed_pgn = replace_game_seperator(arrayed_pgn)

        vec_tidy_up = np.vectorize(tidy_up)
        # print(arrayed_pgn)
        arrayed_pgn = vec_tidy_up(arrayed_pgn)
        string_pgn = "".join(arrayed_pgn)
        string_pgn = string_pgn.rsplit("SEPERATOR", 1)[0]
        arrayed_pgn = np.array(string_pgn.split("SEPERATOR"))
        arrayed_pgn = np.append(arrayed_pgn, "SEPERATOR")
        upper_bound_slice = (len(arrayed_pgn) // 2) * 2
        game_info_array = arrayed_pgn[0 : upper_bound_slice - 10 : 2]
        moves = arrayed_pgn[1 : upper_bound_slice - 9 : 2]
        vec_get_sql_command = np.vectorize(get_sql_command_lichess)

        # print(game_info_array)
        # print(moves)
        vec_get_sql_command(game_info_array, moves)


# directory = "pgn_files"
# i = 0
# for file_name in os.listdir(directory):
#     pgn_file = os.path.join(directory, file_name)
#     print("Conversion of {file_name:} is being done".format(file_name=file_name))
#     i += 1
#     pgn_to_sql(pgn_file)
#     total_number_of_filed = len(os.listdir(directory))
#     percentage = i / total_number_of_filed * 100
#     print("Conversion of {file_name:} is finished".format(file_name=file_name))
#     print(
#         "{percentage:3.1f}% ofSELECT COUNT(*) FROM games; all files is done.".format(
#             percentage=percentage
#         )
#     )
#     print(
#         "{i}|{total_number_of_filed}".format(
#             i=i, total_number_of_filed=total_number_of_filed
#         )
#     )
#     print("------------------------------------------------------------")


# pgn_to_sql(pgn_file="pgn_files/KIDOther56.pgn")

pgn_to_sql("pgn_files/lichess.pgn")
sqliteConnection.commit()
sqliteConnection.close()
