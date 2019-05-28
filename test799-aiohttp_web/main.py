import datetime

from aiohttp import web


routes = web.RouteTableDef()
routes.static("/static", "static")


@routes.get("/")
async def get_home(request):
    return web.FileResponse("home.html")


@routes.get("/status")
async def get_status(request):
    now = datetime.datetime.now(datetime.timezone.utc)
    return web.json_response({"message": now.isoformat(sep=" ")})


app = web.Application()
app.add_routes(routes)


if __name__ == "__main__":
    import argparse
    import logging

    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser()
    parser.add_argument("--host", type=str, default="127.0.0.1")
    parser.add_argument("--port", type=int, default=4000)
    options = parser.parse_args()

    web.run_app(app, host=options.host, port=options.port)
