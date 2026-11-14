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
Return finite scores; population insertion rejects NaN and infinity before
indexing fitness. Failed batch fitness evaluation leaves both indexes unchanged.

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
Pool free-list access is synchronized. Idle caches default to 256 chromosomes
per factory, not a byte limit: large gene vectors can still consume substantial
memory. `setResourcePoolCapacity(n)` immediately prunes excess idle objects;
`clearResourcePool()` releases all idle objects and their free-list storage.
Neither operation invalidates outstanding chromosomes. Returning a chromosome
clears its fitness and evaluator reference; retrieving it resets its age to zero.
Cache allocation failure deletes the returned object rather than escaping its
shared-pointer deleter. Configure factory settings on the owner thread; the
rest of the search manager is not a concurrently mutable container.

Continuous generation uses saturating step intervals at integral type limits.
An enabled starting anchor may lie outside the gene interval if its first step
can reach that interval; disjoint intervals and nonfinite/negative steps throw,
even when requesting unpopulated storage. Permutations initialize the explicit
universe `0..N-1`; random gene bounds are irrelevant, not `-1` sentinels. Empty
and singleton universes skip shuffling, and types unable to represent every
index distinctly are rejected before allocation. Shuffle round limits must be
ordered; excessively large counts still request correspondingly large work.

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

Worst-first replacement keeps the highest-scoring population members after
inserting candidates, up to the configured population maximum. Random replacement
instead exchanges old nonelite members for unique offspring and preserves the
current population size. Both policies clamp their budget to available nonelite
members, requested children, and unique candidates; oversized elitism is a no-op.
Duplicate offspring do not consume unique replacement slots. Random replacement
evaluates candidates before deleting any old members. The worst-first
`REPLACEMENT_FAILED_INSERTS` diagnostic includes duplicate offspring filtered from
the entire submitted batch, not only a truncated candidate prefix.

Population uniqueness compares genome equality within hash-collision buckets;
distinct genomes sharing a hash coexist. `getHashMap()` exposes a read-only
`unordered_multimap` view, not a one-to-one identity map. `hashExists()` reports
whether any member occupies a bucket; `containsChromosome()` checks genome
membership. `getChromosomeByHash()` returns an arbitrary matching member or null.
Removal identifies the exact stored pointer and throws for nonmembers rather
than erasing another colliding genome. Custom chromosomes must give equal genomes
equal hashes and implement a symmetric equality relation.

Populations cannot be copied or moved independently of their managers.
`replacePopulation()` stages and evaluates a replacement before swapping indexes;
invalid candidates leave the original population and input container unchanged.
Batch duplicate filtering preserves accepted order and compacts once instead of
erasing the vector for each rejected candidate. Its hash/equality costs still
depend on the genome size and collision distribution.

Replenishment throws after a configurable number of consecutive duplicate
attempts (`PopulationSettings::setMaxReplenishAttempts`, default 10000), rather
than looping forever when the requested population exceeds the search space.

## Stopping criteria and smoothing

Stateful criteria cache one observation per manager/generation. Repeated queries
do not advance their windows. Switching managers or rewinding the population age
resets their own state; restarting at the same age requires an explicit `reset()`
on the root. This visits every reachable criterion exactly once, even through
shared children and stateless negation wrappers. Automatic observation resets
remain local so one branch cannot reset a child already sampled by another.
Skipped generations count as one observation, not fabricated samples.
Built-in composites reject null children and direct/indirect ownership cycles
before modifying the graph. `CombinedCriterion` also accepts an initial vector
of glue/criterion pairs. Custom composites must expose `getChildren()` and call
`validateChild()` before taking ownership; custom state belongs in `resetState()`.

- `FixPointCriterion` compares oldest/newest best fitness in its bounded window.
  Counts must be whole numbers of at least two; its default minimum is two
  observations, avoiding an immediate stop on the first sample. A decline is not
  a fix point. Improvement is compared without signed-integer subtraction overflow.
- `ConsecutiveDecorator` retains its historical **hit count within a window**
  semantics; hits need not be adjacent. Null criteria and impossible windows throw.
- `PopulationUnchangedCriterion` compares complete genome membership, not age
  extrema. Its first observation is false; two empty observations are unchanged.
  It retains chromosome references, not deep snapshots: keep observed genomes
  immutable until reset, even after removal. A changed hash is detected, but an
  in-place edit with the same hash cannot reconstruct the old genome.
- `TrailingStopCriterion(maximumDrawdown)` now stops on a positive absolute drop
  from its best observed fitness that reaches the tolerance. Zero tolerance stops
  on any decline, not a tie. Thresholds must be finite and nonnegative.

Smoothing respects the optional starting anchor and step/bound constraints. With
no anchor, the first clamped gene is preserved. Peaks taper linearly on each side,
include the last gene, clip safely at genome edges, and saturate the numeric value
type. `SmoothPeakMutationOperation` requires signed or floating genes; unsigned
genes cannot represent its negative elevations. Mutation reads preserve the input
chromosome's fitness/hash caches and create separate result chromosomes.
Smoothed N-point crossover requires a continuous factory and continuous settings;
incompatible factories and infeasible dimensions throw instead of unchecked casts.

## Diagnostics and retention

`DefaultBookkeeper` ignores events without allocating their value payloads.
`StatisticBookkeeper(capacity = 4096)` retains the first capacity events and
reports subsequent drops through `getDroppedEventCount()`. This is a sample of
the beginning of a run, not a rolling window or whole-run aggregate. Call
`clear()` to release events/reset the drop count, or `setCapacity(n)` to change
the limit. Shrinking discards unspecified retained entries and counts them as
drops; zero disables collection. Capacity bounds event count, not payload bytes.

`getEvents()` returns a read-only container view. A scoped timer holds a weak
reference and may safely outlive its bookkeeper; allocation failure during timer
destruction counts a dropped event rather than throwing. Timing reports skip
null, incompatible, negative, or nonfinite duration payloads. Raw
`traceEvent(name, EventData*)` transfers ownership immediately, including on
failure; custom bookkeepers must adopt the pointer before allocating. Event
collection, capacity changes, and views require the owner thread, not fitness
worker threads. Use external synchronization if custom workers collect data.

Attach `Diagnostics` before starting an algorithm; null algorithms, late
attachment, or a replaced nonstatistical bookkeeper now throw instead of silently
failing or dereferencing null. Existing statistical bookkeepers keep their
configured capacity. The event analyzers report retained occurrence counts,
sorted by name for their `COUPLING_`, `CROSSOVER_`, `MUTATION_`, `REPLACEMENT_`,
and `SELECTION_` prefixes. Counts are not sums of payload values or total-run
estimates; an absent group explicitly reports no retained events. The legacy
`anaylseConvergence()` spelling remains, but its output is labeled as the current
generation/population/best/worst snapshot, not an unrecorded convergence history.

## Parallel execution

`SequentialExecutionManager` is the default. Set a `ThreadedExecutionManager` to
evaluate independent offspring fitness tasks concurrently during batch insertion.
The C++20 pool owns `std::jthread` workers, batches tasks, and drains its queue
before returning from `waitForTasks()` or joining at destruction. Completed task
captures are released before a drained wait returns; per-worker batch storage is
reused between batches. The first task
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
