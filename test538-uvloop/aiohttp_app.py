import argparse
import asyncio
import logging

from aiohttp import web
import uvloop


routes = web.RouteTableDef()


@routes.get("/resource/{resid:\d+}")
async def resource(request):
    return web.json_response(
        {"client": request.remote, "id": int(request.match_info["resid"])}
    )


app = web.Application()
app.router.add_routes(routes)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--uvloop", action="store_true", default=False)
    options = parser.parse_args()

    if options.uvloop:
        asyncio.set_event_loop_policy(uvloop.EventLoopPolicy())

    logging.basicConfig(level=logging.INFO)
    web.run_app(app, host="127.0.0.1", port=8080)
