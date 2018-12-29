import os.path

script_dir = os.path.abspath(os.path.dirname(__file__))

clang_flags = [
    "-x", "c++",
    "-std=c++2a",
    "-pedantic-error",
    "-Werror",
    "-Wall",
    "-Wextra",
    "-Wconversion",
    "-Wsign-conversion",
    "-isystem", "/usr/include",
    "-isystem", "/usr/local/include",
    "-isystem", "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include",
    "-isystem", "./include",
]


def Settings(language, **kwargs):
    if language == "cfamily":
        return {"flags": clang_flags, "include_paths_relative_to_dir": script_dir}
    return {}
