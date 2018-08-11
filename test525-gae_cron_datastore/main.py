import datetime

from flask import Flask, render_template, request, abort
from google.cloud import datastore


app = Flask(__name__)
datastore_client = datastore.Client()


@app.route("/")
def home():
    query = datastore_client.query(kind="tick")
    query.order = ["-timestamp"]
    tick, = query.fetch(limit=1)

    return render_template("home.html.j2", tick=tick)


@app.route("/tick")
def tick():
    if request.headers.get("X-Appengine-Cron") != "true":
        return abort(403)

    entity = datastore.Entity(key=datastore_client.key("tick"))
    entity.update(
        {
            "source": request.remote_addr,
            "timestamp": datetime.datetime.now(datetime.timezone.utc),
        }
    )
    datastore_client.put(entity)

    return "OK"


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=8080, debug=True)
