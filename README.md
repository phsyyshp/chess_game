# chess_game

chess_game is a fast C++ powered chess engine with a Python API. The engine is still under early development, so it is not recommended for use yet. 

## Features

- Very fast PGN parsing and FEN converting
  - Includes an SQLlite database for storing large opening books.
  - Python API for accessing and modifying the database.
  - Bulk PGN to JSON conversion.
  - Bulk JSON to SQL conversion.
- Very fast move generation in given depth.
- Very fast move validation.
- Understands, SAN, LAN, and UCI move parsing.
- Python API for accessing the engine.
- Neural network  tools for training and testing.
- Several visualization tools.
  - Terminal board printing.
 

More features are planned for future development.

Overall, chess_game aims to provide a high-performance chess engine with a user-friendly Python API.

## TODO
- [ ] Add more tests to ensure the engine is functioning correctly and efficiently.
- [ ] Add more documentation to make it easier for users to understand and use the engine and API.
- [ ] Add more visualization tools to help users visualize the game and analyze it more effectively.
- [ ] Add more neural network tools to improve the engine's performance and accuracy.
- [ ] Add more features to the engine, such as support for different game modes or rule sets.
- [ ] Add more features to the Python API, such as support for additional data formats or analysis tools.
