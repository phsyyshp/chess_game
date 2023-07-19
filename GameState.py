from utilities.BoardUtilities import *
import numpy as np
import Piece as pc
import Board as bd
import Move as mv
import cProfile

class GameState:
    def __init__(self, board: bd.Board):
        self.board = board

    def get_legal_moves(
        self,
        piece_object: pc.Pawn | pc.King | pc.Bishop | pc.Queen | pc.Knight | pc.Rook,
    ):
        moves_mask = piece_object.squares_in_range_mask()
        moves_row_column = mask_to_row_column(moves_mask)
        legal_moves_mask = np.array(list(map(
            lambda x: mv.Move(piece_object.position_row_column, x, self.board).is_legal(), moves_row_column
        )))
        return moves_row_column[legal_moves_mask]

    def get_all_legal_moves(self, color):
        positions_of_pieces = self.board.get_all_same_color_piece_positions(color)
        legal_moves =  (list(map(
            lambda x: self.get_legal_moves(self.board.get_piece_object_from_position(x)), positions_of_pieces
        )))
        #remove empty arrays
        # legal_moves = list(filter(lambda x: len(x) > 0, legal_moves))
        return  positions_of_pieces.tolist(), legal_moves

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


bb = bd.Board()
bb.set_board_to_initial_configuration()
gs = GameState(bb)
#benchmark
import timeit 

# print(gs.get_all_legal_moves("white")[0])
# print(gs.get_all_legal_moves("white")[1])
 
# print(len(gs.get_all_legal_moves("white")[1]))
# print(len(gs.get_all_legal_moves("white")[0]))
def dd():
    gs.get_all_legal_moves("white")
cProfile.run("dd()")
def a():
    1
execution_time = timeit.timeit( a, number=1_000_000_000)
print(execution_time)
