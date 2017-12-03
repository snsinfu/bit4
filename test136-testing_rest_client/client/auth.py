import hmac
import time

import requests

from .exceptions import ClientException

HTTP_HEADER_KEY = 'X-Access-Key'
HTTP_HEADER_NONCE = 'X-Access-Nonce'
HTTP_HEADER_SIGNATURE = 'X-Access-Signature'
HMAC_HASH = 'sha256'

def _make_nonce():
    time.sleep(0.001)
    return str(int(time.time() * 1000))

def _read_body(body):
    if body is None:
        return bytes()
    if isinstance(body, bytes):
        return body
    if isinstance(body, str):
        return body.encode()
    raise ClientException('Unrecognized body type')

class HMACAuth(requests.auth.AuthBase):
    """ Sign a request with a pair of key and secret """
    def __init__(self, key, secret):
        self._key = key
        self._secret = secret.encode()

    def __call__(self, request):
        nonce = _make_nonce()
        signature = self._compute_signature(request, nonce)
        request.headers[HTTP_HEADER_KEY] = self._key
        request.headers[HTTP_HEADER_NONCE] = nonce
        request.headers[HTTP_HEADER_SIGNATURE] = signature
        return request

    def _compute_signature(self, request, nonce):
        mac = hmac.new(self._secret, digestmod=HMAC_HASH)
        mac.update(nonce.encode())
        mac.update(request.url.encode())
        mac.update(_read_body(request.body))
        return mac.hexdigest()
