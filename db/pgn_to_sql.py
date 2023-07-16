import os
import sqlite3

import numpy as np
from dbutilities import *

# import a module which lets me to show a progress bar
import tqdm
import time


class SQLjobs:
    def __init__(self, database_name):
        self.database_name = database_name
        self.connection = None
        self.cursor = None

    def connect(self):
        self.connection = sqlite3.connect(self.database_name)
        self.cursor = self.connection.cursor()

    def begin_transaction(self):
        self.cursor.execute("BEGIN TRANSACTION;")

    def commit(self):
        self.connection.commit()

    def close(self):
        self.connection.close()


class PgnFile:
    def __init__(self, pgn_file_name) -> None:
        self.pgn_file_name = pgn_file_name

    def __enter__(self):
        self.file = open(self.pgn_file_name, "r", encoding="latin-1")
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.file.close()

    def read_lines(self, number_of_lines):
        return self.file.readlines(number_of_lines)

    def size(self):
        return os.path.getsize(self.pgn_file_name)


class PgnPartioner:
    def __init__(self, pgn_file_object: PgnFile) -> None:
        self.pgn_file_object = pgn_file_object

    def _is_partition_needed(self, max_allowed_size=40_000_000):
        return self.pgn_file_object.size() >= max_allowed_size

    def read_chunk(self, chunk_size=40_000_000):
        while True:
            pgn_lines_chunked = self.pgn_file_object.read_lines(chunk_size)
            if not pgn_lines_chunked:
                break
            yield pgn_lines_chunked


class PgnParser:
    def __init__(self, pgn_lines: np.ndarray) -> None:
        self.pgn_lines = pgn_lines

    def _remove_double_empty_lines(self):
        blank_lines_mask = self.pgn_lines == "\n"
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
        first_empty_line_index = get_first_occurence_index(self.pgn_lines, "SEPERATOR")
        second_empty_line_index = get_next_to_first_occurence_index(
            self.pgn_lines, "SEPERATOR"
        )
        is_game_info_line = is_contain_bracket(
            self.pgn_lines[first_empty_line_index + 1]
        )
        if is_game_info_line:
            return
        else:
            self.pgn_lines = self.pgn_lines[second_empty_line_index + 1 :]

    def _ltrim(self):
        does_begin_with_event = "[event" in self.pgn_lines[0].lower()
        while not does_begin_with_event:
            self.pgn_lines = self.pgn_lines[1:]
            does_begin_with_event = "[event" in self.pgn_lines[0].lower()

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
        self._ltrim()
        self._format_each_line()
        return self.pgn_lines


class PgnToSQL:
    def __init__(self, cursor: sqlite3.Cursor, table_name) -> None:
        self.cursor = cursor
        self.table_name = table_name

    def generate_sql_command(self, game_info: str, move):
        game_info_list = game_info.split("$")
        game_info_dict = game_info_to_dic(game_info_list[:-1])
        game_info_dict["moves"] = move
        template_dict = {
            "Event": "",
            "Site": "",
            "Date": "",
            "Round": "",
            "White": "",
            "Black": "",
            "Result": "",
            "UTCDate": "",
            "UTCTime": "",
            "WhiteElo": "",
            "BlackElo": "",
            "WhiteRatingDiff": "",
            "BlackRatingDiff": "",
            "WhiteTitle": "",
            "BlackTitle": "",
            "ECO": "",
            "Opening": "",
            "TimeControl": "",
            "Termination": "",
            "moves": "",
        }
        template_dict.update(game_info_dict)
        values = "VALUES('{Event}', '{Site}', '{Date}', '{Round}', '{White}', '{Black}', '{Result}', '{UTCDate}', '{UTCTime}', '{WhiteElo}', '{BlackElo}', '{WhiteRatingDiff}', '{BlackRatingDiff}', '{WhiteTitle}', '{BlackTitle}', '{ECO}', '{Opening}', '{TimeControl}', '{Termination}', '{moves}');".format(
            **template_dict
        )
        sql_command = (
            """INSERT INTO {table_name:}(Event, Site, Date, Round, White, Black, Result, UTCDate, UTCTime, WhiteElo, BlackElo, WhiteRatingDiff, BlackRatingDiff, WhiteTitle, BlackTitle, ECO, Opening, TimeControl, Termination, moves)\n""".format(
                table_name=self.table_name
            )
            + values
        )
        self.cursor.execute(sql_command)

    def split_to_info_moves(self, pgn_lines):
        string_pgn = "".join(pgn_lines)
        pgn_lines = np.array(string_pgn.split("SEPERATOR"))
        upper_bound_slice = (len(pgn_lines) // 2) * 2
        game_info_array = pgn_lines[0 : upper_bound_slice - 100 : 2]
        moves = pgn_lines[1 : upper_bound_slice - 99 : 2]
        return game_info_array, moves

    def pgn_to_sql(self, pgn_lines):
        pgn_lines = split_long_lines_numba(pgn_lines, 90)
        pgn_lines = np.array(pgn_lines, dtype="U")
        pgn_lines = PgnParser(pgn_lines).parse()
        game_info_array, moves = self.split_to_info_moves(pgn_lines)
        vec_generate_sql_command = np.vectorize(self.generate_sql_command)
        vec_generate_sql_command(game_info_array, moves)


class MultiPgnToSQL:
    def __init__(self, path_to_pgn_files, sql_handler: SQLjobs, table_name) -> None:
        self.path_to_pgn_files = path_to_pgn_files
        self.sql_handler = sql_handler
        self.table_name = table_name

    def single_pgn_uploader(self, path_to_pgn_file, chunk_size=100_000):
        with PgnFile(path_to_pgn_file) as pgn:
            counter = 0
            total_chunks = pgn.size() // chunk_size
            with tqdm.tqdm(total=total_chunks) as pbar:
                for chunks in PgnPartioner(pgn).read_chunk(chunk_size):
                    PgnToSQL(self.sql_handler.cursor, self.table_name).pgn_to_sql(
                        chunks
                    )

                    pbar.set_postfix(
                        {
                            "Uploaded": file_size_byte_to_pretty(counter * chunk_size)
                            + "/"
                            + file_size_byte_to_pretty(pgn.size())
                        }
                    )
                    pbar.update(1)

                    counter += 1
                    self.sql_handler.commit()

    def multi_pgn_uploader(self):
        directory = self.path_to_pgn_files
        total = len(os.listdir(directory))
        with tqdm.tqdm(total=total) as pbar:
            for file_name in os.listdir(directory):
                pgn_file_name = os.path.join(directory, file_name)
                pbar.set_postfix(
                    {"Uploading": "{file_name:}".format(file_name=file_name)}
                )
                pbar.update(1)
                self.single_pgn_uploader(pgn_file_name)


def main():
    sql_handler = SQLjobs("master_games.db")
    sql_handler.connect()
    sql_handler.begin_transaction()
    MultiPgnToSQL("pgn_files", sql_handler, "lichess_games").multi_pgn_uploader()
    sql_handler.close()


# def main():
#     sql_handler = SQLjobs("master_games.db")
#     sql_handler.connect()
#     sql_handler.begin_transaction()
#     chunk_size = 40_000_000
#     with PgnFile("pgn_files/lichess.pgn") as pgn:
#         counter = 0
#         total_chunks = pgn.size() // chunk_size
#         print(" ----------------------------------")
#         print("| Uploading to database is started |")
#         print(" ----------------------------------")
#         with tqdm.tqdm(total=total_chunks) as pbar:
#             for chunks in PgnPartioner(pgn).read_chunk(chunk_size):
#                 PgnToSQL(sql_handler.cursor, "lichess_games").pgn_to_sql(chunks)

#                 pbar.set_postfix(
#                     {
#                         "Uploaded": "{:.1f}MB/{:.1f}GB".format(
#                             counter * chunk_size / 1_000_000, pgn.size() / 1_000_000_000
#                         )
#                     }
#                 )
#                 pbar.update(1)

#                 counter += 1
#                 sql_handler.commit()

#     sql_handler.close()


main()

# def pgn_stuff(self):
#     directory = "pgn_file_names"
#     i = 0
#     for file_name in os.listdir(directory):
#         pgn_file_name = os.path.join(directory, file_name)
#         print(
#             "Conversion of {file_name:} is being done".format(file_name=file_name)
#         )
#         i += 1
#         pgn_to_sql(pgn_file_name)
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
