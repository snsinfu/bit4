Foo
===

Foo can be tested.

Testing
-------

Do not use tox as the test function of setup.py. tox lives in the layer higher
than setup.py. Also, tox seems to be kinda orthogonal to `setup.py test`, and
thus the test dependencies must be listed in tox.ini.

Resources
---------

- https://python-packaging.readthedocs.io/en/latest/metadata.html
- https://setuptools.readthedocs.io/en/latest/setuptools.html
- https://github.com/kennethreitz/setup.py
- https://docs.pytest.org/en/latest/goodpractices.html
