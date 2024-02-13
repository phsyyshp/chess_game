
#!/bin/bash

cutechess-cli \
-engine cmd=/home/oturker/chess_game/chess_game_cpp/bin/engine \
-engine cmd=/home/oturker/chess_game/chess_game_cpp/bin/engine_1 \
-each \
    tc=inf/40+0.08 \
    proto=uci \
-games 2 -rounds 2000 -repeat 2 -maxmoves 200 \
-sprt elo0=0 elo1=5 alpha=0.05 beta=0.05 \
-concurrency 1 \
-ratinginterval 5 \
-pgnout "sprt.pgn" \
-debug > "/home/oturker/chess_game/chess_game_cpp/debug_output.txt" 2>&1


