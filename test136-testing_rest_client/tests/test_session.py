import hmac
import json
import urllib.parse

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

def get_header(environ, name):
    return environ['HTTP_' + name.replace('-', '_').upper()]

def read_body(environ):
    return environ['wsgi.input'].read(int(environ.get('CONTENT_LENGTH', 0)))

def reconstruct_url(environ):
    # https://www.python.org/dev/peps/pep-3333/
    url = environ['wsgi.url_scheme'] + '://'

    if environ.get('HTTP_HOST'):
        url += environ['HTTP_HOST']
    else:
        url += environ['SERVER_NAME']

        if environ['wsgi.url_scheme'] == 'https':
            if environ['SERVER_PORT'] != '443':
               url += ':' + environ['SERVER_PORT']
        else:
            if environ['SERVER_PORT'] != '80':
               url += ':' + environ['SERVER_PORT']

    url += urllib.parse.quote(environ.get('SCRIPT_NAME', ''))
    url += urllib.parse.quote(environ.get('PATH_INFO', ''))
    if environ.get('QUERY_STRING'):
        url += '?' + environ['QUERY_STRING']

    return url

def handle(environ):
    key = get_header(environ, HTTP_HEADER_KEY)
    nonce = get_header(environ, HTTP_HEADER_NONCE)
    signature = get_header(environ, HTTP_HEADER_SIGNATURE)

    if key not in key_repository:
        return '403 Forbidden', 'Unrecognized key'

    if int(nonce) <= key_repository[key]['nonce']:
        return '403 Forbidden', 'Repeated nonce'
    key_repository[key]['nonce'] = int(nonce)

    secret = key_repository[key]['secret']
    mac = hmac.new(secret.encode(), digestmod=HMAC_HASH)
    mac.update(nonce.encode())
    mac.update(reconstruct_url(environ).encode())
    mac.update(read_body(environ))

    if signature != mac.hexdigest():
        return '401 Unauthorized', 'Bad signature'

    return '200 OK', ''

def fake_app(environ, start_response):
    status, message = handle(environ)
    response_headers = [('Content-type', 'application/json')]
    start_response(status, response_headers)
    return [json.dumps({'error': message}).encode()]

@pytest.fixture
def testserver(request):
    server = pytest_localserver.http.WSGIServer(application=fake_app)
    server.start()
    request.addfinalizer(server.stop)
    return server

def test_auth(testserver):
    with Session(testserver.url, TEST_KEY, TEST_SECRET) as session:
        assert session.post('/series/cpu/0') == {'error': ''}
        assert session.get('/series/cpu/0/load') == {'error': ''}
        assert session.delete('/series/cpu/0') == {'error': ''}

        assert session.post('/series/cpu/1', params={'label': 'newcpu'}) == {'error': ''}
        assert session.get('/series/cpu/1/load', params={'bin': '5s'}) == {'error': ''}
        assert session.delete('/series/cpu', params={'id': 1}) == {'error': ''}

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
