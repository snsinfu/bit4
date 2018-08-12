from sanic import Sanic
from sanic.response import json

app = Sanic(__name__)


@app.route("/")
async def home(request):
    return json({"origin": request.host})
