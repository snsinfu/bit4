import sys


ITEM_TIMESTEP = "ITEM: TIMESTEP"
ITEM_NUMBER_OF_ATOMS = "ITEM: NUMBER OF ATOMS"
ITEM_ATOMS = "ITEM: ATOMS"


def main():
    load_lammps_dump(sys.stdin)


def load_lammps_dump(src):
    for step, frame in every_timestep(src):
        line_count = 0
        for line in frame:
            line_count += 1
        print(step, line_count)


def every_timestep(src):
    scanner = Scanner(src)

    while scanner.text:
        if scanner.text != ITEM_TIMESTEP:
            raise Exception("timestep is required")
        if not scanner.scan():
            raise Exception("timestep has no value")

        step = int(scanner.text)
        yield step, scan_timestep(scanner)


def scan_timestep(scanner):
    while scanner.scan():
        if scanner.text == ITEM_TIMESTEP:
            break
        yield scanner.text


class Scanner:
    def __init__(self, src):
        self._src = iter(src)
        self._line = None
        self.scan()

    @property
    def text(self):
        return self._line

    def scan(self):
        try:
            self._line = next(self._src).strip()
        except StopIteration:
            self._line = None
            return False
        return True


if __name__ == "__main__":
    main()
