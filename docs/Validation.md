# Modernization validation — 2026-11-14

These are observed local results, not a claim that the new remote CI matrix has
already run. Generated build products and galleries are intentionally ignored.

## Toolchains and automated checks

Validation host: macOS 26.4.1, Apple Silicon. CMake 4.3.3 and Ninja were used.

| Check | Observed result |
| --- | --- |
| Apple Clang 21.0.0, release preset, warnings as errors | 27/27 CTest entries pass |
| Apple Clang 21.0.0, AddressSanitizer + UBSan | 27/27 CTest entries pass |
| Apple Clang 21.0.0, ThreadSanitizer | 17/17 CTest entries pass (examples disabled) |
| GCC 16.2.0 with libstdc++, release, warnings as errors | 27/27 CTest entries pass |
| GCC 16.2.0, compiler coverage build | 27/27 CTest entries pass |
| Independent public headers | All 80 compile with both compilers |
| Boost-free library preset | Builds and installs |
| Relocated installed package | Static and shared consumers pass, 2/2 |
| Embedded `add_subdirectory` consumer without Boost | 1/1 passes |
| Legacy `cmake -S src` library-only entry point | Builds without Boost |
| Example artifact checker | All five HTML/SVG/CSV exports pass |
| Portrait repeatability | One-worker/four-worker CSV histories match exactly |
| Doxygen 1.18.0 | API reference builds without warnings |
| Workflow and source checks | `actionlint`, changed C++ clang-format 18, `git diff --check` pass |
| Local Markdown links | All relative file links resolve in 13 tracked documents |

GCC uses `GENEIAL_HEADER_ONLY_TESTS=ON` locally because the Homebrew Boost.Test
binary uses libc++, while GCC uses libstdc++. This is a test-linkage option, not
a library dependency. See [contribution guidance](../CONTRIBUTING.md).

The 27 CTest entries include 16 unit/regression executables, an allocation and
failure-injection probe, and ten showcase smoke/invalid-input checks.
Additional Python checks validate standalone headers,
SVG XML, monotonic fitness histories, exports, invalid art options, and parallel
repeatability. The installed package was moved to a different directory before
configuring its downstream consumers.
Installed sanitizer-enabled static and shared consumers also pass (2/2);
sanitizer compile/link requirements propagate through both exported targets.

## Follow-up correctness audit

The follow-up audit adds 33 active regression cases across population, replacement,
mutation, and criteria tests. The previously ignored mutation source referenced
retired APIs and was not built; it is replaced by a registered test executable.

- Deliberately colliding hashes now coexist. A regression failed before the fix,
  and instantiating the old hash lookup also reproduced a compilation error.
- Collision-bucket removal, duplicate batches, failed parallel evaluation,
  transactional whole-population replacement, and null manager dependencies are
  exercised explicitly. Equal fitness and equal hashes do not confuse identity;
  repeated pointers remain unique even for nonreflexive NaN-containing genomes.
- Both replacement policies have budget/duplicate tests; random replacement also
  covers empty children, oversized elitism, empty/singleton populations, and
  invalid fitness without prior deletion.
- Criteria cover once-per-generation caching, window warm-up, manager changes,
  age rewinds, explicit reset, full-genome population comparisons, finite limits,
  integer-overflow boundaries, and the newly implemented trailing stop. Signed
  and unsigned 64-bit adjacent-score comparisons reproduced seven failures before
  replacing lossy floating-point conversions with exact integer differences.
- Mutation tests cover source/cache ownership, swap multisets, unchosen/zero-point
  behavior, smoothing anchors and integer limits, symmetric edge peaks, impossible
  counts/dimensions, and very large peak widths.

These are regression-case counts, distinct from the measured coverage below.

## Memory and performance audit

This audit adds 20 Boost.Test cases (eleven memory/diagnostic/observer, six factory boundary,
and three criterion graph cases), plus the standalone `runtime_memory_contracts`
CTest. The latter injects allocation failures into payload recording, idle-cache
growth, and scoped-timer destruction without weakening compiler warnings.

- A self-referencing criterion retained itself before the fix: the weak ownership
  probe reported `cycle_rejected=0 ownership_released=0`. The same probe now reports
  `cycle_rejected=1 ownership_released=1`. Regressions cover indirect cycles too.
- Scoped timers may outlive their bookkeeper; invalid names and failed event-name
  allocation release transferred payloads. Timing output skips incompatible data.
  The diagnostic facade rejects null/late attachment and replaced bookkeepers;
  event reports and empty/populated snapshots replace silent analyzer no-ops.
- The best-chromosome observer no longer reads an uninitialized baseline before
  its first notification; negative scores, ties, improvements, and declines are covered.
- Idle caches are bounded and prunable, do not retain old evaluator captures,
  release objects outside their mutex, and recover from failed cache allocation.
  Concurrent return/retrieval tests and worker capture-release tests pass TSan.
- Continuous factories handle full-width signed/unsigned 64-bit steps without
  overflow, reject unreachable/nonfinite settings, and preserve reachable external
  anchors. Degenerate permutations skip even `UINT_MAX` configured shuffle rounds;
  unrepresentable index universes and incompatible smoothed factories throw.

### Reproducible observations

Run `output/release/src/src/tests/runtime_probe --benchmark` for JSON results.
The hook counts ordinary C++ `new`/`new[]`, not aligned allocations or total RSS.
The before binary was compiled from the pre-audit implementation on the same host;
before timings are individual observations, after timings are medians of five.

| Workload | Before | After |
| --- | ---: | ---: |
| 100,000 ignored value events | 100,000 allocations | 0 allocations |
| Filter 10,000 existing duplicate pointers | 25.18 ms | 0.051 ms |
| Filter 40,000 existing duplicate pointers | 335.62 ms | 0.218 ms |
| Filter 80,000 existing duplicate pointers | 1326.51 ms | 0.444 ms |
| Idle chromosomes after a 10,000-object burst | Unbounded retention | 256 (default capacity) |

The duplicate benchmark isolates the old repeated-erasure path, not end-to-end
solver throughput or adversarial hash collisions. Timings depend on machine/load;
CI records observations without flaky timing thresholds. The cache limit counts
objects, not bytes; large genomes still require explicit capacity tuning.

### Compiler-derived coverage

GCC 16.2.0 / gcov 16, gcovr 8.6, the `coverage` preset, and all 27 entries report
87.4% line coverage (2249/2573) and 76.6% branch coverage (1324/1728) locally.
Functions report 81.1% (991/1222). The scope is compiled library implementation
and template instances, not tests, demos, dependencies, or generated configuration;
uninstantiated templates are not evidence of tested behavior. Template lines are
merged and compiler-internal-looking namespace functions explicitly included.
Unreachable and exception-only branches are excluded. See the exact commands in
[the contribution guide](../CONTRIBUTING.md#compiler-coverage).

CI enforces initial 75% line / 60% branch floors and archives Cobertura XML, JSON,
and annotated HTML reports. GCC versions/platforms can produce different counts;
the first hosted report still needs review. Coverage builds get a 120-second
showcase smoke-test budget because unoptimized raster profiling exceeded the
ordinary 30-second budget locally; release/sanitizer budgets are unchanged.

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

The original November 13 gallery's desktop and 390-pixel mobile layouts were
inspected in a browser. Local navigation,
portrait replay buttons and sliders, image loading, horizontal overflow, and
browser errors were checked. Both art replays advanced through their recorded
generations without console errors.

## Not verified or published

- The GitHub-hosted Linux/macOS CI jobs require a push; they have not run here.
- Docker cannot be built locally without a running daemon; only the source-based
  Docker workflow has been updated.
- Windows/MSVC is not validated. TSan passed on this macOS host; hosted Linux
  TSan and explicit LSan jobs have not run. The native `leaks` tool reported a
  restricted/non-debuggable process, so its summary is not treated as proof of
  leak absence; ownership probes, failure injection, and sanitizers are reported
  separately rather than conflated. Explicit `ASAN_OPTIONS=detect_leaks=1` on this
  Apple runtime aborts with `detect_leaks is not supported on this platform`.
- Pages deployment, repository settings, custom-domain DNS, and TLS are
  administrative follow-up work. A local commit does not publish the website.
- The remaining correctness/design backlog is explicit in [the roadmap](../TODO.md).
