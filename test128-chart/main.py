import json
import os
import time

import unixen
import websocket

@unixen.catch
def main():
    WSAPI_URI = os.getenv('WSAPI_URI')
    TRADE_PAIR = os.getenv('TRADE_PAIR')

    def on_open(ws):
        ws.send(json.dumps({'type': 'subscribe',
                            'channel': TRADE_PAIR + '-trades'}))

    def on_message(ws, message):
        trade_id, trade_pair, price, amount, side = json.loads(message)
        print(f'{trade_id}\t{timestamp()}\t{side}\t{price}\t{amount}')

    ws = websocket.WebSocketApp(WSAPI_URI,
                                on_open=on_open,
                                on_message=on_message)
    ws.run_forever()

def timestamp():
    # Keep sufficient precision for time-series DB.
    return round(time.time(), 3)

if __name__ == '__main__':
    main()
