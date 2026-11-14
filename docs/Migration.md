# Migrating from 0.2 to 0.3

0.3 deliberately raises the minimum language version from C++11 to C++20.
Rebuild the library and every consumer; binary compatibility is not promised.
This is a development version, not a claim of a published release.

## Build changes

- Configure from the repository root; `-S src` still works.
- Use CMake 3.21+, target-based usage requirements, and installed
  `find_package(geneial 0.3 CONFIG REQUIRED)`.
- The standard library must provide `std::span` and `std::jthread`; updating only
  compiler language flags is insufficient. CMake probes compile/link support.
- Link `geneial::geneial` or `geneial::geneial_shared` instead of manually
  assembling include paths and `-pthread`/`-std` flags.
- Boost is no longer a library dependency. Disable tests with
  `-DBUILD_TESTING=OFF` (the legacy `-DUNITTEST=OFF` also works).
- Obsolete Travis automation, C++11 feature probes, remote-cloning Ubuntu 16.04
  Docker builds, and interactive Eclipse generation have been retired.

## Behavioral corrections

- Random draws now use a thread-local Mersenne Twister and unbiased standard
  distributions. Old `rand()` sequences are intentionally not preserved.
  Explicitly seed `Random::initialize(uint32_t)` for reproducible experiments.
- Missing builder factories, invalid ranges, bad probabilities, exhausted
  replenishment, impossible crossover/mutation dimensions, and empty population
  queries throw standard exceptions. Catch by reference, not exception pointer.
- Factories own settings formerly borrowed from a builder. Existing builder
  updates still affect factories; finish configuration before running a search.
- Mutable genome/iterator access and evaluator changes invalidate fitness as
  well as hashes. Treat inserted population members as immutable.
- `getMin()`/`getMax()` now return const iterators, matching their const methods.
  Empty ranges return `end()`; `getRange()` returns zero for an empty genome.
- Chromosome equality compares values rather than treating hash equality as
  equality. Population hash-collision handling is still a roadmap item.
- Roulette/fitness-proportional weights are translated into strictly positive
  values; negative and all-zero scores are supported. Parents are unique.
- N-point crossover uses nonempty segments with feasible minimum widths and no
  rejection loop. Equidistant cuts no longer begin with an empty segment.
- Continuous generation checks differences from the previous generated value,
  not the previous interval bound. Blending honors the continuity limit.
- XOR stopping criteria now implement XOR; tolerance criteria bound absolute
  distance on both sides. Empty combined criteria stop; malformed ones throw.
- Thread pools drain tasks, propagate exceptions at `waitForTasks()`, reject
  zero workers/batch sizes, and reject submissions after shutdown.

## New APIs and examples

`FunctionFitnessEvaluator<Value, Score>` wraps a callable accepting
`std::span<const Value>`. Existing evaluators still work. Five interactive
examples, including variable-length Voronoi genomes and parallel translucent
triangle art, demonstrate customization without changing the library core.

See [the architecture contracts](Overview.md) for cache, ownership, threading,
randomness, and still-open limitations.
