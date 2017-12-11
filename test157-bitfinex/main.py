import decimal
import json
import os
import time

import simplejson
import websocket

TRADE_EXECUTION = 'te'

def main():
    BITFINEX_WSURI = os.getenv('BITFINEX_WSURI')
    BITFINEX_CHANNEL = os.getenv('BITFINEX_CHANNEL')
    BITFINEX_PAIR = os.getenv('BITFINEX_PAIR')

    def on_open(ws):
        ws.send(json.dumps({'event': 'subscribe',
                            'channel': BITFINEX_CHANNEL,
                            'symbol': BITFINEX_PAIR}))

    def on_message(ws, payload):
        message = simplejson.loads(payload, use_decimal=True)
        if isinstance(message, dict):
            return handle_event(ws, message)
        if isinstance(message, list):
            return handle_data(ws, message)

    ws = websocket.WebSocketApp(BITFINEX_WSURI,
                                on_open=on_open,
                                on_message=on_message)
    ws.run_forever()

def handle_event(ws, event):
    if event['event'] == 'error':
        print('error {}: {}'.format(event['code'], event['msg']))
        ws.close()

def handle_data(ws, data):
    if len(data) >= 2 and data[1] == TRADE_EXECUTION:
        handle_trade_execution(ws, data)

def handle_trade_execution(ws, data):
    _, _, trade = data
    seq, timestamp, amount, price = trade

    # Bitfinex timestamp is in milliseconds.
    timestamp /= 1000

    # Negative amount means sell order.
    if amount < 0:
        side = 'sell'
        amount = -amount
    else:
        side = 'buy'

    print(f'{seq}\t{timestamp}\t{side}\t{price}\t{amount}')

if __name__ == '__main__':
    main()
