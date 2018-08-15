from main import app


def test_home_returns_json():
    req, res = app.test_client.get("/")
    assert res.status == 200
    assert res.content_type == "application/json"

    entities = res.json["entities"]
    assert 0 <= len(entities) <= 5


def test_update_forbids_normal_access():
    req, res = app.test_client.get("/update")
    assert res.status == 403


def test_update_accepts_cron_access():
    req, res = app.test_client.get("/update", headers={"X-Appengine-Cron": "true"})
    assert res.status == 200
