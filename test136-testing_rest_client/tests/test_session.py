import hmac
import json
import urllib.parse

import bottle
import pytest
import pytest_localserver.http

from client.auth import (HMAC_HASH, HTTP_HEADER_KEY, HTTP_HEADER_NONCE,
                         HTTP_HEADER_SIGNATURE)
from client.exceptions import ClientException
from client.session import Session

TEST_KEY = 'this-is-a-key'
TEST_SECRET = '0123456789'

key_repository = {
    TEST_KEY: {
        'secret': TEST_SECRET,
        'nonce': -1,
    }
}

@bottle.route('/<path:path>', ['GET', 'POST', 'DELETE'])
def handle(path):
    bottle.content_type = 'application/json'

    req = bottle.request
    key = req.headers[HTTP_HEADER_KEY]
    nonce = req.headers[HTTP_HEADER_NONCE]
    signature = req.headers[HTTP_HEADER_SIGNATURE]

    if key not in key_repository:
        bottle.response.status = 403
        return json.dumps({'error': 'Unrecognized key'}).encode()

    if int(nonce) <= key_repository[key]['nonce']:
        bottle.response.status = 403
        return json.dumps({'error': 'Repeated nonce'}).encode()
    key_repository[key]['nonce'] = int(nonce)

    secret = key_repository[key]['secret']
    mac = hmac.new(secret.encode(), digestmod=HMAC_HASH)
    mac.update(nonce.encode())
    mac.update(req.url.encode())
    mac.update(req.body.read())

    if signature != mac.hexdigest():
        bottle.response.status = 401
        return json.dumps({'error': 'Bad signature'}).encode()

    return b'{}'

@pytest.fixture
def testserver(request):
    server = pytest_localserver.http.WSGIServer(application=bottle.default_app())
    server.start()
    request.addfinalizer(server.stop)
    return server

def test_auth(testserver):
    with Session(testserver.url, TEST_KEY, TEST_SECRET) as session:
        assert session.post('/series/cpu/0') == {}
        assert session.get('/series/cpu/0/load') == {}
        assert session.delete('/series/cpu/0') == {}

        assert session.post('/series/cpu/1', params={'label': 'newcpu'}) == {}
        assert session.get('/series/cpu/1/load', params={'bin': '5s'}) == {}
        assert session.delete('/series/cpu', params={'id': 1}) == {}

def test_auth_badkey(testserver):
    with Session(testserver.url, TEST_KEY + 'x', TEST_SECRET) as session:
        with pytest.raises(ClientException):
            session.post('/series/cpu/0')
        with pytest.raises(ClientException):
            session.get('/series/cpu/0/load')
        with pytest.raises(ClientException):
            session.delete('/series/cpu/0')

def test_auth_badsecret(testserver):
    with Session(testserver.url, TEST_KEY, TEST_SECRET + 'x') as session:
        with pytest.raises(ClientException):
            session.post('/series/cpu/0')
        with pytest.raises(ClientException):
            session.get('/series/cpu/0/load')
        with pytest.raises(ClientException):
            session.delete('/series/cpu/0')
