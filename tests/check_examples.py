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
        for name in ("star_tour", "queens", "pixel_art", "voronoi_art", "triangle_art", "maze"):
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
                assert scores[-1] > scores[0], name
                assert html.count("data:image/bmp;base64,") == 1, name
                assert "data-asset=" in html, name
                assert prefix.with_suffix(".html").stat().st_size < 4_000_000, name
                assert "data:image/bmp;base64," in prefix.with_suffix(".svg").read_text(), name
                serial = prefix.with_suffix(".csv").read_bytes()
                command[-1] = "4"
                subprocess.run(command, check=True, capture_output=True, timeout=120)
                assert prefix.with_suffix(".csv").read_bytes() == serial, name
                assert prefix.with_suffix(".ppm").read_text().startswith("P3\n"), name
                for option, value in (("--threads", "0"), ("--shapes", "3"), ("--shapes", "257"), ("--resolution", "31"), ("--resolution", "257"), ("--max-shapes", "257"), ("--input", "missing.ppm")):
                    assert subprocess.run([str(binaries / name), option, value], capture_output=True, timeout=10).returncode != 0
            if name == "maze":
                assert "EXIT FOUND" in prefix.with_suffix(".svg").read_text(), name
                first = prefix.with_suffix(".csv").read_bytes()
                subprocess.run(command, check=True, capture_output=True, timeout=120)
                assert prefix.with_suffix(".csv").read_bytes() == first, name
                for seed in ("0", "1", "7", "4294967295"):
                    probe = command.copy()
                    probe[probe.index("--seed") + 1] = seed
                    probe[probe.index("--generations") + 1] = "200"
                    subprocess.run(probe, check=True, capture_output=True, timeout=120)
                    assert "EXIT FOUND" in prefix.with_suffix(".svg").read_text(), seed
                for size in ("3", "33"):
                    assert subprocess.run([str(binaries / name), "--size", size], capture_output=True, timeout=10).returncode != 0
        bounded = root / "bounded-keyframes"
        subprocess.run(
            [str(binaries / "pixel_art"), "--generations", "159", "--population", "20", "--output", str(bounded)],
            check=True, capture_output=True, timeout=120,
        )
        assert bounded.with_suffix(".html").read_text().count('<template class="frame"') <= 82
    print("Six exports, bounded replay frames, reference deduplication, and seeded/parallel repeatability verified")


if __name__ == "__main__":
    main()
