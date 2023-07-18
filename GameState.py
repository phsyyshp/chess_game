from utilities.BoardUtilities import *
import numpy as np
import Piece as pc
import Board as bd
import Move as mv


class GameState:
    def __init__(self, board: bd.Board):
        self.board = board

    def get_legal_moves(
        self,
        piece_object: pc.Pawn | pc.King | pc.Bishop | pc.Queen | pc.Knight | pc.Rook,
    ):
        moves_mask = piece_object.squares_in_range_mask()
        moves_row_column = mask_to_row_column(moves_mask)
        legal_moves = np.array(list(map(
            lambda x: mv.Move(piece_object.position_row_column, x, self.board).is_legal(), moves_row_column
        )))
        return moves_row_column[legal_moves]

    def get_all_legal_moves(self, color):
        positions_of_pieces = self.board.get_all_same_color_piece_positions(color)
        legal_moves = []
        for position_of_piece in positions_of_pieces:
            legal_moves += self.get_legal_moves(
                self.board.get_piece_object_from_position(position_of_piece)
            )
        return legal_moves

    def can_king_be_saved(self):
        # TODO there are jobs to do here.
        all_possible_moves = self.get_legal_moves()

        pass

    def is_check_mate(self):
        # if not self.is_check():
        # return False
        # return self.can_king_be_saved()
        return False
        # TODO lots to do here

    def is_stale_mate(self):
        return False
