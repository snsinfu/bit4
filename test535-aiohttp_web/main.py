from aiohttp import web


routes = web.RouteTableDef()


@routes.get("/")
async def home(request):
    return web.Response(text="Hello, world")


@routes.get("/resource/{id:\d+}")
async def resource(request):
    return web.json_response({"id": int(request.match_info["id"])})


app = web.Application()
app.add_routes(routes)


if __name__ == "__main__":
    import argparse
    import logging

    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser()
    parser.add_argument("--host", type=str, default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8080)
    options = parser.parse_args()

    web.run_app(app, host=options.host, port=options.port)
