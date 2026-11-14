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

CI adds Linux GCC/Clang, Apple Clang, ASan/UBSan, artifact checks, and installed
consumer coverage. A green local run does not replace that platform matrix.
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
