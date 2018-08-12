from main import app


def test_home_ok():
    req, res = app.test_client.get("/")
    assert res.status == 200


def test_home_returns_json():
    req, res = app.test_client.get("/")
    assert res.content_type == "application/json"
