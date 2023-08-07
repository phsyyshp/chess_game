let chessBoard = document.getElementById("chess_board");
const rows = [];
for (let i = 0; i < 8; i++) {
  rows[i] = chessBoard.insertRow(i);
  for (let j = 0; j < 8; j++) {
    let cell = rows[i].insertCell(j);
    cell.innerHTML = "";
    if ((i + j) % 2 === 0) {
      cell.className = "whiteCells";
    } else {
      cell.className = "blackCells";
    }
    if (i === 6) {
      cell.innerHTML = "♙";
    } else if (i == 1) {
      cell.innerHTML = "♟";
    }
    // Rooks
    if (i === 0 || i === 7) {
      cell.innerHTML = i === 0 ? "♜" : "♖";
    }

    // Knights
    if ((i === 0 || i === 7) & (j === 1 || j === 6)) {
      cell.innerHTML = i === 0 ? "♞" : "♘";
    }

    // Bishops
    if ((i === 0 || i === 7) && (j === 2 || j === 5)) {
      cell.innerHTML = i === 0 ? "♝" : "♗";
    }

    // Queens
    if ((i === 0 && j === 3) || (i === 7 && j === 3)) {
      cell.innerHTML = i === 0 ? "♛" : "♕";
    }

    // Kings
    if ((i === 0 && j === 4) || (i === 7 && j === 4)) {
      cell.innerHTML = i === 0 ? "♚" : "♔";
    }
  }
}
