let chessBoard = document.getElementById("chess_board");
const rows = [];
for (let i = 0; i < 8; i++) {
  rows[i] = chessBoard.insertRow(i);
  for (let j = 0; j < 8; j++) {
    let cell = rows[i].insertCell(j);
    cell.innerHTML = "  a ";
    if ((i + j) % 2 === 0) {
      cell.className = "whiteCells";
    } else {
      cell.className = "blackCells";
    }
  }
}
