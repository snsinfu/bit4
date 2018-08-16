import logging

from aiohttp import web


routes = web.RouteTableDef()


@routes.get("/resource/{resid:\d+}")
async def resource(request):
    return web.json_response(
        {"client": request.remote, "id": int(request.match_info["resid"])}
    )


app = web.Application()
app.router.add_routes(routes)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    web.run_app(app, host="127.0.0.1", port=8080)
