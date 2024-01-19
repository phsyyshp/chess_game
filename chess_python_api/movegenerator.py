import os
import GameState as gs
import sys
import Board as bd
import Visual as vs


class MoveGenerator:
    def __init__(self, board: bd.Board):
        self.board = board

    def play_multiple_moves(self, old_moves, new_moves):
        boards = []
        for old_move, new_move in zip(old_moves, new_moves):
            for nm in new_move:
                if len(nm.tolist()) == 0:
                    continue
                board_copy = self.board.copy()
                board_copy.move(old_move, nm)
                board_copy.change_turn()
                # print( board_copy.board_matrix)
                boards.append(board_copy)
        return boards

    def play_all_legal_moves(self, color):
        old_moves, new_moves = gs.GameState(self.board).get_all_legal_moves(color)
        return self.play_multiple_moves(old_moves, new_moves)

    def generate_all_moves_in_depth(self, depth):
        boards = self.play_all_legal_moves("white")
        current_boards = boards.copy()
        tempbds = []
        color_dic = ["black", "white"]
        i = 0
        while i < depth:
            print(len(current_boards))

            for board_item in current_boards:
                new_boards = MoveGenerator(board_item).play_all_legal_moves(
                    color_dic[i % 2]
                )
                boards += new_boards
                tempbds += new_boards
            i += 1
            current_boards = tempbds.copy()
            tempbds = []

        return boards

    # def get_moves_tree(self, depth)
    #     counter = 0
    #     move_tree = [gs.GameState(self.board).get_all_legal_moves("white")]
    #     for i in range(depth):
    #         for level
    #            boards = self.play_multiple_moves(move_tree[i])

    #         all_legal_moves = gs.GameState(self.board).get_all_legal_moves(color)


bb = bd.Board()
bb.set_board_to_initial_configuration()
# bb.set_board_to_fen_configuration("4rbk1/3p1p2/1p2p2p/p3B1pQ/3PB3/4P1KP/5PP1/4q3")
print(vs.Visual("white", bb).show())
mg = MoveGenerator(bb)
board_tree = mg.generate_all_moves_in_depth(1)
# with open("board_tree.txt", "w", encoding="utf8") as f:
#     for board in board_tree:
#         f.write(vs.Visual("white", board).show())
#         f.write("\n")
import time

for board in board_tree:
    os.system("cls||clear")
    # print(board)

    b = vs.Visual("white", board).show()
    print(b)
    # time.sleep(1)
print(len(board_tree))
