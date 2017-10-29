"use strict"

window.addEventListener("DOMContentLoaded", function(event) {
  let board = document.getElementById("board")
  createBoardInterface(board)
})

function createBoardInterface(board) {
  let table = document.createElement("table")
  table.classList.add("quoridor-board")

  let cells = new Array()
  for (var rowIndex = 0; rowIndex < 9; rowIndex++) {
    let tr = document.createElement("tr")
    table.appendChild(tr)

    let row = new Array()
    cells.push(row)

    for (var colIndex = 0; colIndex < 9; colIndex++) {
      let td = document.createElement("td")
      tr.appendChild(td)

      row.push(td)
    }
  }

  cells[3][3].classList.add("south-blocked")
  cells[3][4].classList.add("south-blocked")

  cells[2][4].classList.add("south-blocked")
  cells[2][5].classList.add("south-blocked")

  cells[2][3].classList.add("east-blocked")
  cells[3][3].classList.add("east-blocked")

  cells[3][5].classList.add("east-blocked")
  cells[4][5].classList.add("east-blocked")

  const pieceText = "●";

  cells[5][5].appendChild(document.createTextNode(pieceText))
  cells[5][5].classList.add("occupied-by-red")

  cells[2][2].appendChild(document.createTextNode(pieceText))
  cells[2][2].classList.add("occupied-by-blue")

  cells[3][7].appendChild(document.createTextNode(pieceText))
  cells[3][7].classList.add("occupied-by-green")

  cells[6][4].appendChild(document.createTextNode(pieceText))
  cells[6][4].classList.add("occupied-by-yellow")

  board.appendChild(table)
}
