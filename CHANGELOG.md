# Changelog

## 0.3.0 — unreleased

### Numeric-summary maintenance

- Exact integral means without overflowing or floating-point rounding at 64-bit
  limits; preserve truncation toward zero for mixed-sign genomes.
- Checked integral sums and ranges throw `std::overflow_error` for unrepresentable
  final results. Signed sums allow cancellation independent of gene order.
- One-pass min/max range calculation, constant-space arithmetic, and regression
  coverage across integer widths, boolean genes, cancellation orders, empty
  inputs, and seeded inputs compared with wider arithmetic.
- Document arithmetic/migration contracts and replace stale README test counts
  with the command that reports the current registered suite.
- Correct preset-relative JUnit report paths in the build, coverage and TSan CI
  jobs, and assert that reports exist where artifact uploads expect them.

### Example and website refinements

- Seeded random-maze C++ example and a live, dependency-free browser GA playground
  with animated populations, random/seed controls, stepping and a fitness overlay.
- Sharp 396 × 599 reference portraits, area-averaged configurable scoring rasters,
  residual-guided growth/restarts, persistent multiscale geometry mutations, and
  parallel analytical color refinement. Gallery triangles increase from 50 to 96;
  the CLI default remains 50. These are explicitly labeled hybrid genetic searches.
- Replay asset deduplication prevents embedding a large reference per keyframe;
  ceiling-based frame spacing also honors the documented keyframe bound.
- Scalar-reference raster regressions, image/color fitting tests, cross-language
  maze fixtures and seeded convergence checks; remove redundant hero badges.
- A restrained technical example index, smaller typography and plain-language
  documentation replace the promotional landing page and replay styling. Run
  configurations, CSV/source links and automatic local-link checks are included.
- Cache the maze distance-field maximum instead of rescanning it for every
  cell on every visualization frame.

### Modern development

- C++20, CMake 3.21, root/legacy source entry points, presets, target-based flags,
  and relocatable static/shared CMake packages.
- Standard-library optional/hash/RNG/threading; Boost is only required for tests.
- GCC/Clang/macOS CI, sanitizer coverage, downstream linking, artifact validation,
  source-based Docker builds, and a GitHub Pages gallery workflow.

### Correctness

- Owned factory settings, continuous generation/blending, permutation settings
  and crossover boundaries, and coordinate interpolation.
- Seeded thread-local RNG, negative/zero fitness selection, N-point segment
  feasibility, bounded mutation and population requests, and value exceptions.
- Fitness/hash invalidation, value equality, numeric summaries, stopping-criterion
  XOR/tolerances, replacement counts, and empty population checks.
- C++20 worker-pool task draining, task exception propagation, batching, and
  validated worker/shutdown configurations.
- Self-contained fitness/crossover includes and a usable crossover lambda adapter.
- Collision-aware population indexes, usable hash lookup, exact-pointer removal,
  finite-score validation, transactional replacement, and noncopyable populations.
- Random replacement budget/elitism fixes and duplicate-aware candidate prefixes;
  null manager dependencies are rejected without replacing valid ones.
- Once-per-generation stopping windows with manager/age resets, validated
  fix-point warm-up, full-genome population comparison, and absolute trailing stop.
- Smoothing anchors, numeric limits and edge tapering; bounded signed/floating
  peak mutation and cache-preserving reads in all three mutation policies.
- Three additional unit suites and 33 new active regression cases; removed the
  stale, unregistered mutation test source.

### Memory and performance audit

- Reject shared-ownership cycles in stopping graphs; add graph-wide reset with
  once-only shared-child traversal and configurable initial combined criteria.
- Bound idle chromosome caches, expose pruning, release cached evaluator/fitness
  state, reset reused ages, and delete safely if cache growth allocation fails.
- Bound diagnostic retention, count drops, eliminate ignored-event allocations,
  make scoped timers safe after bookkeeper destruction, and validate timing data.
- Replace empty diagnostic analyzers with retained event-frequency reports and
  an explicitly labeled current-population snapshot; validate facade lifecycles.
- Initialize the best-chromosome observer's baseline before comparing fitness.
- Compact duplicate batches once instead of repeated vector erasure; reuse worker
  batch buffers and release completed captures before reporting a drained queue.
- Saturate continuous factory steps at integral limits; validate anchors, empty
  permutations, representable universes, shuffle limits, and smoothed factories.
- Add 20 regression cases, a deterministic allocation/failure probe, and a JSON
  benchmark; add GCC coverage, separate Linux TSan, explicit Linux leak detection,
  warnings-as-errors, test reports, and artifact retention limits to CI.

### Examples and docs

- New callable/span fitness evaluator.
- Six offline HTML/SVG/CSV evolution showcases: galaxy tour, N-queens, pixel art,
  variable-length Voronoi art, parallel translucent-triangle EvoLisa, and random mazes.
- Custom PPM targets, bundled credited public-domain Mona Lisa, vector export,
  exact fitness rasters, and replay/fitness history controls.
- Rebuilt responsive website; refreshed architecture, migration, contribution,
  example documentation, and an honest remaining roadmap.

The minimum language version and random sequences change. See
[migration notes](docs/Migration.md) before upgrading existing consumers.
