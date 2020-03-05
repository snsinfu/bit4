def _test_match(x, pattern):
    for key, value in pattern.items():
        if key not in x:
            return False
        if x[key] != value:
            return False
    return True


def select_where(xs, **kwargs):
    result = []
    for x in xs:
        if _test_match(x, kwargs):
            result.append(x)
    return result


def select_unless(xs, **kwargs):
    result = []
    for x in xs:
        if not _test_match(x, kwargs):
            result.append(x)
    return result


class FilterModule:
    def filters(self):
        return {"where": select_where, "unless": select_unless}
