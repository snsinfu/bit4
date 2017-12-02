import pytest

from foo.calc import add, mul

ADD_TESTDATA = [
    (1, 2, 3),
    (3, 4, 7),
]


@pytest.mark.parametrize(['x', 'y', 'expected'], ADD_TESTDATA)
def test_add(x, y, expected):
    assert add(x, y) == expected


MUL_TESTDATA = [
    (1, 2, 2),
    (3, 4, 12),
]


@pytest.mark.parametrize(['x', 'y', 'expected'], MUL_TESTDATA)
def test_mul(x, y, expected):
    assert mul(x, y) == expected
