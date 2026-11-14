# Architecture and contracts

## Search pipeline

`SteadyStateAlgorithm<Score>` owns a manager, which owns a population, a chromosome
factory, execution strategy, settings, and bookkeeper. Each generation:

1. Notify `BEFORE_GENERATION` observers and increment population/chromosome ages.
2. Select parents, favoring higher fitness.
3. Couple parents; crossover produces offspring.
4. Mutate offspring, leaving parents untouched.
5. Evaluate and insert distinct offspring, then remove the worst excess members.
6. Replenish missing members and evaluate the stopping criterion.
7. Notify `AFTER_GENERATION` observers.

Observers execute on the solver thread. Custom operators should return new
chromosomes, not modify indexed parents. Search is heuristic, not an optimality
proof. Finite stopping criteria are strongly recommended.

## Fitness

The library maximizes scores. For a distance or error objective, return its
negative. `FunctionFitnessEvaluator<Value, Score>` accepts a callable taking a
read-only `std::span<const Value>`. Existing polymorphic `FitnessEvaluator<Score>`
implementations remain supported, including custom `Fitness` subclasses.

Fitness is lazily cached. Mutable container/iterator access, value replacement,
and evaluator changes invalidate that cache. Do not retain mutable references
across evaluations: later writes through an old reference cannot be intercepted.
Do not mutate a chromosome after inserting it into a population; its fitness and
hash are already indexed externally. Remove it first or create a new chromosome.
Return finite scores; roulette selection rejects nonfinite weights.

## Genomes and settings

`MultiValueChromosome<Value, Score>` owns a vector of arithmetic values. A
`MultiValueChromosomeFactory` creates fixed-length random vectors. The continuous
factory constrains neighboring differences; the permutation factory preserves a
gene set. Factory settings are shared between a builder and its built factories,
and their lifetime is owned—not borrowed from the builder. Editing builder
settings still updates its existing factories. Configure them before starting a
search; concurrent settings changes are unsupported.

Factories recycle chromosome allocations. A chromosome can outlive its factory;
its deleter falls back to ordinary deletion if the resource pool has gone away.
Pool free-list access is synchronized, but the rest of the search manager is not
a general-purpose concurrently mutable container.

The variable-length Voronoi demo uses custom shape-aware crossover and mutation.
It explicitly resizes allocated chromosomes. Built-in fixed-width operators
reject dimensions they cannot safely process.

## Selection and replacement

Fitness-proportional and roulette selection sample without replacement. Weights
are `1 + fitness - min(0, worstFitness)`, preserving positive weights even for
negative, zero, or integral scores. This translation is part of the 0.3 contract;
it is not invariant under adding an arbitrary constant to every objective.
Fitness-proportional selection reserves a configurable number of elites first.
Uniform selection ignores scores.

Replacement keeps the highest-scoring population members after inserting
candidates. Requested offspring counts and elites must fit the population.
Population uniqueness is currently indexed by genome hashes; hash collisions can
still discard distinct genomes. Equality itself compares complete values, but
collision buckets in population indexes remain a tracked limitation.

Replenishment throws after a configurable number of consecutive duplicate
attempts (`PopulationSettings::setMaxReplenishAttempts`, default 10000), rather
than looping forever when the requested population exceeds the search space.

## Parallel execution

`SequentialExecutionManager` is the default. Set a `ThreadedExecutionManager` to
evaluate independent offspring fitness tasks concurrently during batch insertion.
The C++20 pool owns `std::jthread` workers, batches tasks, and drains its queue
before returning from `waitForTasks()` or joining at destruction. The first task
exception is rethrown by `waitForTasks()` after the entire batch drains; a later
batch can still be submitted. Explicitly wait to observe errors—destructors cannot
report unobserved exceptions.

- Evaluators must be reentrant; capture immutable targets or synchronize state.
- Never concurrently evaluate the same uncached chromosome, mutate shared
  chromosomes, recursively wait on your own executor, or shut down from a worker.
- Submit work and call `joinAll()` from the owning thread; simultaneous shutdown
  calls are not supported. Submission after shutdown throws.
- Public population mutation and solver operations remain single-owner-thread APIs.

## Randomness

`Random` uses a thread-local `std::mt19937`. `initialize(seed)` resets the calling
thread’s engine, while `initialize()` reseeds nondeterministically. Integral
sampling includes both endpoints; floating sampling includes the lower endpoint
and excludes the upper endpoint (an equal-bound range returns that bound).
Invalid ranges, nonfinite floating bounds, and probabilities outside `[0,1]`
throw `std::invalid_argument`.

Draw sequences are reproducible within the same build and standard library.
Standard distribution implementations may differ across platforms. If custom
worker tasks use randomness, seed each worker deliberately; a solver-thread seed
does not initialize other threads. The portrait examples avoid worker RNG entirely.

## Build layout

The root CMake project and legacy `src/` entry point build the same targets.
`geneial::geneial` aliases the static library; `geneial::geneial_shared` provides
the shared variant. Installed CMake packages propagate C++20 and thread linkage.
Boost is only a test dependency. Doxygen’s optional `doc` target writes API HTML
under the build directory’s `src/doc/html` (or `doc/html` for the legacy entry point).
