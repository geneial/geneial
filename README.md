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

Presets: `release`, `debug`, `sanitized` (AddressSanitizer + UBSan), and `library`
(no tests, examples, or Boost). All generated artifacts stay in `output/`.
The legacy `cmake -S src -B output/legacy` entry point still works.

## The evolution lab

Five real searches export self-contained HTML replays, editable SVGs, and CSV
fitness histories. No browser framework, network service, or plotting package.

| Executable | Experiment |
| --- | --- |
| `voronoi_art` | Minimalist portraits from colored cells; genes duplicate and disappear |
| `triangle_art` | Parallel EvoLisa: 50 layered translucent triangles by default |
| `star_tour` | Untangle a closed traveling-salesperson tour through a spiral galaxy |
| `queens` | Solve N-queens with a permutation genome and a conflict visualization |
| `pixel_art` | Discover a ringed planet from 768 palette-index genes |

```sh
mkdir -p output/art
./output/release/src/demo/voronoi_art --seed 42 --threads 4 \
  --generations 6000 --output output/art/cells
./output/release/src/demo/triangle_art --seed 42 --threads 4 \
  --shapes 50 --generations 12000 --output output/art/triangles
./output/release/src/demo/queens --size 20 --output output/art/queens
# Open any generated .html directly in a browser.
```

The portrait demos also accept `--input target.ppm` (P3/P6, 8-bit samples), and
export the exact fitness raster as PPM. Their bundled Mona Lisa target needs no
download. All demos accept `--help`. See [the example guide](docs/Demos.md) for
representations, options, tradeoffs, and source credits.

### Build the website locally

```sh
cmake --build --preset release --target website
python3 -m http.server 8765 --bind 127.0.0.1 --directory output/release/site
# Open http://127.0.0.1:8765
```

This evolves all five examples and assembles the gallery from `docs/site/`.
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
#include <numeric>

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

## Quality and reproducibility

```sh
cmake --preset sanitized
cmake --build --preset sanitized --parallel
ctest --preset sanitized --output-on-failure
python3 tests/check_examples.py output/release/src/demo
```

CI covers GCC, Clang with sanitizers, Apple Clang, library-only builds, installed
consumers, and exported artifacts. Random engines are thread-local and explicitly
seedable. Seeded results repeat within the same build and standard library;
cross-platform draw sequences are not promised. Portrait worker threads perform
no random draws; serial/parallel histories are checked for equality.

Read [architecture and safety contracts](docs/Overview.md),
[migration notes](docs/Migration.md), [contribution guidance](CONTRIBUTING.md),
[local validation results](docs/Validation.md), [the changelog](CHANGELOG.md),
and [the remaining roadmap](TODO.md).

## Credits and license

MIT licensed; see [LICENSE.md](LICENSE.md) and [AUTHORS.md](AUTHORS.md).
The new art experiments independently implement ideas from
[Sebastian Proost’s Voronoi genetic art](https://blog.4dcu.be/programming/2020/02/10/Genetic-Art-Algorithm-2.html)
and [Károly Zsolnai-Fehér’s parallel EvoLisa](https://users.cg.tuwien.ac.at/zsolnai/gfx/mona_lisa_parallel_genetic_algorithm/).
The public-domain portrait’s provenance is [documented separately](src/demo/showcase/assets/README.md).
