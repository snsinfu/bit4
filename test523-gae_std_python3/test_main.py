import unittest
import main


class TestMain(unittest.TestCase):
    def setUp(self):
        self.client = main.app.test_client()

    def test_home(self):
        r = self.client.get("/")
        self.assertEqual(r.status_code, 200)
        self.assertIn("Hello, world!", r.data.decode("utf-8"))
