# BFS, with depth!


def depth_first_search(node, cb, depth=0):
    if node is None:
        return

    value, children = node
    cb(depth, value)

    for child in children:
        depth_first_search(child, cb, depth + 1)


def breadth_first_search(node, cb):
    queue = [node]
    depth = 0
    breadth = 1

    while queue:
        value, children = queue[0]
        cb(depth, value)
        queue = queue[1:] + children

        breadth -= 1
        if breadth == 0:
            breadth = len(queue)
            depth += 1


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

print('DFS:')
depth_first_search(tree, print)

print('\nBFS:')
breadth_first_search(tree, print)
