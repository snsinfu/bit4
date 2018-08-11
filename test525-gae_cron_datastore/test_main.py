import unittest
import main


class TestMain(unittest.TestCase):
    def setUp(self):
        self.client = main.app.test_client()

    def test_home_works(self):
        r = self.client.get("/")
        self.assertEqual(r.status_code, 200)

    def test_tick_forbids_normal_access(self):
        r = self.client.get("/tick")
        self.assertEqual(r.status_code, 403)

    def test_tick_accepts_cron_access(self):
        r = self.client.get("/tick", headers={"X-Appengine-Cron": "true"})
        self.assertEqual(r.status_code, 200)
