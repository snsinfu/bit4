import tornado.ioloop
import tornado.web


class MainHandler(tornado.web.RequestHandler):
    def get(self, name: str) -> None:
        self.set_status(200)
        self.set_header("content-type", "text/plain")
        self.write(f"Hello, {name}!")


def start() -> None:
    app = tornado.web.Application([
        (r"/(.*)", MainHandler)
    ])
    app.listen(8888)
    tornado.ioloop.IOLoop.current().start()
