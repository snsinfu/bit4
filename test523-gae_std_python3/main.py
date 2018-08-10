from flask import Flask, render_template

app = Flask(__name__)


@app.route("/")
def home():
    return render_template("page.html.j2", title="Home", message="Hello, world!")


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=8080, debug=True)
