import asyncio
import os

import aiohttp

from sanic import Sanic
from sanic.response import json, text

app = Sanic()


@app.route("/")
async def home(request):
    return json({"remote_addr": request.ip + ":" + str(request.port)})


@app.route("/example")
async def tick(request):
    async with aiohttp.ClientSession() as session:
        async with session.get("https://example.com/") as response:
            m = await response.text()
    return text(str(len(m)) + "\n")


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=(os.getenv("PORT") or 8080), debug=True)
