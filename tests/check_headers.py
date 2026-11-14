"""Compile each public header as the first include in an otherwise empty TU."""

import concurrent.futures
import pathlib
import subprocess
import sys


def main() -> None:
    source = pathlib.Path(__file__).resolve().parents[1] / "src/src/lib"
    generated = pathlib.Path(sys.argv[1]).resolve() / "src/src/lib/generated"
    compiler = sys.argv[2] if len(sys.argv) > 2 else "c++"

    def check(header: pathlib.Path) -> str:
        include = header.relative_to(source).as_posix()
        result = subprocess.run(
            [compiler, "-std=c++20", "-fsyntax-only", "-x", "c++", "-I", str(source), "-I", str(generated), "-"],
            input=f"#include <{include}>\n",
            text=True,
            capture_output=True,
            timeout=60,
        )
        return f"{include}\n{result.stderr}" if result.returncode else ""

    headers = sorted(source.rglob("*.h"))
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
        failures = [failure for failure in executor.map(check, headers) if failure]
    if failures:
        print("\n".join(failures), file=sys.stderr)
        raise SystemExit(1)
    print(f"{len(headers)} public headers compile independently")


if __name__ == "__main__":
    main()
