import tornado.ioloop
import tornado.web


class MainHandler(tornado.web.RequestHandler):
    def get(self, path: str) -> None:
        self.set_status(200)
        self.set_header("content-type", "text/plain")
        req = self.request
        self.write(f"Hello, world!\n")
        self.write(f"Path: {path}\n")
        self.write(f"Host: {req.host}\n")
        self.write(f"Remote IP: {req.remote_ip}\n")


if __name__ == "__main__":
    app = tornado.web.Application([
        (r"/(.*)", MainHandler)
    ])
    app.listen(80)
    tornado.ioloop.IOLoop.current().start()
