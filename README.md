# A UCI Chess Engine

## Features
- UCI protocol
- Magic bitboards
- Iterative deepening
- Alpha-beta pruning
- Quiescence search
- Transposition table
- Move ordering
- Killer move heuristic
- Zobrist hashing
- Mate distance pruning

## Latest Stable Release
v0.3.0-alpha

## Installing and Building
- On Linux, you can download the source files and  build the engine by running the following commands:
```
git clone --depth 1 https://github.com/phsyyshp/chess_game
cd chess_game
mkdir build
cd build
cmake ..
make
```
## Running the Engine
After building the engine, you can run it by executing the following command:
```cd ..
cd bin
./engine_0_3_0_alpha
```

## Using the Engine
While engine is running you can use it with UCI commands. Here is an example:
```
uci
isready
ucinewgame
position startpos moves e2e4 e7e5
go wtime 8080 btime 8080 winc 80 binc 80
```
After this set of commands engine will start calculating the best move for the given position, whitin the given time limits. 

Alternatively, you can use the engine with a third party GUI.
## Demo
Here is a demo of the engine playing against itself,
on cutechess gui.
[Demo](https://www.github.com/phsyyshp/chess_game/ChessEngineDemo.mp4)






