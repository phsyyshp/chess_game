#board visualization class
#create a class that visualizes the board by printing it to terminal in various styles
import numpy as np
import board as bd  #board.py
from utilities import *

class BoardVisualizer:
    def __init__(self, board):
        self.board = board
        self.board_matrix = board.board_matrix
        self.board_matrix = np.flip(self.board_matrix, axis=0)
        self.board_matrix = np.rot90(self.board_matrix, k=3)
        self.board_matrix = np.flip(self.board_matrix, axis=1)
        self.board_matrix = self.board_matrix.astype('str')
        self.board_matrix[self.board_matrix == '0'] = ' '
        self.board_matrix[self.board_matrix == '1'] = '♟'
        self.board_matrix[self.board_matrix == '2'] = '♙'
        self.board_matrix[self.board_matrix == '3'] = '♝'
        self.board_matrix[self.board_matrix == '4'] = '♗'
        self.board_matrix[self.board_matrix == '5'] = '♞'
        self.board_matrix[self.board_matrix == '6'] = '♘'
        self.board_matrix[self.board_matrix == '7'] = '♜'
        self.board_matrix[self.board_matrix == '8'] = '♖'
        self.board_matrix[self.board_matrix == '9'] = '♛'
        self.board_matrix[self.board_matrix == '10'] = '♕'
        self.board_matrix[self.board_matrix == '11'] = '♚'
        self.board_matrix[self.board_matrix == '12'] = '♔'
        self.board_matrix[self.board_matrix == '13'] = '•'
        self.board_matrix[self.board_matrix == '14'] = '◦'
        self.board_matrix[self.board_matrix == '15'] = '○'
        self.board_matrix[self.board_matrix == '16'] = '●'
        self.board_matrix[self.board_matrix == '17'] = '◆'
        self.board_matrix[self.board_matrix == '18'] = '◇'
        self.board_matrix[self.board_matrix == '19'] = '◈'
        self.board_matrix[self.board_matrix == '20'] = '◊'
        self.board_matrix[self.board_matrix == '21'] = '◉'
        self.board_matrix[self.board_matrix == '22'] = '◎'
        self.board_matrix[self.board_matrix == '23'] = '◍'
        self.board_matrix[self.board_matrix == '24'] = '◌'
        self.board_matrix[self.board_matrix == '25'] = '◐'
        self.board_matrix[self.board_matrix == '26'] = '◑'
        self.board_matrix[self.board_matrix == '27'] = '◒'
        self.board_matrix[self.board_matrix == '28'] = '◓'
        self.board_matrix[self.board_matrix == '29'] = '◔'
        self.board_matrix[self.board_matrix == '30'] = '◕'
        self.board_matrix[self.board_matrix == '31'] = '◖'
        self.board_matrix[self.board_matrix == '32'] = '◗'
        self.board_matrix[self.board_matrix == '33'] = '◘'
        self.board_matrix[self.board_matrix == '34'] = '◙'
        self.board_matrix[self.board_matrix == '35'] = '◚'
        self.board_matrix[self.board_matrix == '36'] = '◛'
        self.board_matrix[self.board_matrix == '37'] = '◜'
        self.board_matrix[self.board_matrix == '38'] = '◝'
        self.board_matrix[self.board_matrix == '39'] = '◞'
        self.board_matrix[self.board_matrix == '40'] = '◟'
        self.board_matrix[self.board_matrix == '41'] = '◠'
        self.board_matrix[self.board_matrix == '42'] = '◡'
        self.board_matrix[self.board_matrix == '43'] = '◢'
        self.board_matrix[self.board_matrix == '44'] = '◣'
        self.board_matrix[self.board_matrix == '45'] = '◤'
        self.board_matrix[self.board_matrix == '46'] = '◥'
        self.board_matrix[self.board_matrix == '47'] = '◦'
        self.board_matrix[self.board_matrix == '48'] = '◧'
        self.board_matrix[self.board_matrix == '49'] = '◨'
        self.board_matrix[self.board_matrix == '50'] = '◩'
        self.board_matrix[self.board_matrix == '51'] = '◪'
        self.board_matrix[self.board_matrix == '52'] = '◫'
        self.board_matrix[self.board_matrix == '53'] = '◬'
        self.board_matrix[self.board_matrix == '54'] = '◭'
    def print_board(self):
        print(self.board_matrix)
    def print_board_with_coordinates(self):
        print('   A  B  C  D  E  F  G  H')
        print('  ------------------------')
        for i in range(8):
            print(8-i, end='|')
            for j in range(8):
                print(self.board_matrix[i][j], end='|')
            print(8-i)
            print('  ------------------------')
        print('   A  B  C  D  E  F  G  H')
    def print_board_with_coordinates_flipped(self):
        print('   H  G  F  E  D  C  B  A')
        print('  ------------------------')
        for i in range(8):
            print(i+1, end='|')
            for j in range(8):
                print(self.board_matrix[i][j], end='|')
            print(i+1)
            print('  ------------------------')
        print('   H  G  F  E  D  C  B  A')
    # method for print colored board
    def print_colored_board(self):
        #create ansi color codes
        #black
        black = '\033[30m'
        #red
        red = '\033[31m'
        #green
        green = '\033[32m'
        #yellow
        yellow = '\033[33m'
        #blue
        blue = '\033[34m'
        #create chess board with colored background
        print('   A  B  C  D  E  F  G  H')
        print('  ------------------------')
        for i in range(8):
            print(8-i, end='|')
            for j in range(8):
                if i % 2 == 0:
                    if j % 2 == 0:
                        print(black + self.board_matrix[i][j] + '\033[0m', end='|')
                    else:
                        print(red + self.board_matrix[i][j] + '\033[0m', end='|')
                else:
                    if j % 2 == 0:
                        print(red + self.board_matrix[i][j] + '\033[0m', end='|')
                    else:
                        print(black + self.board_matrix[i][j] + '\033[0m', end='|')
            print(8-i)
            print('  ------------------------') 
        print('   A  B  C  D  E  F  G  H')
    # method for print colored board flipped
    def print_colored_board_flipped(self):
        #create ansi color codes
        #black
        black = '\033[30m'
        #red
        red = '\033[31m'
        #green
        green = '\033[32m'
        #yellow
        yellow = '\033[33m'
        #blue
        blue = '\033[34m'
        #create chess board with colored background
        print('   H  G  F  E  D  C  B  A')
        print('  ------------------------')
        for i in range(8):
            print(i+1, end='|')
            for j in range(8):
                if i % 2 == 0:
                    if j % 2 == 0:
                        print(black + self.board_matrix[i][j] + '\033[0m', end='|')
                    else:
                        print(red + self.board_matrix[i][j] + '\033[0m', end='|')
                else:
                    if j % 2 == 0:
                        print(red + self.board_matrix[i][j] + '\033[0m', end='|')
                    else:
                        print(black + self.board_matrix[i][j] + '\033[0m', end='|')
            print(i+1)
            print('  ------------------------') 
        print('   H  G  F  E  D  C  B  A')


#examples for each method   
gg=bd.board()
gg.set_board_to_initial_configuration()
a = BoardVisualizer(gg)
a.print_board()
a.print_board_with_coordinates()
a.print_board_with_coordinates_flipped()
a.print_colored_board()
a.print_colored_board_flipped()
