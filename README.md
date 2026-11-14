# GeneiAL

**Composable genetic algorithms for C++20.** Evolve routes, solve constraints,
and paint portraits with independently replaceable operators.

![C++ CI](https://github.com/geneial/geneial/actions/workflows/ci.yml/badge.svg)

## Build

Requirements: a C++20 compiler/library with `std::span` and `std::jthread`, CMake 3.21+, Ninja.
Use GCC 12+ or Clang 15+ with libstdc++ 12+, LLVM/libc++ 20+, or a current Xcode
26 toolchain. Older libc++/Apple libraries may lack `jthread` despite accepting C++20;
CMake checks these features at configuration time.
Boost.Test is needed for tests only. The library depends only on the standard
library and system threads. MSVC is not currently in the validation matrix.

```sh
# macOS: brew install cmake ninja boost
# Ubuntu: sudo apt-get install build-essential cmake ninja-build libboost-test-dev
git clone https://github.com/geneial/geneial.git
cd geneial
cmake --preset release
cmake --build --preset release --parallel
ctest --preset release --output-on-failure
```

Presets: `release`, `debug`, `sanitized` (AddressSanitizer + UBSan), `tsan`
(ThreadSanitizer, no examples), `coverage` (GCC), and `library` (no tests,
examples, or Boost). All generated artifacts stay in `output/`.
The legacy `cmake -S src -B output/legacy` entry point still works.

## Example searches

Six real C++ searches export self-contained HTML replays, editable SVGs, and CSV
fitness histories. No browser framework, network service, or plotting package.

| Executable | Experiment |
| --- | --- |
| `voronoi_art` | Minimalist portraits from colored cells; genes duplicate and disappear |
| `triangle_art` | Parallel EvoLisa: 50 layered translucent triangles by default |
| `star_tour` | Untangle a closed traveling-salesperson tour through a spiral galaxy |
| `queens` | Solve N-queens with a permutation genome and a conflict visualization |
| `pixel_art` | Discover a ringed planet from 768 palette-index genes |
| `maze` | Evolve valid escape routes through a seeded random labyrinth |

```sh
mkdir -p output/art
./output/release/src/demo/voronoi_art --seed 42 --threads 4 \
  --max-shapes 256 --generations 6000 --output output/art/cells
./output/release/src/demo/triangle_art --seed 42 --threads 4 \
  --shapes 96 --generations 12000 --output output/art/triangles
./output/release/src/demo/queens --size 20 --output output/art/queens
./output/release/src/demo/maze --seed 42 --size 18 --output output/art/maze
# Open any generated .html directly in a browser.
```

The portrait demos also accept `--input target.ppm` (P3/P6, 8-bit samples), and
export the exact fitness raster as PPM. Their bundled Mona Lisa target needs no
download. A sharp 396 × 599 reference is independent of the fitness raster
(`--resolution 32..256`, default 128-pixel longest edge). Error-guided growth,
multiscale mutations and analytical color fitting make the portraits **hybrid
genetic searches**, rather than blind color jitter. Replay files embed the target
only once instead of copying it into every keyframe.

The gallery also includes a **live random-maze playground** at `maze.html`:
new-maze/seed controls, animated populations, single stepping, speed and prefix
exploration controls, and an optional fitness-field overlay. It runs a real
JavaScript GA with the same maze topology as the native example—not C++ compiled
to WebAssembly or a prerecorded animation. All native demos accept `--help`.
See [the example guide](docs/Demos.md) for
representations, options, tradeoffs, and source credits.

### Build the website locally

```sh
cmake --build --preset release --target website
python3 -m http.server 8765 --bind 127.0.0.1 --directory output/release/site
# Open http://127.0.0.1:8765
```

This evolves all six examples and assembles the gallery from `docs/site/`.
The Pages workflow also builds API docs with Doxygen. An administrator must set
the repository’s GitHub Pages source to **GitHub Actions** before deployment.
Local builds and commits do not publish the site or change domain settings.

## Use the library

```sh
cmake --preset library
cmake --build --preset library
cmake --install output/library --prefix "$HOME/.local"
```

```cmake
find_package(geneial 0.3 CONFIG REQUIRED)
target_link_libraries(your_program PRIVATE geneial::geneial)
# Or geneial::geneial_shared.
```

Write objectives as C++20 callables instead of implementing a fitness subclass:

```cpp
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <memory>
#include <numeric>
#include <span>

auto evaluator = std::make_shared<geneial::FunctionFitnessEvaluator<double>>(
    [](std::span<const double> genes) {
        return -std::inner_product(genes.begin(), genes.end(), genes.begin(), 0.0);
    });
geneial::population::chromosome::MultiValueChromosomeFactory<double, double>::Builder builder(evaluator);
builder.getSettings().setNum(8);
auto factory = builder.create();
auto candidate = factory->createChromosome();
auto score = candidate->getFitness().get();
```

GeneiAL **maximizes** fitness: negate costs for minimization. Factories own their
settings, so they can safely outlive a local builder. Compose a steady-state
algorithm with selection, coupling, crossover, mutation, replacement, and a
stopping criterion; the [showcase sources](src/demo/showcase) are complete examples.
The [installed-package consumer](tests/consumer) verifies static and shared linking.

### Safety contracts

- Return **finite** scores. Population insertion rejects NaN/infinity and null
  candidates before indexing them; failed fitness evaluation does not delete members.
- Population uniqueness compares complete genomes within hash-collision buckets.
  `hashExists()` tests a bucket, not identity; use `containsChromosome()` for
  genome membership. `getChromosomeByHash()` returns any matching member or null.
- Treat indexed members as immutable. Remove a chromosome before editing it, and
  do not retain mutable references across fitness/hash evaluations. Populations
  are manager-bound, noncopyable, single-owner-thread containers.
- Random replacement preserves population size and protects elites; worst-first
  replacement keeps the best candidates up to the configured maximum. Both
  deduplicate offspring before applying their replacement budget.
- Stateful stopping criteria sample once per observed generation. They reset
  when the manager changes or its age rewinds. Call `reset()` on the root
  criterion for a same-generation restart of the entire reachable graph,
  including shared children. Built-in composites reject ownership cycles.
- Population-unchanged stopping retains observed chromosomes. Keep those genomes
  immutable until reset, even if removed from the current population.
- Idle chromosome caches retain at most 256 objects per factory by default.
  Set `setResourcePoolCapacity(0)` to disable reuse, or call `clearResourcePool()`
  after a burst. Cached chromosomes release their fitness and evaluator captures.
- Default diagnostics do not allocate event payloads. `StatisticBookkeeper`
  retains the first 4096 events by default, exposes dropped-event counts, and can
  be cleared or resized. Its event view and collection are owner-thread APIs.

See [operator and stopping semantics](docs/Overview.md) and
[migration notes](docs/Migration.md) before upgrading existing searches.

## Quality and reproducibility

```sh
cmake --preset sanitized
cmake --build --preset sanitized --parallel
ctest --preset sanitized --output-on-failure
python3 tests/check_examples.py output/sanitized/src/demo
python3 tests/check_headers.py output/sanitized
# Focus on the follow-up audit's regression suites:
ctest --preset sanitized -R '(criteria|memory|factory_boundaries)_test|runtime_memory_contracts'
./output/release/src/src/tests/runtime_probe --benchmark
```

The suite contains 27 CTest entries: 16 unit/regression executables, an allocation
and failure-injection probe, and ten showcase smoke/invalid-input checks. The
memory/performance audit adds 20 Boost.Test cases on top of the earlier 33-case
correctness audit. Coverage includes ownership cycles, expired diagnostic timers,
bounded caches, allocation failures, worker capture lifetimes, and 64-bit factory
boundaries. Test counts are not a line-coverage percentage; the GCC `coverage`
preset produces compiler-derived line/branch reports in CI.

CI is configured for GCC, Clang with address/undefined/leak sanitizers, Apple
Clang, a separate Linux ThreadSanitizer job, coverage reports, library-only builds,
installed consumers, and exported test/benchmark artifacts. Project warnings
are errors in CI. Random engines are thread-local and explicitly
seedable. Seeded results repeat within the same build and standard library;
cross-platform draw sequences are not promised. Portrait worker threads perform
no random draws; serial/parallel histories are checked for equality.

Read [architecture and safety contracts](docs/Overview.md),
[migration notes](docs/Migration.md), [contribution guidance](CONTRIBUTING.md),
[local validation results](docs/Validation.md), [the changelog](CHANGELOG.md),
and [the remaining roadmap](TODO.md).

Important remaining work includes an edit-guard API for mutable genomes, numeric
extreme-value audits in legacy summaries/operators, criterion cloning, and Windows
coverage. Local checks do not establish the hosted Linux sanitizer results. These limitations are
tracked explicitly rather than described as completed work.

## Credits and license

MIT licensed; see [LICENSE.md](LICENSE.md) and [AUTHORS.md](AUTHORS.md).
The new art experiments independently implement ideas from
[Sebastian Proost’s Voronoi genetic art](https://blog.4dcu.be/programming/2020/02/10/Genetic-Art-Algorithm-2.html)
and [Károly Zsolnai-Fehér’s parallel EvoLisa](https://users.cg.tuwien.ac.at/zsolnai/gfx/mona_lisa_parallel_genetic_algorithm/).
The public-domain portrait’s provenance is [documented separately](src/demo/showcase/assets/README.md).
