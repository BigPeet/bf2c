from pathlib import Path

from .py_bf2c import parse_file, parse_text, print_code, emit_to_file

__ALL__ = ["BF2C"]


class BF2C:
    def __init__(self, code: str | Path):
        if isinstance(code, str):
            self._rep = parse_text(code)
        elif isinstance(code, Path):
            if not code.exists():
                raise FileNotFoundError(f"No such file: '{code}'")
            self._rep = parse_file(str(code))
        else:
            raise TypeError("code must be a str or Path")

    def print(self) -> None:
        print_code(self._rep)

    def to_file(self, file_path: str | Path) -> None:
        if isinstance(file_path, Path):
            file_path = str(file_path)
        emit_to_file(self._rep, file_path)
