# Coding guidelines

- Target C++20 with portable standard-library facilities. Keep platform flags on
  CMake targets, never in global compiler strings.
- Prefer RAII, value exceptions, `std::unique_ptr`, `std::shared_ptr` only for
  actual shared ownership, `std::optional`, `std::span`, and scoped locks.
- Validate public inputs in release builds; assertions document internal invariants,
  not user-facing error handling. Never throw exception pointers.
- Keep templates in headers/adjacent `.hpp` files and headers self-contained.
  Avoid reliance on unrelated include order or transitive declarations.
- Preserve parent genomes in genetic operators; allocate children explicitly.
  Do not modify an object while its hash/fitness is indexed in a population.
- Keep public names compatible unless the migration cost is documented. Do not
  rewrite untouched legacy formatting as part of a bug fix.
- Use the repository `.clang-format` (CI uses clang-format 18 for showcase files).
  New code uses descriptive identifiers and braces around nontrivial control flow.
- Public API documentation should state ownership, exceptions, thread safety,
  determinism, and boundary behavior. Explain why non-obvious logic exists.
- Regression tests reproduce the old failure and check the corrected contract.
  Use fixed seeds and mathematical invariants, not assumptions that random
  crossover must always produce a different child or that a heuristic always wins.
- Keep generated files under `output/`, test dependencies optional, and examples
  offline. Document and credit any bundled external assets.

See [CONTRIBUTING.md](CONTRIBUTING.md) for the build/test workflow.
