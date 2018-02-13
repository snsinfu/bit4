import pytest

from binner import BinnerException, bin_series


def test_normal_example():
    src = [
        ('a', 0, 4, 1.0),
        ('a', 4, 6, 2.0),
        ('a', 7, 9, 3.0),
        ('b', 2, 3, 4.0),
        ('b', 5, 6, 5.0),
    ]
    sizes = {
        'a': 9,
        'b': 7,
    }
    expected = [
        ('a', 0, 5, 1.2),
        ('a', 5, 9, 2.0),
        ('b', 0, 5, 0.8),
        ('b', 5, 7, 2.5),
    ]
    assert list(bin_series(src, bin_size=5, series_sizes=sizes)) == expected


def test_anonymous_series():
    src = [
        ('a', 0, 4, 1.0),
        ('a', 4, 6, 2.0),
        ('a', 7, 9, 3.0),
        ('b', 2, 3, 4.0),
        ('b', 5, 6, 5.0),
    ]
    expected = [
        ('a', 0, 5, 1.2),
        ('a', 5, 10, 1.6),
        ('b', 0, 5, 0.8),
        ('b', 5, 10, 1.0),
    ]
    assert list(bin_series(src, bin_size=5)) == expected
