from sanic import Sanic, response


app = Sanic(__name__)


@app.route("/resource/<resid:int>")
async def resource(request, resid):
    return response.json({"client": request.ip, "id": resid})


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=8080)
