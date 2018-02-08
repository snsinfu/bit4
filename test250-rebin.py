import os
import signal
import sys


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    source = [
        (64, 65, 1.0),
        (65, 67, 4.0),
        (67, 68, 5.0),
        (68, 70, 6.0),
    ]

    for b in bin_stream(source, bin_size=5, hard_end=100):
        print(b)


class Bin:
    def __init__(self, beg=None, end=None, value=0.0):
        self.beg = beg
        self.end = end
        self.value = value


def bin_stream(source, bin_size, hard_end):
    cur = Bin(0, min(bin_size, hard_end))

    def pop_bin():
        bin_beg = cur.beg
        bin_end = cur.end
        bin_value = cur.value / (cur.end - cur.beg)

        cur.beg += bin_size
        cur.end = min(cur.end + bin_size, hard_end)
        cur.value = 0.0

        return bin_beg, bin_end, bin_value

    for beg, end, value in source:
        if end <= beg:
            raise BinStreamException('input contains degenerate or reverse interval')

        if end <= cur.beg:
            raise BinStreamException('input is not sorted')

        if end > hard_end:
            raise BinStreamException('input overshoots')

        # The bin may be behind the input

        while cur.end <= beg:
            yield pop_bin()

        # Now the bin overlaps with the input

        while cur.end <= end:
            cur.value += value * bin_size
            yield pop_bin()

        # Now the bin overshoots the input

        if cur.beg < end:
            cur.value += value * (end - cur.beg)

    while cur.beg < hard_end:
        yield pop_bin()


class BinStreamException(Exception):
    pass


if __name__ == '__main__':
    main()
