import requests
import requests_mock

with requests_mock.Mocker() as mock:
    mock.get('https://api.bitfinex.com/v1/pubticker/x',
             status_code=400,
             json={'message': 'Unknown symbol'})

    mock.get('https://api.bitfinex.com/v1/pubticker/BTCUSD',
             status_code=200,
             json={'mid': '17467.5',
                   'bid': '17461.0',
                   'ask': '17474.0',
                   'last_price': '17464.0',
                   'low': '16210.0',
                   'high': '17579.0',
                   'volume':'66259.99585512',
                   'timestamp':'1513103299.653059'})

    r = requests.get('https://api.bitfinex.com/v1/pubticker/x')
    assert r.status_code == 400
    assert r.json() == {'message': 'Unknown symbol'}

    r = requests.get('https://api.bitfinex.com/v1/pubticker/BTCUSD')
    assert r.status_code == 200
    assert r.json()['ask'] == '17474.0'
