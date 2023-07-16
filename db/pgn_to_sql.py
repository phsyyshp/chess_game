import os
import sqlite3
import pandas as pd
import tqdm
from dbutilities import units


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

    def read(self, size):
        return self.file.read(size)

    def size(self):
        return os.path.getsize(self.pgn_file_name)


class PgnPartioner:
    def __init__(self, pgn_file_object: PgnFile) -> None:
        self.pgn_file_object = pgn_file_object

    def read_chunk(self, chunk_size=100_000_000):
        while True:
            pgn_str_chunked = self.pgn_file_object.read(chunk_size)
            if not pgn_str_chunked:
                break
            yield pgn_str_chunked


class PgnConverter:
    def __init__(self, pgn_str: str) -> None:
        self.pgn_str = pgn_str

    def _replace(self, old, new):
        self.pgn_str = self.pgn_str.replace(old, new)

    def to_json(self):
        self._replace(' "]', '"]')
        self._replace("\n\n", "SEPARATOR")
        self._replace("]SEPARATOR", '][moves "')
        self._replace("SEPARATOR[", "\"}, {\"")
        self._replace(' "', '": "')
        self._replace("\n", "")
        self._replace('"][', '", "')
        self.pgn_str = self.pgn_str.rsplit('}, {"Event', 1)[0]
        # print(self.pgn_str)
        self.pgn_str = self.pgn_str.split('}, {"Event"', 1)[1]
        self.pgn_str = '[{"Event"' + self.pgn_str + "}]"
        return self.pgn_str

    def to_df(self) -> pd.DataFrame:
        return pd.read_json(self.to_json())

    def to_sql(self, table_name, sql_handler: SQLjobs):
        self.to_df().to_sql(
            table_name, sql_handler.connection, if_exists="append", index=False
        )


class MultiPgnConverter:
    def __init__(self, path_to_pgn_files, sql_handler: SQLjobs, table_name) -> None:
        self.path_to_pgn_files = path_to_pgn_files
        self.sql_handler = sql_handler
        self.table_name = table_name

    def single_pgn_uploader(self, path_to_pgn_file, chunk_size=100_000_000):
        with PgnFile(path_to_pgn_file) as pgn:
            total_chunks = pgn.size() // chunk_size
            pbar = tqdm.tqdm(total=total_chunks, postfix={"Uploaded": "0/0"})

            for counter, chunks in enumerate(PgnPartioner(pgn).read_chunk(chunk_size)):
                PgnConverter(chunks).to_sql(self.table_name, self.sql_handler)

                uploaded = units(counter * chunk_size)
                total_size = units(pgn.size())
                pbar.set_postfix({"Uploaded": f"{uploaded}/{total_size}"})
                pbar.update(1)

            # self.sql_handler.commit()

        pbar.close()


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
    MultiPgnConverter("pgn_files", sql_handler, "lichess_games").single_pgn_uploader(
        "pgn_files/lichess.pgn"
    )
if __name__ == "__main__":
    main()