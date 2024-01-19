with open("D:\db\pgn_files\lcgames.pgn", "r", encoding="latin-1") as f:
    new_pgn = f.read(100_000_000)
with open("new.txt", "w") as new:
    new.write(new_pgn)
