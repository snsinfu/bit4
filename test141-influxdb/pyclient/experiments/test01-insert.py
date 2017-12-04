import decimal
import getpass
import json
import os
import time

from influxdb import InfluxDBClient
import websocket

INFLUXDB_HOST = 'localhost'
INFLUXDB_PORT = 8086
INFLUXDB_USER = os.getenv('INFLUXDB_USER')
INFLUXDB_PASSWORD = os.getenv('INFLUXDB_PASSWORD') or getpass.getpass('Password: ')
DATABASE_NAME = 'testdb'
MEASUREMENT_NAME = 'trade'
MARKET_NAME = 'coincheck'
COINCHECK_WS_URI = os.getenv('COINCHECK_WS_URI')
TRADE_PAIR = os.getenv('TRADE_PAIR')
MINIMUM_BTC = decimal.Decimal('0.00000001')

client = InfluxDBClient(host=INFLUXDB_HOST,
                        port=INFLUXDB_PORT,
                        database=DATABASE_NAME,
                        username=INFLUXDB_USER,
                        password=INFLUXDB_PASSWORD)

def on_open(ws):
    ws.send(json.dumps({'type': 'subscribe',
                        'channel': TRADE_PAIR + '-trades'}))

def on_message(ws, message):
    trade_id, trade_pair, price, amount, side = json.loads(message)
    timestamp = int(time.time() * 1e6)

    price_value = int(decimal.Decimal(price))
    amount_value = int(decimal.Decimal(amount) / MINIMUM_BTC)

    # FIXME: Timestamp?
    data = {
        'points': [
            {
                'fields': {
                    'price': price_value,
                    'amount': amount_value,
                },
                'tags': {
                    'market': MARKET_NAME,
                    'pair': TRADE_PAIR,
                },
                'measurement': MEASUREMENT_NAME,
                'time': timestamp,
            },
        ],
    }
    params = {
        'db': DATABASE_NAME,
        'precision': 'us',
    }
    client.write(data, params)

    print(f'{timestamp}\t{side}\t{price_value}\t{amount_value}')

ws = websocket.WebSocketApp(COINCHECK_WS_URI,
                            on_open=on_open,
                            on_message=on_message)
ws.run_forever()
