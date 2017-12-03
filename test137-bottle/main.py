import bottle

class TestServer:
    def __init__(self, port):
        self._app = bottle.Bottle()
        self._app.get('/series/cpu_load', callback=self.get_cpuload)
        self._port = port

    def run(self):
        self._app.run(port=self._port, quiet=True)

    def get_cpuload(self):
        r = bottle.request
        print('Headers:', list(r.headers.items()))
        print('Query:', list(r.query.items()))
        if r.json:
            print('JSON:', r.json)
        else:
            print('Forms:', list(r.forms.items()))

srv = TestServer(8788)
srv.run()

