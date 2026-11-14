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
- [x] Five replayable examples, including both requested genetic-art approaches.
- [x] Dependency-free website source, gallery builder, and Pages workflow.
- [x] Replace obsolete build, Docker, contribution, and API documentation.
- [x] Preserve distinct hash-colliding genomes; repair hash lookup, exact-pointer
  removal, finite-score validation, and transactional population replacement.
- [x] Repair random replacement budgets/elitism and duplicate candidate accounting
  in both replacement policies, with empty/small population regressions.
- [x] Repair stopping-window initialization, manager/age cache resets, population
  equality stopping, checked thresholds, and the unimplemented trailing stop.
- [x] Replace the ignored mutation test file with a registered suite; fix source
  cache invalidation, smoothing anchors/edge peaks, and bounded peak requests.

## Next correctness and compatibility work

- [ ] Replace escaping mutable genome references with an edit guard or explicit
  immutable genome API; make cache invalidation impossible to bypass. Give
  population observers immutable snapshots: reusing an observed object after
  changing it to a hash-colliding genome cannot be detected from shared pointers.
- [ ] Add graph-wide stopping-criterion reset/clone semantics, including shared
  stateful children. Same-age restarts currently require resetting each instance.
- [ ] Audit extreme integral bounds in continuous factories and numeric summaries.
  Exercise `lastValue +/- epsilon`, full-range sums/ranges, and signed/unsigned
  boundary inputs under UBSan; use checked/saturating arithmetic where appropriate.
- [ ] Preserve exact low-order integer gene changes during peak interpolation
  near 64-bit limits on hosts where `long double` has only double precision.
  Saturation is safe, but intermediate floating-point rounding can lose units.
- [ ] Exercise smoothed crossover with incompatible factory/settings types and
  degenerate dimensions; replace unchecked settings downcasts with validated APIs.
- [ ] Replace permutation settings' `-1` sentinel convention with an explicit
  mode; add tests for empty/duplicate gene universes and bounded shuffle rounds.
- [ ] Validate remaining legacy operator/settings combinations beyond the covered
  uniform/swap/peak mutations and random/worst replacement policies.
- [ ] Establish compiler-derived line/branch coverage reporting and a reviewed
  baseline in CI; regression case counts alone are not coverage percentages.
- [ ] Test ThreadSanitizer on a supported Linux runner; document any unsupported
  evaluator sharing patterns with executable examples.
- [ ] Add a Windows/MSVC CI job before claiming Windows support.

## Design and performance

- [ ] Tournament/rank selection with pressure benchmarks against translated
  roulette weights; negative-fitness tests and unique-parent semantics required.
- [ ] Optional scaling with a documented fitness/evaluation-cache contract.
- [ ] Resource-pool pruning with memory high-water benchmarks.
- [ ] Simplify the historical namespace/typedef/export machinery in a separate
  API-breaking change, preserving a documented migration path.
- [ ] Decouple coupling/crossover only if concrete operator use cases justify it.
- [ ] Benchmark art at larger fitness resolutions; consider SIMD or incremental
  raster updates without weakening deterministic serial/parallel behavior.
- [ ] Add perceptual color objectives and antialiased scoring as explicit options,
  not silent changes to the current RGB-MSE objective.

## Documentation and release work

- [ ] Broaden API reference prose for legacy operators and diagnostics.
- [ ] Publish a reviewed 0.3 release/tag, reproducible source archive, and package
  manager recipes after downstream compatibility feedback.
- [ ] Enable GitHub Actions Pages in repository settings and verify custom-domain
  DNS/TLS separately; local source changes cannot repair account/domain settings.
