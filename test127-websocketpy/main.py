import os
import sys

import websocket

def main():
    try:
        uri = os.getenv('WS_URI') or input('URI: ')
        message = os.getenv('INITIAL_MESSAGE') or input('Message: ')
        with Session(uri, message) as session:
            session.run()
    except KeyboardInterrupt:
        pass

class Session:
    def __init__(self, uri, initial_message):
        ws = websocket.WebSocketApp(uri,
                                    on_open=self._on_open,
                                    on_message=self._on_message,
                                    on_error=self._on_error)
        self._initial_message = initial_message
        self._ws = ws

    def close(self):
        self._ws.close()

    def run(self):
        self._ws.run_forever()

    def _on_open(self, ws):
        ws.send(self._initial_message)

    def _on_message(self, ws, message):
        print(message)

    def _on_error(self, ws, ex):
        raise ex

    def __enter__(self):
        return self

    def __exit__(self, extype, ex, backtrace):
        self.close()

if __name__ == '__main__':
    main()
