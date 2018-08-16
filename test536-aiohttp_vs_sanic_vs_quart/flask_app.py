from flask import Flask, jsonify, request


app = Flask(__name__)


@app.route("/resource/<int:resid>")
def resource(resid):
    return jsonify({"client": request.remote_addr, "id": resid})


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=8080)
