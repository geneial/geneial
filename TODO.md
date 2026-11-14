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

## Next correctness and compatibility work

- [ ] Replace hash-only population uniqueness with collision buckets; test two
  different genomes whose custom hash intentionally collides.
- [ ] Replace escaping mutable genome references with an edit guard or explicit
  immutable genome API; make cache invalidation impossible to bypass.
- [ ] Audit all legacy stateful stopping decorators for duplicate observations
  per generation and boundary behavior; expand tests beyond the repaired core.
- [ ] Validate every legacy operator/settings combination, including smoothing,
  replacement policies other than ReplaceWorst, and degenerate search spaces.
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
