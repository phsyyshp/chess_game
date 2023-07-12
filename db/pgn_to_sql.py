import sqlite3

# sqliteConnection = sqlite3.connect("master_games.db")
# cursor = sqliteConnection.cursor()


def clean_game(current_game: list):
    current_game = " ".join(current_game)
    current_game = current_game.replace("\n", " ")
    current_game = [current_game]
    return current_game


def pgn_to_sql(pgn_file):
    """
    Converts a pgn file to a sql file.
    """
    with open(pgn_file, "r") as f:
        listed_pgn = f.readlines()
    while len(listed_pgn) > 150:
        if listed_pgn[listed_pgn.index("\n") + 1] == "\n":
            listed_pgn.pop(listed_pgn.index("\n") + 1)
        current_game_info = listed_pgn[: listed_pgn.index("\n")]
        current_game_info = [
            item.rsplit('"', 2)[1].replace("'", "") for item in current_game_info
        ]
        if len(current_game_info) == 9:
            current_game_info.append("")
        listed_pgn = listed_pgn[listed_pgn.index("\n") + 1 :]
        current_game = listed_pgn[: listed_pgn.index("\n")]

        current_game = clean_game(current_game)
        listed_pgn = listed_pgn[listed_pgn.index("\n") + 1 :]
        values = (
            "VALUES('"
            + "', '".join(current_game_info)
            + "', '"
            + current_game[0]
            + "')"
            + ";"
        )
        # print(values)
        sql_command = (
            """INSERT INTO games(event, site, date, round, white, black, result, WhiteElo, BlackElo, ECO, moves)\n"""
            + values
        )

        # cursor.execute(sql_command)
        # print(current_game_info + current_game)
    # sqliteConnection.commit()
    # sqliteConnection.close()


pgn_to_sql("PircOtherBlack3.pgn")
# pgn_to_sql("Adams.pgn")
