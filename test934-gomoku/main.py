import random


HORIZONTAL = "H"
VERTICAL = "V"
DIAGONAL = "D"
CONTIG = 5


def main():
    board = Board(5)

    rand = random.Random()
    for i in range(board.size):
        for j in range(board.size):
            board[i, j] = rand.randint(-1, 1)

    print(board)

    # See if there is a winning move for us.
    candidate = find_gomoku_candidate(board, side=1, contig=CONTIG)
    if candidate:
        direction, i, j = candidate
        move_i, move_j = find_hole(board, direction, i, j, length=CONTIG)
        print(move_i, move_j)
        return

    # See if we can prevent the enemy from winning.
    candidate = find_gomoku_candidate(board, side=-1, contig=CONTIG)
    if candidate:
        direction, i, j = candidate
        move_i, move_j = find_hole(board, direction, i, j, length=CONTIG)

        # Try the move and determine if the enemy can really not win.
        board[move_i, move_j] = 1

        if find_gomoku_candidate(board, side=-1, contig=CONTIG):
            # The enemy has another winning move. We lose.
            print("LOSE")
            return

        # This move prevents the enemy from winning.
        print(move_i, move_j)
        return

    # We nor the enemy won't win in single moves. Try an arbitrary move.
    for i in range(board.size):
        for j in range(board.size):
            if board[i, j] == 0:
                print(i, j)
                return

    assert False


def find_hole(board, direction, i, j, *, length):
    delta_i = delta_j = 0
    if direction == VERTICAL:
        delta_i = 1
    if direction == HORIZONTAL:
        delta_j = 1
    if direction == DIAGONAL:
        delta_i = delta_j = 1

    for k in range(length):
        pos_i = i + delta_i * k
        pos_j = j + delta_j * k

        if max(pos_i, pos_j) >= board.size:
            break

        if board[pos_i, pos_j] == 0:
            return pos_i, pos_j

    return None


def find_gomoku_candidate(board, *, contig, side):
    """
    Find a gomoku candidate in a board.

    Parameters:

      board  = Board filled with [-1, 0, 1] values. 0 indicates empty cell.
      contig = Number of contiguous cells required to fill for winning.
      side   = -1 or 1 specifying the player.

    Returns:

      Tuple (dir, i, j) of direction dir and coordinate (i, j). None if no
      candidate can be found.

    """
    n = board.size
    target_score = (contig - 1) * side

    # Search for any horizontal candidate
    for i in range(n):
        for j in range(n - contig + 1):
            score = sum_horizontal(board, i, j, length=contig)
            if score == target_score:
                return HORIZONTAL, i, j

    # Search for any vertical candidate
    for i in range(n - contig + 1):
        for j in range(n):
            score = sum_vertical(board, i, j, length=contig)
            if score == target_score:
                return VERTICAL, i, j

    # Search for any diagonal candidate
    for i in range(n - contig + 1):
        for j in range(n - contig + 1):
            score = sum_diagonal(board, i, j, length=contig)
            if score == target_score:
                return DIAGONAL, i, j

    return None


class Board:
    def __init__(self, size):
        self._size = size
        self._store = [0] * (size * size)

    @property
    def size(self):
        return self._size

    def __getitem__(self, address):
        i, j = address
        return self._store[self._locate(i, j)]

    def __setitem__(self, address, value):
        i, j = address
        self._store[self._locate(i, j)] = value

    def _locate(self, i, j):
        return i * self._size + j

    def __repr__(self):
        s = ""
        for i in range(self.size):
            s += " ".join(f"{self[i, j]:2d}" for j in range(self.size))
            s += "\n"
        return s


def sum_horizontal(board, i, j, length):
    s = 0
    for k in range(j, j + length):
        s += board[i, k]
    return s


def sum_vertical(board, i, j, length):
    s = 0
    for k in range(i, i + length):
        s += board[k, j]
    return s


def sum_diagonal(board, i, j, length):
    s = 0
    for k in range(length):
        s += board[i + k, j + k]
    return s


if __name__ == "__main__":
    main()
