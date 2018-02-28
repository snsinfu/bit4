# BFS, with depth!


def breadth_first_search(node, cb):
    sentinel = object()

    queue = [node, sentinel]
    depth = 0

    while len(queue) > 1:
        node = queue[0]
        queue = queue[1:]

        if node is sentinel:
            depth += 1
            queue.append(sentinel)
            continue

        value, children = node
        cb(depth, value)

        queue += children

    assert queue == [sentinel]


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

breadth_first_search(tree, print)
