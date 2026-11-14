# Contributing

Start with a focused issue or patch explaining the failure, expected behavior,
and compatibility impact. Include regression coverage and update affected docs.
Use C++20 and the [coding guidelines](CODING-GUIDELINES.md).

```sh
cmake --preset release
cmake --build --preset release --parallel
ctest --preset release --output-on-failure
cmake --preset sanitized
cmake --build --preset sanitized --parallel
ctest --preset sanitized --output-on-failure
python3 tests/check_examples.py output/release/src/demo
git diff --check
```

Also test a Boost-free build and installed consumers when touching public headers,
build rules, dependencies, or packaging:

```sh
cmake --preset library -DCMAKE_DISABLE_FIND_PACKAGE_Boost=ON
cmake --build --preset library
cmake --install output/library --prefix "$PWD/output/install"
cmake -S tests/consumer -B output/consumer -G Ninja \
  -DCMAKE_PREFIX_PATH="$PWD/output/install"
cmake --build output/consumer
ctest --test-dir output/consumer --output-on-failure
```

For showcase changes, format with clang-format 18, build the `website` target,
and visually check desktop/mobile layouts, replay/slider controls, and console
errors. Avoid checked-in generated galleries and image processing dependencies.
`doc` is available when Doxygen is installed. Source docs live in `docs/`; update
the short website guide when a documented user workflow changes.

The Dockerfile builds the local checkout and runs tests, rather than cloning a
possibly different remote revision. It needs a running Docker daemon:

```sh
docker build -t geneial .
docker run --rm geneial --help
```

CI adds Linux GCC/Clang, Apple Clang, ASan/UBSan/LSan, separate Linux TSan,
compiler-derived coverage, artifact checks, and installed consumer tests.
Project warnings are errors (`-DGENEIAL_WARNINGS_AS_ERRORS=ON`); this option does
not leak warning flags into installed consumers. A green local run does not
replace that platform matrix. Linux ASan jobs explicitly enable leak detection;
on other hosts, check runtime support rather than assuming it ran.
Public headers are independently compiled with `python3 tests/check_headers.py output/release`.
When mixing GCC/libstdc++ with a Boost.Test binary built for libc++, configure
with `-DGENEIAL_HEADER_ONLY_TESTS=ON` to use Boost.Test headers instead.
Actions are pinned to immutable commits and maintained through Dependabot.
Embedded consumers default to no tests/examples; validate with
`cmake -S tests/embedded -B output/embedded -G Ninja` and build/test that directory.
Git revision macros default to `unknown` for source-archive/reproducible builds;
opt in with `-DGENEIAL_EMBED_GIT_METADATA=ON` when needed.
Do not introduce new success-only test placeholders. Track unfinished work with
concrete acceptance criteria in `TODO.md` rather than silently removing it.

## Memory, concurrency, and performance checks

```sh
cmake --preset tsan -DGENEIAL_WARNINGS_AS_ERRORS=ON
cmake --build --preset tsan --parallel
TSAN_OPTIONS=halt_on_error=1 ctest --preset tsan
./output/release/src/src/tests/runtime_probe --verify-memory
./output/release/src/src/tests/runtime_probe --benchmark
```

The probe asserts allocation-free ignored events and injects allocation failures
into event recording, cache growth, and timer destruction. Its allocation hook
counts ordinary C++ `new`/`new[]`, not aligned allocations or process-wide RSS.
Benchmarks emit JSON observations; timing thresholds are deliberately not CI gates.
Pair ownership changes with explicit lifetime regressions, not just a leak-tool
summary. Do not share uncached chromosomes or unsynchronized diagnostics between
workers; see [threading contracts](docs/Overview.md#parallel-execution).

## Compiler coverage

Use GCC and a matching `gcov` executable. Keep coverage, ASan/UBSan, and TSan in
separate build trees; CMake rejects incompatible combinations.

```sh
CXX=g++ cmake --preset coverage -DGENEIAL_WARNINGS_AS_ERRORS=ON
cmake --build --preset coverage --parallel
ctest --preset coverage --parallel 2
python3 -m venv output/coverage-tools
output/coverage-tools/bin/pip install gcovr==8.6
output/coverage-tools/bin/gcovr output/coverage --root . --filter 'src/src/lib/geneial/' \
  --include-internal-functions --merge-lines \
  --exclude-unreachable-branches --exclude-throw-branches \
  --fail-under-line 75 --fail-under-branch 60 \
  --cobertura-pretty --cobertura=output/coverage/coverage.xml \
  --json-summary-pretty --json-summary=output/coverage/summary.json \
  --html-details=output/coverage/index.html --print-summary
```

For Homebrew GCC use `CXX=g++-16`, `GENEIAL_HEADER_ONLY_TESTS=ON`, and add
`--gcov-executable gcov-16` to gcovr. The filter includes library headers/templates
and implementation files while excluding tests, demos, dependencies, and generated
configuration. The historical `__geneial_` namespace looks compiler-internal to
gcovr: `--include-internal-functions` is essential to count the actual library.
`--merge-lines` combines template instances at each source line. Unreachable and
exception-only branches are excluded explicitly. Initial CI floors are 75% lines
and 60% branches, below the measured local baseline; review compiler differences
against the first hosted report rather than silently lowering the floors.
Inspect uncovered paths rather than equating a test count with coverage. CI
archives report files only, not instrumented objects or coverage data trees.
