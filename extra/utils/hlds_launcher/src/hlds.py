#!/usr/bin/env python3

assert "__main__" == __name__


def _run():
    import os
    import sys
    import pathlib

    _directory = pathlib.Path.cwd().resolve(strict = True)

    _executable = _directory / "hlds_linux"
    assert _executable.resolve(strict = True) == _executable
    assert _executable.is_file()

    try: _ld_library_path = os.environ["LD_LIBRARY_PATH"]
    except KeyError: _ld_library_path = "."
    else:
        _ld_library_path = tuple(_ld_library_path.split(":"))
        if "." not in _ld_library_path: _ld_library_path = ".", *_ld_library_path
        _ld_library_path = ":".join(_ld_library_path)
    os.environ.update(LD_LIBRARY_PATH = _ld_library_path)
    del _ld_library_path
    os.chdir(_directory)
    os.execv(_executable.as_posix(), (f"./{_executable.name}", *sys.argv[1:]))
    raise RuntimeError("invalid state")


try: _run()
finally: del _run
