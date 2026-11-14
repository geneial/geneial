"""Check authored gallery pages against the generated website, without dependencies."""

from html.parser import HTMLParser
from pathlib import Path
import sys
from urllib.parse import unquote, urlsplit


class Page(HTMLParser):
    def __init__(self, source: str) -> None:
        super().__init__()
        self.identifiers: set[str] = set()
        self.references: list[str] = []
        self.headings = 0
        self.main_sections = 0
        self.feed(source)

    def handle_starttag(self, tag: str, attributes: list[tuple[str, str | None]]) -> None:
        values = dict(attributes)
        if identifier := values.get("id"):
            assert identifier not in self.identifiers, f"Duplicate ID: {identifier}"
            self.identifiers.add(identifier)
        if tag == "h1":
            self.headings += 1
        if tag == "main":
            self.main_sections += 1
        if tag == "img":
            assert values.get("alt"), "Image is missing alternative text"
        if tag == "button":
            assert values.get("type") in ("button", "submit", "reset"), "Button needs an explicit type"
        for attribute in ("href", "src"):
            if reference := values.get(attribute):
                self.references.append(reference)


def main() -> None:
    root = Path(sys.argv[1]).resolve()
    pages = {name: Page((root / name).read_text()) for name in ("index.html", "guide.html", "maze.html")}
    count = 0
    for name, page in pages.items():
        assert page.headings == page.main_sections == 1, name
        for reference in page.references:
            url = urlsplit(reference)
            if url.scheme or url.netloc:
                continue
            destination = (root / unquote(url.path or name)).resolve()
            assert destination.is_relative_to(root), f"{name}: link escapes site: {reference}"
            assert destination.is_file(), f"{name}: missing file: {reference}"
            if url.fragment:
                relative = destination.relative_to(root).as_posix()
                target = pages.get(relative) or Page(destination.read_text())
                assert unquote(url.fragment) in target.identifiers, f"{name}: missing anchor: {reference}"
            count += 1
    print(f"Three authored pages: {count} local links/assets resolve; landmarks, IDs, images and buttons checked")


if __name__ == "__main__":
    main()
