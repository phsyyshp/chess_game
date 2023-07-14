import sqlite3
import numpy as np
from dbutilities import *
import os


class SQLjobs:
    def __init__(self, database_name):
        self.database_name = database_name
        self.connection = None
        self.cursor = None

    def connect(self):
        self.connection = sqlite3.connect(self.database_name)
        self.cursor = self.connection.cursor()

    def close_commit(self):
        self.connection.commit()
        self.connection.close()


class PgnFile:
    def __init__(self, pgn_file) -> None:
        self.pgn_file = pgn_file

    def __enter__(self):
        self.file = open(self.pgn_file, "r", encoding="latin-1")
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.file.close()

    def read_lines(self, nol):
        return self.file.readlines(nol)

    def get_file(self):
        pass

    # def pgn_stuff(self):
    #     directory = "pgn_files"
    #     i = 0
    #     for file_name in os.listdir(directory):
    #         pgn_file = os.path.join(directory, file_name)
    #         print(
    #             "Conversion of {file_name:} is being done".format(file_name=file_name)
    #         )
    #         i += 1
    #         pgn_to_sql(pgn_file)
    #         total_number_of_filed = len(os.listdir(directory))
    #         percentage = i / total_number_of_filed * 100
    #         print("Conversion of {file_name:} is finished".format(file_name=file_name))
    #         print(
    #             "{percentage:3.1f}% ofSELECT COUNT(*) FROM games; all files is done.".format(
    #                 percentage=percentage
    #             )
    #         )
    #         print(
    #             "{i}|{total_number_of_filed}".format(
    #                 i=i, total_number_of_filed=total_number_of_filed
    #             )
    #         )
    #         print("------------------------------------------------------------")


class PgnParser:
    def __init__(self, pgn_lines: np.ndarray) -> None:
        self.pgn_lines = pgn_lines

    def _remove_double_empty_lines(self):
        blank_lines_mask = self.pgn_lines == "\n"
        # print(blank_lines_mask)
        blank_lines_mask_left_shifted = shift(blank_lines_mask, -1)
        double_blank_lines_mask = blank_lines_mask_left_shifted * blank_lines_mask
        self.pgn_lines = self.pgn_lines[0 == double_blank_lines_mask]

    def _add_missing_empty_lines(self, direction):
        game_info_lines_mask = vec_does_contain_bracket(self.pgn_lines)
        slided_game_info_lines_mask = shift(game_info_lines_mask, direction)
        last_game_info_lines_mask = game_info_lines_mask * (
            False == slided_game_info_lines_mask
        )
        slided_last_game_info_lines_mask = shift(last_game_info_lines_mask, -direction)
        missing_line_mask = (self.pgn_lines != "\n") * slided_last_game_info_lines_mask
        slided_missing_line_mask = shift(missing_line_mask, direction)
        indices = np.nonzero(slided_missing_line_mask == True)[0].tolist()
        if len(indices) != 0:
            self.pgn_lines = np.insert(self.pgn_lines, indices, "\n")

    def _rtrim(self):
        string_pgn = "".join(self.pgn_lines)
        string_pgn = string_pgn.rsplit("SEPERATOR", 1)[0]
        self.pgn_lines = np.array(string_pgn.split("SEPERATOR"))
        self.pgn_lines = np.append(self.pgn_lines, "SEPERATOR")

    def _replace_blank_lines_with_seperator(self):
        blank_lines_mask = self.pgn_lines == "\n"
        self.pgn_lines[blank_lines_mask] = "SEPERATOR"

    def _format_each_line(self):
        tidying_lambda = lambda item: extract_str_between_apost(
            remove_line_breaks(item)
        )
        vectorized_format_each_line = np.vectorize(tidying_lambda)
        self.pgn_lines = vectorized_format_each_line(self.pgn_lines)

    def parse(self):
        self._remove_double_empty_lines()
        self._add_missing_empty_lines(-1)
        self._add_missing_empty_lines(1)
        self._replace_blank_lines_with_seperator()
        self._format_each_line()
        # self._ltrim()
        self._rtrim()
        return self.pgn_lines


class PgnToSQL:
    def __init__(self, cursor: sqlite3.Cursor, table_name) -> None:
        self.cursor = cursor
        self.table_name = table_name

    def generate_sql_command_reg(self, game_info: str, move):
        game_info_list = game_info.split("$")

        values = (
            "VALUES('" + "', '".join(game_info_list[:10]) + "', '" + move + "')" + ";"
        )
        # print(game_info_list)
        sql_command = (
            """INSERT INTO games(event, site, date, round, white, black, result, WhiteElo, BlackElo, ECO, moves)\n"""
            + values
        )
        self.cursor.execute(sql_command)

    def generate_sql_command(self, game_info: str, move):
        game_info_list = game_info.split("$")
        values = (
            "VALUES('" + "', '".join(game_info_list[:15]) + "', '" + move + "')" + ";"
        )
        sql_command = (
            """INSERT INTO {table_name:}(event, site, white, black, result, utcdate, utctime, WhiteElo, BlackElo, whiteRatingDiff, blackRatingDiff, ECO, opening, timeControl, termination, moves)\n""".format(
                table_name=self.table_name
            )
            + values
        )
        self.cursor.execute(sql_command)

    def pgn_to_sql(self, pgn_file):
        with PgnFile(pgn_file) as pgn:
            pgn_lines = pgn.read_lines(100000)

        pgn_lines = np.array(pgn_lines)
        pgn_lines = PgnParser(pgn_lines).parse()
        upper_bound_slice = (len(pgn_lines) // 2) * 2
        game_info_array = pgn_lines[0 : upper_bound_slice - 10 : 2]
        moves = pgn_lines[1 : upper_bound_slice - 9 : 2]
        vec_generate_sql_command = np.vectorize(self.generate_sql_command)

        # print(game_info_array)
        # print(moves)
        vec_generate_sql_command(game_info_array, moves)


# pgn_to_sql(pgn_file="pgn_files/KIDOther56.pgn")
def main():
    sqlhandler = SQLjobs("master_games.db")
    sqlhandler.connect()
    PgnToSQL(sqlhandler.cursor, "lichess_games").pgn_to_sql("pgn_files/lichess.pgn")
    # pgn_to_sql("pgn_files/lichess.pgn")
    sqlhandler.close_commit()


main()
