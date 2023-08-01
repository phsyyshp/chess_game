//add a code for ches boar gui
//create the board
function create_board(){
    var board = document.createElement("table");
    board.setAttribute("id", "chess_board");
    for (var i = 0; i < 8; i++){
        var row = document.createElement("tr");
        for (var j = 0; j < 8; j++){
            var cell = document.createElement("td");
            cell.setAttribute("id", "cell" + i.toString() + j.toString());
            row.appendChild(cell);
        }
        board.appendChild(row);
    }
    return board;
}
//create the pieces
function create_pieces(){
    var pieces = document.createElement("table");
    pieces.setAttribute("id", "chess_pieces");
    for (var i = 0; i < 2; i++){
        var row = document.createElement("tr");
        for (var j = 0; j < 8; j++){
            var cell = document.createElement("td");
            cell.setAttribute("id", "piece" + i.toString() + j.toString());
            row.appendChild(cell);
        }
        pieces.appendChild(row);
    }
    return pieces;
}
//create the board and pieces
function create_board_and_pieces(){
    var board = create_board();
    var pieces = create_pieces();
    var container = document.createElement("div");
    container.setAttribute("id", "chess_container");
    container.appendChild(board);
    container.appendChild(pieces);
    return container;
}

// Path: front_end\chess_gui.js
//add a code for ches boar gui
//create the board
