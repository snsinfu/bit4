import argparse
import os.path
import re


RE_LOCAL_INCLUDE = re.compile('#include "(.+?)"')
TEMPLATE_BUNDLE_MARKER = "// BUNDLE //"


def main():
    run(**parse_args())


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--template", type=str, default=None, help="Output template")
    parser.add_argument("root", type=str, help="Root header")
    return vars(parser.parse_args())


def run(root, template):
    root_header = Header(root)
    headers = load_headers(root_header.depend_paths)

    for header in headers.values():
        header.resolve_depends(headers)

    bundle = merge(headers.values())

    if template:
        print(render_template(template, bundle))
    else:
        print(bundle)


def load_headers(paths, headers=dict()):
    for path in paths:
        if path in headers:
            continue
        header = Header(path)
        headers[path] = header
        load_headers(header.depend_paths, headers)
    return headers


def render_template(template, bundle):
    with open(template) as file:
        lines = [line.rstrip() for line in file]

    output = []
    for line in lines:
        if line == TEMPLATE_BUNDLE_MARKER:
            output.append(bundle)
        else:
            output.append(line)

    return "\n".join(output)


def merge(headers, resolved=set()):
    output = ""

    for header in headers:
        if header in resolved:
            continue

        if header.depends:
            output += merge(header.depends)

        output += header.make_output() + "\n"
        resolved.add(header)

    return output


class Header:
    def __init__(self, path):
        lines = load_code_lines(path)

        self._path = path
        self._code_lines = lines

        rel_depends = extract_local_includes(lines)
        basedir = os.path.dirname(path)
        self.depend_paths = [os.path.normpath(os.path.join(basedir, dep)) for dep in rel_depends]

    def resolve_depends(self, headers):
        self.depends = [headers[path] for path in self.depend_paths]

    def make_output(self):
        header = [
            "",
            "//--------------------------------------------------------------------------",
            "",
        ]
        return "\n".join(header + remove_local_includes(self._code_lines))


def remove_local_includes(lines):
    output = []
    for line in lines:
        if RE_LOCAL_INCLUDE.match(line):
            output = strip_empty(output)
        else:
            output.append(line)
    return output


def extract_local_includes(lines):
    depends = []
    for line in lines:
        match = RE_LOCAL_INCLUDE.match(line)
        if match:
            depends.append(match.group(1))
    return depends


def load_code_lines(path):
    with open(path) as file:
        lines = [line.rstrip() for line in file]

    beg = index_if(lines, lambda s: s.startswith("#define")) + 1
    end = rindex_if(lines, lambda s: s.startswith("#endif"))

    return strip_empty(lines[beg:end])


def strip_empty(texts):
    beg = index_if(texts, len)
    end = rindex_if(texts, len) + 1
    return texts[beg:end]


def index_if(texts, pred):
    for i, text in enumerate(texts):
        if pred(text):
            return i
    return None


def rindex_if(texts, pred):
    for i, text in enumerate(texts[::-1]):
        if pred(text):
            return len(texts) - i - 1
    return None


if __name__ == "__main__":
    main()
