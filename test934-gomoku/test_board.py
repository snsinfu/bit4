import unittest

from main import Board, sum_horizontal, sum_vertical, sum_diagonal


class TestBoard(unittest.TestCase):
    def test_creation_empty(self):
        board = Board(0)
        self.assertEqual(board.size, 0)

    def test_creation_normal(self):
        board = Board(10)
        self.assertEqual(board.size, 10)

    def test_setitem(self):
        board = Board(5)
        board[1, 2] = 3
        self.assertEqual(board[1, 2], 3)
        self.assertEqual(board[2, 1], 0)


class TestBoardSum(unittest.TestCase):
    def setUp(self):
        self.board = board = Board(4)
        board[0, 0] = 1
        board[0, 1] = 2
        board[0, 2] = 3
        board[0, 3] = 4
        board[1, 0] = 5
        board[1, 1] = 6
        board[1, 2] = 7
        board[1, 3] = 8
        board[2, 0] = 9
        board[2, 1] = 0
        board[2, 2] = 1
        board[2, 3] = 2
        board[3, 0] = 3
        board[3, 1] = 4
        board[3, 2] = 5
        board[3, 3] = 6

    def test_sum_horizontal(self):
        self.assertEqual(
            sum_horizontal(self.board, 1, 1, length=3),
            6 + 7 + 8
        )

    def test_sum_vertical(self):
        self.assertEqual(
            sum_vertical(self.board, 1, 1, length=3),
            6 + 0 + 4
        )

    def test_sum_diagonal(self):
        self.assertEqual(
            sum_diagonal(self.board, 1, 1, length=3),
            6 + 1 + 6
        )


if __name__ == "__main__":
    unittest.main()
