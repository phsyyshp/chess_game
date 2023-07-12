import sqlite3
import numpy as np

sqliteConnection = sqlite3.connect("master_games.db")
cursor = sqliteConnection.cursor()


def remove_line_breaks(current_game: str):
    current_game = current_game.replace("\n", " ")
    return current_game


def extract_str_between_apost(field: str):
    if "[" in field:
        return field.rsplit('"', 2)[1].replace("'", "") + "$"
    return field


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
    # print(game_info)

    sql_command = (
        """INSERT INTO games(event, site, date, round, white, black, result, WhiteElo, BlackElo, ECO, moves)\n"""
        + values
    )
    cursor.execute(sql_command)


def pgn_to_sql(pgn_file):
    """
    Converts a pgn file to a sql file.
    """
    with open(pgn_file, "r") as f:
        listed_pgn = f.readlines()
    arrayed_pgn = np.array(listed_pgn)
    arrayed_pgn = remove_double_empty_lines(arrayed_pgn)
    arrayed_pgn = replace_game_seperator(arrayed_pgn)
    vec_tidy_up = np.vectorize(tidy_up)
    arrayed_pgn = vec_tidy_up(arrayed_pgn)
    string_pgn = "".join(arrayed_pgn)
    arrayed_pgn = np.array(string_pgn.split("SEPERATOR"))
    upper_bound_slice = (len(arrayed_pgn) // 2) * 2
    game_info_array = arrayed_pgn[0 : upper_bound_slice - 10 : 2]
    moves = arrayed_pgn[1 : upper_bound_slice - 9 : 2]
    vec_get_sql_command = np.vectorize(get_sql_command)

    vec_get_sql_command(game_info_array, moves)


pgn_to_sql("SemiSlavMeran.pgn")

sqliteConnection.commit()
sqliteConnection.close()
