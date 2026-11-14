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
  equality. Population hash buckets now preserve distinct colliding genomes;
  `Population::hash_map` is an `unordered_multimap`, not an `unordered_map`.
  Use `containsChromosome()` for genome membership; hash lookup returns any match.
- Populations are noncopyable/nonmovable because their manager association and
  indexes cannot safely be copied independently. Whole-population replacement is
  staged, so invalid fitness does not destroy the original population.
- Population insertion rejects null candidates and nonfinite fitness. Removing
  nonmembers and setting null manager dependencies throw instead of dereferencing
  invalid pointers. Both indexes remain consistent after rejection.
- Roulette/fitness-proportional weights are translated into strictly positive
  values; negative and all-zero scores are supported. Parents are unique.
- N-point crossover uses nonempty segments with feasible minimum widths and no
  rejection loop. Equidistant cuts no longer begin with an empty segment.
- Continuous generation checks differences from the previous generated value,
  not the previous interval bound. Blending honors the continuity limit.
- XOR stopping criteria now implement XOR; tolerance criteria bound absolute
  distance on both sides. Empty combined criteria stop; malformed ones throw.
- Random replacement no longer deletes members without children, violates
  elitism, or inserts more children than its budget. Both replacement policies
  deduplicate before taking a candidate prefix. The worst-first duplicate
  diagnostic counts rejected duplicates across the entire submitted batch.
- Fix-point windows require at least two observations; the default `minDiff`
  changes from one to two. Stateful criteria reset on manager changes/age rewinds
  and expose `reset()` for explicit restarts. Consecutive means window hit count,
  not necessarily adjacent hits. Trailing stop now implements absolute drawdown
  instead of silently returning false forever. Integral fitness differences use
  exact unsigned-distance arithmetic, including adjacent scores near 64-bit limits.
  Population-unchanged observations retain references, so previously observed
  genomes must stay immutable until reset, even after removal from the population.
- Smoothing uses the starting anchor only when enabled and safely handles empty
  genomes, numeric boundaries, and tapered edge peaks. Smooth-peak mutation
  rejects unsigned genes and invalid elevations/counts/dimensions.
- Thread pools drain tasks, propagate exceptions at `waitForTasks()`, reject
  zero workers/batch sizes, and reject submissions after shutdown.

## New APIs and examples

`FunctionFitnessEvaluator<Value, Score>` wraps a callable accepting
`std::span<const Value>`. Existing evaluators still work. Five interactive
examples, including variable-length Voronoi genomes and parallel translucent
triangle art, demonstrate customization without changing the library core.

See [the architecture contracts](Overview.md) for cache, ownership, threading,
randomness, and still-open limitations.
