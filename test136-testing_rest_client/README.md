Testing REST client code using [pytest-localserver][1] and [Bottle][2].

[1]: https://bitbucket.org/pytest-dev/pytest-localserver/
[2]: https://bottlepy.org/docs/dev/

pytest-localserver launches a local HTTP server as a test fixture and Bottle
handles routing and WSGI gotcha. Test code communicates with the local server
and check client behavior.

Run tests:

    pipenv install --dev
    pipenv run python -m pytest
