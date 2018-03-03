import time

max_number = 200
print_interval = 0.1
roman = ''

for n in range(1, max_number + 1):
    roman += 'I'

    if roman.endswith('IIII'):
        roman = roman[:-3] + 'V'

    if roman.endswith('IVI'):
        roman = roman[:-3] + 'V'

    if roman.endswith('VIV'):
        roman = roman[:-3] + 'IX'

    if roman.endswith('IXI'):
        roman = roman[:-3] + 'X'

    if roman.endswith('XXXX'):
        roman = roman[:-3] + 'L'

    if roman.endswith('XLX'):
        roman = roman[:-3] + 'L'

    if roman.endswith('LXL'):
        roman = roman[:-3] + 'XC'

    if roman.endswith('XCX'):
        roman = roman[:-3] + 'C'

    # Now I can see the pattern!
    # The first four ifs are for IVX.
    # The next four ifs are for XLC.
    # Then, another four ifs for CDM should follow.

    print('\r{:3d} - {:20s}'.format(n, roman), end='')
    time.sleep(print_interval)

print('')
