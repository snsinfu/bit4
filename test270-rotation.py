import io


def main():
    output_cols = []

    tree = Node(5,
                Node(3),
                Node(8,
                     Node(6,
                          None,
                          Node(7)),
                     Node(9)))
    assert validate(tree)
    output_cols.append(build_tree_text(tree))

    tree = rotate(tree)
    assert validate(tree)
    output_cols.append(build_tree_text(tree))

    tree = rotate(tree)
    assert validate(tree)
    output_cols.append(build_tree_text(tree))

    print(stack_columns(output_cols, padding=(' ' * 6)), end='')


class Node:
    def __init__(self, value=None, left=None, right=None):
        self.value = value
        self.left = left
        self.right = right


def rotate(tree):
    pivot = tree.right
    tree.right = pivot.left
    pivot.left = tree
    return pivot


def validate(tree):
    if not tree:
        return False

    if tree.left:
        if tree.left.value > tree.value:
            return False
        if not validate(tree.left):
            return False

    if tree.right:
        if tree.right.value < tree.value:
            return False
        if not validate(tree.right):
            return False

    return True


def build_tree_text(tree):
    return write_tree(tree, io.StringIO()).getvalue()


def write_tree(tree, out):
    NULL_MARK = '○'
    NORMAL_MARKER = '├ '
    NORMAL_INDENT = '│ '
    LAST_MARKER = '└ '
    LAST_INDENT = '  '

    def recurse(node, indent='', next_indent='', node_marker=''):
        if not node:
            out.write(indent + node_marker + NULL_MARK + '\n')
            return

        out.write(indent + node_marker + str(node.value) + '\n')

        indent += next_indent
        recurse(node.left, indent, NORMAL_INDENT, NORMAL_MARKER)
        recurse(node.right, indent, LAST_INDENT, LAST_MARKER)

    recurse(tree)

    return out


def stack_columns(cols, padding=' '):
    line_cols = [list_lines(col) for col in cols]
    widths = [max(len(line) for line in col) for col in line_cols]
    height = max(len(col) for col in line_cols)

    just_cols = []
    for col, width in zip(line_cols, widths):
        lines = [line.ljust(width) for line in col]
        lines += [''] * (height - len(lines))
        just_cols.append(lines)

    output = ''
    for cells in zip(*just_cols):
        output += padding.join(cells) + '\n'

    return output


def list_lines(text):
    lines = text.split('\n')
    if lines and lines[-1] == '':
        lines = lines[:-1]
    return lines


if __name__ == '__main__':
    main()
