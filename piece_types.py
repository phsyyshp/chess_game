import numpy as np
import individual_pieces as pc
class piece_types:
    def __init__(self, piece_id_number):
        self.piece_id_number = piece_id_number 
    def create_piece(self, position):
        color = "white" if self.piece_id_number <= 7 else   "black"
        normalized_piece_id= self.piece_id_number - (color == "black") * 6
        if normalized_piece_id  == 1:
            return pc.pawn(position, color)
        if normalized_piece_id  == 6:
            return pc.knight(position, color)
        if normalized_piece_id  == 4:
            return pc.rook(position, color)
        if normalized_piece_id  == 3:
            return pc.queen(position, color)
        if normalized_piece_id  == 2:
            return pc.king(position, color)
        if normalized_piece_id  == 5:
            return pc.bishop(position, color)
