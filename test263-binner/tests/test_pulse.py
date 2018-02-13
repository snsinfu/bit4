import pytest

from binner import BinnerException, bin_pulses


def test_normal_example():
    src = [
        (0, 4, 1.0),
        (4, 6, 2.0),
        (7, 9, 1.0),
    ]
    expected = [
        (0, 5, 1.2),
        (5, 9, 1.0),
    ]
    assert list(bin_pulses(src, bin_size=5, hard_end=9)) == expected


def test_single_bin():
    src = [
        (0, 4, 1.0),
        (4, 6, 2.0),
        (7, 9, 1.0),
    ]
    expected = [
        (0, 9, 10.0 / 9),
    ]
    assert list(bin_pulses(src, bin_size=10, hard_end=9)) == expected


def test_empty_input_nonempty_output():
    src = []
    expected = [
        (0, 5, 0.0),
        (5, 9, 0.0),
    ]
    assert list(bin_pulses(src, bin_size=5, hard_end=9)) == expected


def test_no_hard_end():
    src = [
        (0, 4, 1.0),
        (4, 6, 2.0),
        (7, 9, 1.0),
    ]
    expected = [
        (0, 5, 1.2),
        (5, 10, 0.8),
    ]
    assert list(bin_pulses(src, bin_size=5)) == expected


def test_empty_input_empty_output():
    assert list(bin_pulses([], bin_size=5)) == []


def test_invalid_input():
    with pytest.raises(BinnerException):
        list(bin_pulses([(0, 0, 0.0)], bin_size=5))

    with pytest.raises(BinnerException):
        list(bin_pulses([(2, 0, 0.0)], bin_size=5))

    with pytest.raises(BinnerException):
        list(bin_pulses([(3, 6, 0.0), (0, 2, 0.0)], bin_size=5))

    with pytest.raises(BinnerException):
        list(bin_pulses([(0, 11, 0.0)], bin_size=5, hard_end=10))
