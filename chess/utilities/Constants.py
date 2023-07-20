[
    WHITE_KNIGHT_PIECE_ID,
    WHITE_BISHOP_PIECE_ID,
    WHITE_ROOK_PIECE_ID,
    WHITE_QUEEN_PIECE_ID,
    WHITE_KING_PIECE_ID,
    WHITE_PAWN_PIECE_ID,
    EMPTY_SQUARE_ID,
] = [6, 5, 4, 3, 2, 7, 0]

BLACK_PIECE_ID_OFFSET = 6
UNICODE_OFFSET = 9810
WHITE_SPACE_UNICODE = 32

LETTER_TO_PIECE_TYPE = {
    "N": "knight",
    "B": "bishop",
    "Q": "queen",
    "K": "king",
    "R": "rook",
}
BACKGROUND_COLOR_TO_ANSII = {
    "black": "40",
    "red": "41",
    "green": "42",
    "yellow": "43",
    "blue": "44",
    "magenta": "45",
    "cyan": "46",
    "white": "47",
}

FOREGROUND_COLOR_TO_ANSII = {
    "black": "30",
    "red": "31",
    "green": "32",
    "yellow": "33",
    "blue": "34",
    "magenta": "35",
    "cyan": "36",
    "white": "37",
}
FEN_PIECE_CODE_TO_PIECE_TYPE_DICTIONARY = {
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