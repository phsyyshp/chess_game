from utilities.Constants import *
import Board as bd
import Move as mv
import Piece as pc


def pgn_to_list(pgn: str):
    pgn = pgn.replace("+", "").replace("\n", " ")
    list_of_moves = pgn.split(" ")
    # print(pgn)
    print(list_of_moves)
    return list_of_moves


def get_piece_type(pgn_move: str):
    first_letter_of_move = pgn_move[0]
    if first_letter_of_move.islower():
        return "pawn"
    return LETTER_TO_PIECE_TYPE[first_letter_of_move]


def pgn_to_uci(pgn_move: str, board: bd.Board):
    # board.turn = "w" if pgn_move.find(".") != -1 else "b"
    # handle castling
    if pgn_move == "O-O":
        if board.turn == "white":
            return "e1g1"
        else:
            return "e8g8"
    elif pgn_move == "O-O-O":
        if board.turn == "white":
            return "e1c1"
        else:
            return "e8c8"

    # handle normal moves
    piece_type = get_piece_type(pgn_move)

    destination_file_rank = pgn_move[-2:]
    dummy_piece = pc.Piece(destination_file_rank, board.turn, piece_type)
    if not piece_type == "pawn":
        dummy_piece.is_in_range()
    is_capture = "x" in pgn_move
    if is_capture:
        pgn_move = pgn_move.replace("x", "")

    if piece_type:
        for i in range(8):
            for j in range(8):
                piece = board.get_piece_object_from_position([i, j])
                if piece and piece.color == board.turn and piece.type == piece_type:
                    uci_move = mv.Move(
                        [i, j], [8 - int(to_square[1]), ord(to_square[0]) - 97], board
                    )
                    if uci_move.is_legal():
                        return f"{chr(j+97)}{8-i}{to_square}"

    else:  # pawn move
        file = ord(to_square[0]) - 97
        rank = 8 - int(to_square[1])
        if board.turn == "white":
            uci_move = mv.Move([rank + 1, file], [rank, file], board)
            if not uci_move.is_legal():
                uci_move = mv.Move([rank + 2, file], [rank, file], board)
        else:
            uci_move = mv.Move([rank - 1, file], [rank, file], board)
            if not uci_move.is_legal():
                uci_move = mv.Move([rank - 2, file], [rank, file], board)
        if uci_move.is_legal():
            return f"{chr(file+97)}{8-to_square[0]}{to_square}"

    raise ValueError(f"Invalid PGN move: {pgn_move}")


# refactor above into dictionary


# def single_destination_to_source(pgn_move: str):
#     is_white = pgn_move.find(".") != -1
#     if is_white:
#         white_move = str(pgn_move.split(".")[-1])
#         move_type = get_move_type(white_move)
#         return white_move


pgn = """1.e4 c6 2.d4 d5 3.exd5 cxd5 4.c4 Nf6 5.Nc3 Nc6 6.Bg5 Qa5 7.Bxf6 exf6 8.cxd5 Bb4
9.Qd2 Bxc3 10.bxc3 Qxd5 11.Ne2 O-O 12.Nf4 Qa5 13.Bd3 Re8+ 14.Ne2 Bg4 15.f3 Bf5
16.O-O Rad8 17.Bxf5 Qxf5 18.Ng3 Qa5 19.Rfe1 Rxe1+ 20.Rxe1 h5 21.Nf1 g6 22.Rb1 b6
23.Qc2 Rc8 24.Qb3 Qf5 25.Ne3 Qg5 26.Nc4 Qf4 27.Re1 Qc7 28.d5 Nb8 29.Nd2 Qc5+
30.Kf1 f5 31.c4 Nd7 32.Qe3 Qc7 33.Qe7 Qd8 34.Qxd8+ Rxd8 35.Nb3 Kf8 36.Nd4 Nb8
37.Ke2 Re8+ 38.Kd2 Rc8 39.Kd3 Rc7 40.Nb5 Rd7 41.Nd4 Rc7 42.Re2 Rd7 43.Kc3 Re7
44.Rxe7 Kxe7 45.Kb4 Kd7 46.a3 Na6+ 47.Kc3 Nc5 48.Nb3 Na6 49.a4 Nc7 50.Kd4 Kd6
51.a5 Na6 52.axb6 axb6 53.Nc1 Nb4 54.Kc3 Na6 55.Kd4 Nb4 56.Kc3 Na6 57.Nd3 Nb8
58.Kb4 Nd7 59.Kb5 Kc7 60.f4 Kd6 61.Ne5 Nxe5 62.fxe5+ Kc7 63.e6 fxe6 64.dxe6 f4
65.e7 Kd7 66.Kxb6 h4 67.h3 Kxe7 68.c5 g5 69.c6 g4 70.c7  1-0"""
pgn_to_list(pgn)
board = bd.Board()
board.set_board_to_initial_configuration()
print(pgn_to_uci("e4", board))
# print(single_destination_to_source(pgn_to_list(pgn)[0]))
