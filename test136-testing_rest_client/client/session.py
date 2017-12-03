import json
import requests

from .auth import HMACAuth
from .exceptions import ClientException

def _parse_response(response):
    if not response.ok:
        raise ClientException(_describe_error(response))
    return response.json()

def _describe_error(response):
    status = f'{response.status_code} {response.reason}'
    try:
        status += ': ' + response.json()['error']
    except:
        pass # No error details.
    return status

class Session:
    def __init__(self, host, key, secret):
        self._host = host
        self._session = requests.Session()
        self._session.auth = HMACAuth(key, secret)

    def get(self, path, params=None):
        r = self._session.get(self._host + path, params=params)
        return _parse_response(r)

    def post(self, path, params=None):
        r = self._session.post(self._host + path, json=params)
        return _parse_response(r)

    def delete(self, path, params=None):
        r = self._session.delete(self._host + path, json=params)
        return _parse_response(r)

    def close(self):
        self._session.close()

    def __enter__(self):
        return self

    def __exit__(self, etype, e, backtrace):
        self.close()
