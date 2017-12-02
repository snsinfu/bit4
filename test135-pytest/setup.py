#!/usr/bin/env python

import os

from setuptools import find_packages, setup

name = 'foo'
requires = []
setup_requires = ['pytest-runner']
test_requires = ['pytest>=2.0']

here = os.path.abspath(os.path.dirname(__file__))

with open(os.path.join(here, 'README.rst'), encoding='utf8') as f:
    long_description = '\n' + f.read()

about = {}
with open(os.path.join(here, name, '__version__.py'), encoding='utf8') as f:
    exec(f.read(), about)

setup(
    name=name,
    version=about['__version__'],
    author='nobody',
    author_email='nobody@example.com',
    url='https://www.example.com/',
    description='Foo does something',
    long_description=long_description,
    classifiers=[
        'Development Status :: 3 - Alpha',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
    ],
    license='MIT',
    include_package_data=True,
    packages=find_packages(exclude=('tests',)),
    install_requires=requires,
    setup_requires=setup_requires,
    tests_require=test_requires,
)
