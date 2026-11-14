# Modernization validation — 2026-11-13

These are observed local results, not a claim that the new remote CI matrix has
already run. Generated build products and galleries are intentionally ignored.

## Toolchains and automated checks

Validation host: macOS 26.4.1, Apple Silicon. CMake 4.3.3 and Ninja were used.

| Check | Observed result |
| --- | --- |
| Apple Clang 21.0.0, release preset | 21/21 CTest entries pass |
| Apple Clang 21.0.0, AddressSanitizer + UBSan | 21/21 CTest entries pass |
| GCC 16.2.0 with libstdc++, release | 21/21 CTest entries pass |
| Independent public headers | All 80 compile with both compilers |
| Boost-free library preset | Builds and installs |
| Relocated installed package | Static and shared consumers pass, 2/2 |
| Embedded `add_subdirectory` consumer without Boost | 1/1 passes |
| Legacy `cmake -S src` library-only entry point | Builds without Boost |
| Example artifact checker | All five HTML/SVG/CSV exports pass |
| Portrait repeatability | One-worker/four-worker CSV histories match exactly |
| Doxygen 1.18.0 | API reference builds without warnings |
| Workflow and source checks | `actionlint`, showcase clang-format 18, `git diff --check` pass |

GCC uses `GENEIAL_HEADER_ONLY_TESTS=ON` locally because the Homebrew Boost.Test
binary uses libc++, while GCC uses libstdc++. This is a test-linkage option, not
a library dependency. See [contribution guidance](../CONTRIBUTING.md).

The 21 CTest entries include 11 unit/regression executables and ten showcase
smoke/invalid-input checks. Additional Python checks validate standalone headers,
SVG XML, monotonic fitness histories, exports, invalid art options, and parallel
repeatability. The installed package was moved to a different directory before
configuring its downstream consumers.
Installed sanitizer-enabled static and shared consumers also pass (2/2);
sanitizer compile/link requirements propagate through both exported targets.

## Actual evolutionary-art runs

Both gallery runs use seed 42, population 64, four fitness workers, and the bundled
40 × 60 Mona Lisa target. Fitness is negative RGB mean squared error (MSE).

| Experiment | Configuration | Initial MSE | Final MSE |
| --- | --- | ---: | ---: |
| Voronoi art | 24 initial sites; duplication/deletion; 6,000 generations | 1,741.01 | 407.002 |
| Translucent triangles | 50 layers; 12,000 generations | 2,554.50 | 200.583 |

These are examples, not performance benchmarks or guaranteed convergence claims.
Draw sequences are only repeatable within the same build/standard library.
Run `cmake --build --preset release --target website` to recreate the complete
five-example gallery with HTML replay controls and fitness plots.

Desktop and 390-pixel mobile layouts were inspected in a browser. Local navigation,
portrait replay buttons and sliders, image loading, horizontal overflow, and
browser errors were checked. Both art replays advanced through their recorded
generations without console errors.

## Not verified or published

- The GitHub-hosted Linux/macOS CI jobs require a push; they have not run here.
- Docker cannot be built locally without a running daemon; only the source-based
  Docker workflow has been updated.
- Windows/MSVC and ThreadSanitizer are not validated.
- Pages deployment, repository settings, custom-domain DNS, and TLS are
  administrative follow-up work. A local commit does not publish the website.
- The remaining correctness/design backlog is explicit in [the roadmap](../TODO.md).
