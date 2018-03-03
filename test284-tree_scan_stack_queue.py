def depth_first_search(node, callback):
    stack = [node]
    while stack:
        value, children = stack.pop()
        callback(value)
        stack.extend(children[::-1])


def breadth_first_search(node, callback):
    queue = [node]
    while queue:
        value, children = queue[0]
        callback(value)
        queue = queue[1:] + children


tree = ('usr', [('bin',   [('awk',         []),
                           ('env',         []),
                           ('sed',         [])]),
                ('lib',   [('libc.so',     []),
                           ('libiconv.so', []),
                           ('libz.so',     [])]),
                ('share', [('dict',        [('words', [])]),
                           ('misc',        [('ascii', [])]),
                           ('man',         [('man1',  [('awk.1', []),
                                                       ('env.1', []),
                                                       ('sed.1', [])])])])])

print('=== DFS ===')
depth_first_search(tree, print)
print('')
print('=== BFS ===')
breadth_first_search(tree, print)
