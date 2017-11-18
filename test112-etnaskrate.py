import requests
import unixen

@unixen.catch
def main():
    etn_price_in_btc = cryptopia('/api/GetMarket/ETN_BTC')['Data']['AskPrice']
    print('Cryptopia\t{:.8f} BTC/ETN'.format(etn_price_in_btc))

    etn_price_in_ltc = cryptopia('/api/GetMarket/ETN_LTC')['Data']['AskPrice']
    print('Cryptopia\t{:.8f} LTC/ETN'.format(etn_price_in_ltc))

    btc_price_in_jpy = float(coincheck('/api/rate/btc_jpy')['rate'])
    estimated_etn_price_in_jpy = etn_price_in_btc * btc_price_in_jpy
    print('Coincheck\t{:8.1f} JPY/BTC\t=> {:.3f} JPY/ETN'.format(btc_price_in_jpy, estimated_etn_price_in_jpy))

    ltc_price_in_jpy = float(coincheck('/api/rate/ltc_jpy')['rate'])
    estimated_etn_price_in_jpy = etn_price_in_ltc * ltc_price_in_jpy
    print('Coincheck\t{:8.1f} JPY/LTC\t=> {:.3f} JPY/ETN'.format(ltc_price_in_jpy, estimated_etn_price_in_jpy))

    btc_price_in_jpy = bitflyer('/api/echo/price')['ask']
    estimated_etn_price_in_jpy = etn_price_in_btc * btc_price_in_jpy
    print('Bitflyer\t{:8.1f} JPY/BTC\t=> {:.3f} JPY/ETN'.format(btc_price_in_jpy, estimated_etn_price_in_jpy))

def cryptopia(api):
    return requests.get('https://www.cryptopia.co.nz' + api).json()

def coincheck(api):
    return requests.get('https://coincheck.com' + api).json()

def bitflyer(api):
    return requests.get('https://bitflyer.jp' + api).json()

if __name__ == '__main__':
    main()
