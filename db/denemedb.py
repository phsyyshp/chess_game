import pandas as pd

import sqlite3

# load data from database to dataframe
conn = sqlite3.connect("master_games.db")
df = pd.read_sql_query("SELECT * FROM games", conn)
df.head()
#
