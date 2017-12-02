Foo
===

Foo can be tested.

Testing
-------

Do not use tox as the test function of setup.py. tox lives in the layer higher
than setup.py. Seemingly we may just specify `python setup.py test` as the test
command in the tox.ini. The requests package does this. I still do not
understand the `pip install -e .` command though. Dependencies are installed by
setup.py and tests just run without this command.

- https://github.com/requests/requests/blob/master/tox.ini

Resources
---------

- https://python-packaging.readthedocs.io/en/latest/metadata.html
- https://setuptools.readthedocs.io/en/latest/setuptools.html
- https://github.com/kennethreitz/setup.py
- https://docs.pytest.org/en/latest/goodpractices.html
