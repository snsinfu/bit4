#!/usr/bin/env python

import io
import os

from setuptools import find_packages, setup

name = 'binner'
requires = []
setup_requires = ['pytest-runner']
test_requires = ['pytest>=2.0']
scripts = ['bin/binner']

here = os.path.abspath(os.path.dirname(__file__))

with io.open(os.path.join(here, 'README.rst'), encoding='utf8') as f:
    long_description = '\n' + f.read()

about = {}
with io.open(os.path.join(here, name, '__version__.py'), encoding='utf8') as f:
    exec(f.read(), about)

setup(
    name=name,
    version=about['__version__'],
    author='snsinfu',
    author_email='snsinfu@gmail.com',
    url='https://github.com/snsinfu/bit4/test263-binner',
    description='Pulse binner',
    long_description=long_description,
    classifiers=[
        'Development Status :: 3 - Alpha',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
    ],
    license='MIT',
    include_package_data=True,
    scripts=scripts,
    packages=find_packages(exclude=('tests',)),
    install_requires=requires,
    setup_requires=setup_requires,
    tests_require=test_requires,
)
