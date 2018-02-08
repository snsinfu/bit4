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


def bin_stream(source, bin_size, hard_end):
    class State:
        pass

    cur = State()
    cur.end = 0
    cur.sum = 0.0

    def pop_bin():
        bin_beg = cur.end - bin_size
        bin_end = cur.end
        bin_value = cur.sum / bin_size

        cur.end = min(cur.end + bin_size, hard_end)
        cur.sum = 0.0

        return bin_beg, bin_end, bin_value

    for beg, end, value in source:
        if beg >= end:
            raise BinStreamException('input contains degenerate or reverse interval')

        if beg < cur.end:
            raise BinStreamException('input is not sorted')

        if end > hard_end:
            raise BinStreamException('input overshoots')

        while beg >= cur.end:
            yield pop_bin()

        assert beg < cur.end

        while beg < end and cur.end - bin_size < end:
            slice_end = min(cur.end, end)
            cur.sum += (slice_end - beg) * value
            beg = slice_end

        if cur.end <= beg:
            yield pop_bin()


class BinStreamException(Exception):
    pass


if __name__ == '__main__':
    main()
