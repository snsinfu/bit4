import binascii
import hmac
import time
import urllib.parse

import requests

class HMACAuth(requests.auth.AuthBase):
    def __init__(self, key, secret, digestmod='sha256'):
        self._key = key
        self._secret = secret
        self._digestmod = digestmod

    def __eq__(self, other):
        return (self._key == other._key and
                self._secret == other._secret and
                self._digestmod == other._digestmod)

    def __ne__(self, other):
        return not (self == other)

    def __call__(self, request):
        timestamp, signature = self._compute_signature(request)
        request.headers['X-Auth-Key'] = self._key
        request.headers['X-Auth-Timestamp'] = timestamp
        request.headers['X-Auth-Signature'] = signature
        return request

    def _compute_signature(self, request):
        timestamp = self._make_timestamp()
        path = urllib.parse.urlsplit(request.url).path

        mac = hmac.new(self._secret, digestmod=self._digestmod)
        mac.update(timestamp.encode('ascii'))
        mac.update(request.method.encode('ascii'))
        mac.update(path.encode('ascii'))
        mac.update(request.body)

        return timestamp, binascii.hexlify(mac.digest())

    def _make_timestamp(self):
        return str(int(time.time()))
