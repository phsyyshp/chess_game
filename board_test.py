import unittest
import numpy as np
from board import board


class TestBoard(unittest.TestCase):
    def setUp(self):
        self.board = board()

    def test_set_board_to_initial_configuration(self):
        self.board.set_board_to_initial_configuration()
        expected_board_matrix = np.array(
            [
                [4, 6, 5, 3, 2, 5, 6, 4],
                [7, 7, 7, 7, 7, 7, 7, 7],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [6 + 7, 6 + 7, 6 + 7, 6 + 7, 6 + 7, 6 + 7, 6 + 7, 6 + 7],
                [10, 12, 6 + 5, 6 + 3, 6 + 2, 6 + 5, 6 + 6, 6 + 4],
            ]
        )
        self.assertTrue(np.array_equal(self.board.board_matrix, expected_board_matrix))

    def test_get_piece_id_from_position(self):
        self.board.board_matrix = np.array(
            [
                [4, 6, 5, 3, 2, 5, 6, 4],
                [1, 1, 1, 1, 1, 1, 1, 1],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [7, 7, 7, 7, 7, 7, 7, 7],
                [4, 6, 5, 3, 2, 5, 6, 4],
            ]
        )
        self.assertEqual(self.board.get_piece_id_from_position((0, 0)), 4)
        self.assertEqual(self.board.get_piece_id_from_position((1, 1)), 1)
        self.assertEqual(self.board.get_piece_id_from_position((7, 7)), 4)

    def test_is_square_empty(self):
        self.board.board_matrix = np.array(
            [
                [4, 6, 5, 3, 2, 5, 6, 4],
                [1, 1, 1, 1, 1, 1, 1, 1],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [7, 7, 7, 7, 7, 7, 7, 7],
                [4, 6, 5, 3, 2, 5, 6, 4],
            ]
        )
        self.assertFalse(self.board.is_square_empty((0, 0)))
        self.assertFalse(self.board.is_square_empty((1, 1)))
        self.assertTrue(self.board.is_square_empty((2, 0)))

    def test_get_piece_object_from_position(self):
        self.board.board_matrix = np.array(
            [
                [4, 6, 5, 3, 2, 5, 6, 4],
                [1, 1, 1, 1, 1, 1, 1, 1],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [7, 7, 7, 7, 7, 7, 7, 7],
                [4, 6, 5, 3, 2, 5, 6, 4],
            ]
        )
        piece_object = self.board.get_piece_object_from_position((0, 0))
        self.assertEqual(piece_object.type(), "rook")
        self.assertEqual(piece_object.color(), "white")

    def test_is_destination_occupied_by_same_color(self):
        self.board.board_matrix = np.array(
            [
                [4, 6, 5, 3, 2, 5, 6, 4],
                [1, 1, 1, 1, 1, 1, 1, 1],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [7, 7, 7, 7, 7, 7, 7, 7],
                [10, 6, 5, 3, 2, 5, 6, 4],
            ]
        )
        piece_object = self.board.get_piece_object_from_position((0, 0))
        self.assertTrue(
            self.board.is_destination_occupied_by_same_color(piece_object, (1, 0))
        )
        self.assertFalse(
            self.board.is_destination_occupied_by_same_color(piece_object, (7, 0))
        )

    def test_is_pawn_path_clear(self):
        self.board.board_matrix = np.array(
            [
                [0, 6, 0, 3, 2, 5, 6, 4],
                [1, 0, 1, 1, 1, 1, 1, 1],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [7, 7, 7, 7, 7, 7, 7, 7],
                [4, 6, 5, 3, 2, 5, 6, 4],
            ]
        )
        piece_object = self.board.get_piece_object_from_position((1, 0))
        self.assertTrue(self.board.is_pawn_path_clear(piece_object, (3, 0)))
        self.assertFalse(self.board.is_pawn_path_clear(piece_object, (2, 0)))


# def test_is_path_clear(self):
#     self.board.board_matrix = np.array([
#         [4, 6, 5, 3, 2, 5, 6, 4],
#         [0, 1, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [7, 7, 7, 7, 7, 7, 7, 7],
#         [4, 6, 5, 3, 2, 5, 6, 4]
#     ])
#     piece_object = self.board.get_piece_object_from_position((0, 0))
#     self.assertTrue(self.board.is_path_clear(piece_object, (0, 3)))
#     self.assertFalse(self.board.is_path_clear(piece_object, (0, 4)))

# def test_is_under_attack_by_any_piece(self):
#     self.board.board_matrix = np.array([
#         [4, 6, 5, 3, 2, 5, 6, 4],
#         [0, 1, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [7, 7, 7, 7, 7, 7, 7, 7],
#         [4, 6, 5, 3, 2, 5, 6, 4]
#     ])
#     self.assertTrue(self.board.is_under_attack_by_any_piece((0, 0), "black"))
#     self.assertFalse(self.board.is_under_attack_by_any_piece((0, 0), "white"))

# def test_is_castling_attempt(self):
#     self.board.board_matrix = np.array([
#         [4, 6, 5, 3, 2, 5, 6, 4],
#         [0, 1, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 0, 0, 0, 0, 0],
#         [0, 0, 0, 7, 0, 0, 0, 0],
#         [0, 0, 0, 7, 0, 0, 0, 0],
#         [7, 7, 7, 0, 7, 7, 7, 7],
#         [4, 6, 5, 3, 2, 5, 6, 4]])
#     self.assertTrue(self.board.is_castling_attempt((0, 4), (0, 6)))
#     self.assertFalse(self.board.is_castling_attempt((0, 4), (0, 2)))
