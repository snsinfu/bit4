def translate(lst, tr):
    if isinstance(lst, str):
        return lst.translate(tr)
    return [translate(x, tr) for x in lst]

CARRY_TEMPLATE = [('IIII', 'V'), ('IVI', 'V'), ('VIV', 'IX'), ('IXI', 'X')]
CARRY_TABLE = ( translate(CARRY_TEMPLATE, str.maketrans('IVX', 'IVX'))
              + translate(CARRY_TEMPLATE, str.maketrans('IVX', 'XLC'))
              + translate(CARRY_TEMPLATE, str.maketrans('IVX', 'CDM')) )

def next_roman(roman):
    roman += 'I'
    for pattern, replacement in CARRY_TABLE:
        if roman.endswith(pattern):
            roman = roman[:-3] + replacement
    return roman

roman = ''
for n in range(1, 1001):
    roman = next_roman(roman)
    print(roman)
