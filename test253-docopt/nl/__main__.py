"""Number lines.

Usage:
  nl [-o FILE] [INPUT ...]

  -o FILE, --output FILE  Specify output file. [default: -]
"""

import gzip
import signal
import sys

import docopt


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    options = docopt.docopt(__doc__)

    output_file = sys.stdout
    if options['--output'] != '-':
        output_file = open(options['--output'], 'w')

    if not options['INPUT']:
        number_lines(sys.stdin, output_file)

    for input_path in options['INPUT']:
        number = 1
        with open(input_path) as input_file:
            number = number_lines(input_file, output_file, init=number)


def number_lines(input_file, output_file, init=1):
    number = init
    for line in input_file:
        output_file.write('{:d}\t{:s}'.format(number, line))
        number += 1
    return number


if __name__ == '__main__':
    main()
