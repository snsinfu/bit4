import requests
import unixen

@unixen.catch
def main():
    report('electroneum')
    report('ark')
    report('neo')

def report(currency):
    r, = coinmarketcap('/v1/ticker/{}/?convert=JPY'.format(currency))

    report = [
        '%8.5f USD/%s' % (float(r['price_usd']), r['symbol']),
        '%8.3f JPY/%s' % (float(r['price_jpy']), r['symbol']),
    ]
    print('\t'.join(report))

def coinmarketcap(api):
    return requests.get('https://api.coinmarketcap.com' + api).json()

if __name__ == '__main__':
    main()
