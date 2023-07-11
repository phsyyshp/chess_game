from utilities.Constants import *


def piece_id_to_color(piece_id):
    color = "white" if piece_id <= 7 else "black"
    return color


def piece_id_to_type_color(piece_id):
    color = "white" if piece_id <= 7 else "black"
    normalized_piece_id = piece_id - (color == "black") * BLACK_PIECE_ID_OFFSET
    if normalized_piece_id == WHITE_PAWN_PIECE_ID:
        return "pawn", color
    if normalized_piece_id == WHITE_KNIGHT_PIECE_ID:
        return "knight", color
    if normalized_piece_id == WHITE_ROOK_PIECE_ID:
        return "rook", color
    if normalized_piece_id == WHITE_QUEEN_PIECE_ID:
        return "queen", color
    if normalized_piece_id == WHITE_KING_PIECE_ID:
        return "king", color
    if normalized_piece_id == WHITE_BISHOP_PIECE_ID:
        return "bishop", color
    if normalized_piece_id == EMPTY_SQUARE_ID:
        return "empty", color


def piece_type_to_id(piece_type, color):
    piece_ids = {
        "pawn": WHITE_PAWN_PIECE_ID,
        "knight": WHITE_KNIGHT_PIECE_ID,
        "rook": WHITE_ROOK_PIECE_ID,
        "queen": WHITE_QUEEN_PIECE_ID,
        "king": WHITE_KING_PIECE_ID,
        "bishop": WHITE_BISHOP_PIECE_ID,
        "empty": EMPTY_SQUARE_ID,
    }

    offset = 0 if color == "white" else BLACK_PIECE_ID_OFFSET
    return piece_ids.get(piece_type, EMPTY_SQUARE_ID) + offset


def fen_piece_letter_to_piece_id(fen_piece_code):
    fen_piece_code_to_piece_type_dictionary = {
        "P": WHITE_PAWN_PIECE_ID,
        "N": WHITE_KNIGHT_PIECE_ID,
        "B": WHITE_BISHOP_PIECE_ID,
        "R": WHITE_ROOK_PIECE_ID,
        "Q": WHITE_QUEEN_PIECE_ID,
        "K": WHITE_KING_PIECE_ID,
        "p": WHITE_PAWN_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "n": WHITE_KNIGHT_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "b": WHITE_BISHOP_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "r": WHITE_ROOK_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "q": WHITE_QUEEN_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "k": WHITE_KING_PIECE_ID + BLACK_PIECE_ID_OFFSET,
        "e": EMPTY_SQUARE_ID,
    }
    return fen_piece_code_to_piece_type_dictionary[fen_piece_code]
