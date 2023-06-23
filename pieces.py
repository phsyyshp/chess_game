import individual_pieces as ip
class pieces:
    def __init__(self, color):
        if color == "black":
            self.p = [ip.pawn("a7","black"),ip.pawn("b7","black"),ip.pawn("c7","black"),ip.pawn("d7","black"),ip.pawn("e7","black"),ip.pawn("f7","black"),ip.pawn("g7","black"),ip.pawn("h7","black")]
            self.N = [ip.knight("b8","black"),ip.knight("g8","black")]
            self.B = [ip.bishop("c8","black"),ip.bishop("f8","black")] 
            self.R = [ip.rook("a8","black"),ip.rook("h8","black")]
            self.Q = [ip.queen("d8","black")]
            self.K = [ip.king("e8","black")]
        else:
            self.p = [ip.pawn("a2","white"),ip.pawn("b2","white"),ip.pawn("c2","white"),ip.pawn("d2","white"),ip.pawn("e2","white"),ip.pawn("f2","white"),ip.pawn("g2","white"),ip.pawn("h2","white")]
            self.N = [ip.knight("b1","white"),ip.knight("g1","white")]
            self.B = [ip.bishop("c1","white"),ip.bishop("f1","white")] 
            self.R = [ip.rook("a1","white"),ip.rook("h1","white")]
            self.Q = [ip.queen("d1","white")]
            self.K = [ip.king("e1","white")]
    def all_pieces(self):
        return self.p + self.N + self.B + self.R + self.Q + self.K
                       
 
aa = pieces("black")            
print(type(aa.p))