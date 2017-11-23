import binascii
import hmac
import io
import json
import os
import time
import urllib.request

def main():
    API_SERVER = os.getenv('API_SERVER')
    API_KEY = os.getenv('API_KEY')
    API_SECRET = os.getenv('API_SECRET').encode('ascii')

    post = {
        'title': 'Beautiful is better than ugly',
        'score': 99,
    }

    method = 'POST'
    path = '/v1/posts'
    timestamp = str(int(time.time()))
    payload = json.dumps(post).encode('utf-8')

    mac = hmac.new(API_SECRET, digestmod='sha256')
    mac.update(timestamp.encode('ascii'))
    mac.update(method.encode('ascii'))
    mac.update(path.encode('ascii'))
    mac.update(payload)
    signature = binascii.hexlify(mac.digest()).decode('ascii')

    headers = {
        'X-Auth-Key':       API_KEY,
        'X-Auth-Timestamp': timestamp,
        'X-Auth-Signature': signature,
        'Content-Type':     'application/json',
    }
    req = urllib.request.Request(API_SERVER + path,
                                 method=method, headers=headers, data=payload)
    with urllib.request.urlopen(req) as res:
        response = json.load(res)
        print(response['message'])

if __name__ == '__main__':
    main()
