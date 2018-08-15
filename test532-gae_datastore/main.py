import asyncio
import datetime

import aiohttp
from google.cloud import datastore
from sanic import Sanic, response


app = Sanic(__name__)
datastore_client = datastore.Client()


@app.route("/")
async def home(request):
    query = datastore_client.query(kind="tick", order=("-timepoint",))

    entities = []
    for entity in query.fetch(5):
        entities.append(
            {
                "requester": entity["requester"],
                "timepoint": entity["timepoint"].isoformat(" "),
            }
        )

    return response.json({"entities": entities})


@app.route("/update")
async def update(request):
    if request.headers.get("X-Appengine-Cron") != "true":
        return response.text("", 403)

    with datastore_client.batch() as batch:
        for _ in range(2):
            await asyncio.sleep(1)

            entity_names = [
                "91018dfad51dba77",
                "6b0f94a7e0475046",
                "1d7a542d045c52a5",
                "69f277c87ddfa831",
                "9d4a30f552011dd9",
                "27311ca59c28e075",
                "a110a9dc3f302035",
                "438d71e746a3ad08",
                "32f523dcc8e38099",
            ]

            time = datetime.datetime.now(datetime.timezone.utc)
            name = entity_names[int(time.timestamp()) % 9]
            key = datastore_client.key("tick", name)
            entity = datastore.Entity(key)
            entity.update(
                {
                    "requester": "{}:{}".format(request.ip, request.port),
                    "timepoint": time,
                }
            )
            batch.put(entity)

    return response.text("OK")
