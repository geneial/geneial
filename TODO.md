# Roadmap

This is a maintained backlog, not a claim that every historical TODO is solved.
Inline TODOs remain useful implementation notes. Add reproducible failures and
acceptance criteria before turning broad ideas into changes.

## Completed in the 0.3 modernization

- [x] C++20, root CMake entry point, presets, install/exported package targets.
- [x] Boost-free library; Boost.Test limited to the test suite.
- [x] Compiler/sanitizer CI and installed static/shared consumer tests.
- [x] Seedable thread-local randomness without modulo bias.
- [x] Factory settings lifetime, continuity, blending, permutation, and interpolation fixes.
- [x] Negative/all-zero fitness selection and unique, bounded parent sampling.
- [x] Feasible N-point segments and bounded impossible mutation/replenishment requests.
- [x] Worker-pool draining, exception propagation, and shutdown validation.
- [x] Lambda fitness evaluator and repaired crossover lambda adapter.
- [x] Real criteria/coupling tests instead of success-only placeholders.
- [x] Six replayable C++ examples, including both requested genetic-art approaches
  and seeded random mazes, plus a live browser maze playground.
- [x] Sharp art references, separate configurable fitness resolution, residual-guided
  multiscale search, parallel color refinement, and deduplicated replay assets.
- [x] Dependency-free website source, gallery builder, and Pages workflow.
- [x] Replace promotional landing-page copy with a technical example index,
  run configurations, direct CSV/source links, and automated site-link checks.
- [x] Replace obsolete build, Docker, contribution, and API documentation.
- [x] Preserve distinct hash-colliding genomes; repair hash lookup, exact-pointer
  removal, finite-score validation, and transactional population replacement.
- [x] Repair random replacement budgets/elitism and duplicate candidate accounting
  in both replacement policies, with empty/small population regressions.
- [x] Repair stopping-window initialization, manager/age cache resets, population
  equality stopping, checked thresholds, and the unimplemented trailing stop.
- [x] Replace the ignored mutation test file with a registered suite; fix source
  cache invalidation, smoothing anchors/edge peaks, and bounded peak requests.
- [x] Bound/prune idle resource caches, release cached evaluators and fitness,
  reset recycled ages, and test allocation-failure cleanup and concurrent returns.
- [x] Bound diagnostic retention, count drops, avoid ignored-event allocation,
  and make timers safe after bookkeeper destruction and allocation failure.
- [x] Replace silent diagnostic analyzer no-ops with explicit retained-event
  reports and a labeled population snapshot; reject invalid facade lifecycles.
- [x] Reject criterion ownership cycles and reset shared criterion graphs once per
  node, including stateless wrappers; implement combined-criterion initialization.
- [x] Replace repeated duplicate-vector erasure with one stable compaction and
  reuse worker batches without retaining captures after a drained wait.
- [x] Exercise continuous factory full-width integral bounds and reachable anchors;
  saturate step arithmetic and validate finite ordered settings.
- [x] Validate smoothed crossover factory/settings types and degenerate dimensions.
- [x] Remove permutation `-1` sentinels; initialize explicit index universes, check
  representability/round limits, and skip empty/singleton shuffle work.
- [x] Add compiler-derived GCC line/branch coverage reports and a separate TSan
  preset/job. Local TSan validation is recorded; hosted Linux execution is pending.

## Next correctness and compatibility work

- [ ] Replace escaping mutable genome references with an edit guard or explicit
  immutable genome API; make cache invalidation impossible to bypass. Give
  population observers immutable snapshots: reusing an observed object after
  changing it to a hash-colliding genome cannot be detected from shared pointers.
- [ ] Add criterion cloning with explicit shared-child identity semantics. A clone
  must not accidentally share mutable windows with its source; graph reset alone
  is not an independent-search cloning API.
- [ ] Audit extreme integral bounds in numeric summaries and remaining legacy
  operators. Exercise full-range sums/ranges and signed/unsigned boundary inputs
  under UBSan; use checked/saturating arithmetic where appropriate.
- [ ] Preserve exact low-order integer gene changes during peak interpolation
  near 64-bit limits on hosts where `long double` has only double precision.
  Saturation is safe, but intermediate floating-point rounding can lose units.
- [ ] Validate remaining legacy operator/settings combinations beyond the covered
  uniform/swap/peak mutations and random/worst replacement policies.
- [ ] Review the first hosted GCC coverage report and Linux TSan/LSan results.
  Local compiler/runtime results are not a substitute for that platform matrix.
- [ ] Add a Windows/MSVC CI job before claiming Windows support.

## Design and performance

- [ ] Add an opt-in, bounded generation/fitness history for convergence analysis.
  The legacy `anaylseConvergence()` now reports a current snapshot explicitly;
  do not describe that snapshot or first-N retained events as a historical curve.
- [ ] Tournament/rank selection with pressure benchmarks against translated
  roulette weights; negative-fitness tests and unique-parent semantics required.
- [ ] Optional scaling with a documented fitness/evaluation-cache contract.
- [ ] Add optional byte-budgeted caches if real workloads need them; the current
  object-count limit and burst benchmark do not bound arbitrarily large genomes.
- [ ] Simplify the historical namespace/typedef/export machinery in a separate
  API-breaking change, preserving a documented migration path.
- [ ] Decouple coupling/crossover only if concrete operator use cases justify it.
- [x] Benchmark the hybrid art search against preserved pre-change binaries at
  equal raster dimensions, seed, shape counts and generation budgets.
- [ ] Consider SIMD or incremental
  raster updates without weakening deterministic serial/parallel behavior.
- [ ] Add perceptual color objectives and antialiased scoring as explicit options,
  not silent changes to the current RGB-MSE objective.
- [ ] More visual experiments: evolve soft-body walkers over terrain; synthesize
  reaction-diffusion patterns against a target; evolve orbital slingshot routes;
  and evolve compact logic circuits. Require real fitness histories, deterministic
  seeds, clear domain constraints and regression tests rather than canned animations.

## Documentation and release work

- [ ] Broaden API reference prose for legacy operators and diagnostics.
- [ ] Publish a reviewed 0.3 release/tag, reproducible source archive, and package
  manager recipes after downstream compatibility feedback.
- [ ] Enable GitHub Actions Pages in repository settings and verify custom-domain
  DNS/TLS separately; local source changes cannot repair account/domain settings.
