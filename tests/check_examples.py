"""Exercise exports and serial/parallel repeatability without third-party modules."""

import csv
import pathlib
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ET


def main() -> None:
    binaries = pathlib.Path(sys.argv[1]).resolve()
    with tempfile.TemporaryDirectory(prefix="geneial-examples-") as directory:
        root = pathlib.Path(directory)
        for name in ("star_tour", "queens", "pixel_art", "voronoi_art", "triangle_art"):
            prefix = root / name
            command = [str(binaries / name), "--generations", "40", "--seed", "42", "--output", str(prefix)]
            if name.endswith("_art") and name != "pixel_art":
                command.extend(["--threads", "1"])
            subprocess.run(command, check=True, capture_output=True, timeout=120)
            with prefix.with_suffix(".csv").open() as stream:
                rows = list(csv.DictReader(stream))
            assert len(rows) >= 2, name
            scores = [float(row["fitness"]) for row in rows]
            assert all(after >= before for before, after in zip(scores, scores[1:])), name
            ET.parse(prefix.with_suffix(".svg"))
            html = prefix.with_suffix(".html").read_text()
            assert "<template" in html and "<script src=" not in html, name
            if name in ("voronoi_art", "triangle_art"):
                serial = prefix.with_suffix(".csv").read_bytes()
                command[-1] = "4"
                subprocess.run(command, check=True, capture_output=True, timeout=120)
                assert prefix.with_suffix(".csv").read_bytes() == serial, name
                assert prefix.with_suffix(".ppm").read_text().startswith("P3\n"), name
                for option, value in (("--threads", "0"), ("--shapes", "3"), ("--input", "missing.ppm")):
                    assert subprocess.run([str(binaries / name), option, value], capture_output=True, timeout=10).returncode != 0
    print("Five exports and parallel repeatability verified")


if __name__ == "__main__":
    main()
