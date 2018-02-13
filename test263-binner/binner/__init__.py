import itertools


class Bin:
    def __init__(self, beg=None, end=None, value=0.0):
        self.beg = beg
        self.end = end
        self.value = value


class BinnerException(Exception):
    pass


def bin_pulses(pulses, bin_size, hard_end=None):
    """Rebin pulses into uniform bins.
    """

    def cap_end(end):
        return min(end, hard_end) if hard_end else end

    def pop_bin():
        bin_beg = cur.beg
        bin_end = cur.end
        bin_value = cur.value / (cur.end - cur.beg)

        cur.beg += bin_size
        cur.end = cap_end(cur.end + bin_size)
        cur.value = 0.0

        return bin_beg, bin_end, bin_value

    cur = Bin(0, cap_end(bin_size))
    pulse_count = 0

    for beg, end, value in pulses:
        if end <= beg:
            raise BinnerException(
                'input contains degenerate or reverse interval')

        if end <= cur.beg:
            raise BinnerException('input is not sorted')

        if end > cap_end(end):
            raise BinnerException('input overshoots')

        while cur.end <= beg:
            yield pop_bin()

        while cur.end < end:
            cur.value += value * (cur.end - max(cur.beg, beg))
            yield pop_bin()

        if cur.beg < end:
            cur.value += value * (end - max(cur.beg, beg))

        pulse_count += 1

    if not hard_end:
        if pulse_count > 0:
            hard_end = (cur.end + bin_size - 1) // bin_size * bin_size
        else:
            hard_end = 0

    while cur.beg < hard_end:
        yield pop_bin()


def bin_series(src, bin_size, series_sizes=None):
    """Rebin tagged pulses into uniform bins.
    """
    for tag, series in itertools.groupby(src, lambda rec: rec[0]):

        pulses = (rec[1:] for rec in series)
        hard_end = series_sizes[tag] if series_sizes else None
        bins = bin_pulses(pulses, bin_size, hard_end)

        for beg, end, value in bins:
            yield tag, beg, end, value
