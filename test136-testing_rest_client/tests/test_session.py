import json

import pytest
import pytest_localserver.http

from client.session import Session

TEST_KEY = 'this-is-a-key'
TEST_SECRET = '0123456789'

def fake_app(environ, start_response):
    status = '200 OK'
    response_headers = [
        ('Content-type', 'application/json')
    ]
    start_response(status, response_headers)
    return [json.dumps({'error': 'No error'}).encode()]

@pytest.fixture
def testserver(request):
    server = pytest_localserver.http.WSGIServer(application=fake_app)
    server.start()
    request.addfinalizer(server.stop)
    return server

def test_aaa(testserver):
    with Session(testserver.url, TEST_KEY, TEST_SECRET) as session:
        r = session.get('/')
        assert r == {'error': 'No error'}
