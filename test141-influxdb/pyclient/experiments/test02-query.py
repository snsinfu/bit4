import getpass
import os
import random
import time

from influxdb import InfluxDBClient

INFLUXDB_USER = os.getenv('INFLUXDB_USER')
INFLUXDB_PASSWORD = os.getenv('INFLUXDB_PASSWORD') or getpass.getpass('Password: ')

client = InfluxDBClient(host='localhost',
                        port=8086,
                        database='testdb',
                        username=INFLUXDB_USER,
                        password=INFLUXDB_PASSWORD)

data_bin = '1s'
params = {
    'market': 'coincheck',
    'pair': 'btc_jpy',
}
query = f"""
SELECT sum(weighted_price) / sum(amount) AS mean_price
  FROM (SELECT price * amount AS weighted_price, amount
          FROM trade
         WHERE market = 'coincheck'
           AND pair = 'btc_jpy')
GROUP BY time({data_bin}) fill(none)
"""

result = client.query(query, params=params)

for point in result.get_points():
    print(point)

client.close()
