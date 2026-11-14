# Evolution lab

Build with `cmake --preset release && cmake --build --preset release`.
Executables live in `output/release/src/demo/`. Run `--help` for validated options.
Create the output parent directory yourself. Every run writes:

- `.html`: standalone scrubber/replay, keyframe artwork, and fitness chart.
- `.svg`: final vector image, readable without the C++ executable.
- `.csv`: `generation,fitness`; higher fitness is better, displayed costs are negated.
- `.ppm`: exact final fitness raster (portrait demos only).

No output embeds a network dependency. The HTML can be opened as a local file.
The seed, population, and actual generation count are shown. There are at most
roughly 82 keyframes; CSV includes every observed generation. Early stopping
can finish before the configured maximum.

## Shared command-line interface

| Flag | Range / behavior |
| --- | --- |
| `--seed N` | Unsigned 32-bit seed, default 42 |
| `--generations N` | 1–100000, default varies by demo |
| `--population N` | 20–2000, default 128 (64 for portraits) |
| `--output PATH` | Output prefix, not a directory; parent must exist |
| `--help` | Usage and supported flags |

Bad options and invalid numbers fail with a nonzero exit and a standard error
message. Repeating a seed reproduces a run within the same build and standard
library, not necessarily with another compiler or standard library.

## Voronoi portrait (Chromatic cells)

```sh
mkdir -p output/art
./output/release/src/demo/voronoi_art --shapes 24 --max-shapes 256 --threads 4 \
  --generations 6000 --output output/art/voronoi
```

Inspired by [Sebastian Proost’s Minimalist Art Using a Genetic Algorithm](https://blog.4dcu.be/programming/2020/02/10/Genetic-Art-Algorithm-2.html).
Each five-gene block encodes a site `(x, y, red, green, blue)`. Pixels take the
nearest site’s color. Geometry mutation, residual-guided insertion, and occasional
deletion let the representation grow and simplify. `--shapes 4..256` sets the
initial count (default 24); `--max-shapes 4..256` sets the growth limit (default
128, raised to the initial count if necessary). New sites sample high-error
regions. For each candidate geometry, exact cell means minimize RGB squared
error. Equal-position sites use the first site’s color.
The SVG exporter clips a rectangular canvas against pairwise bisector half-planes.
There is no dependency on SciPy or an external Voronoi package.
Fitness avoids rebuilding those polygons: a row-wise lower envelope of squared
distance parabolas assigns pixels in roughly `O(sites × height + pixels)` after
sorting sites by x. Regression tests compare it against exhaustive nearest-site
search, including coincident sites and stable ties.

## EvoLisa: parallel triangle painting

```sh
./output/release/src/demo/triangle_art --shapes 96 --threads 4 \
  --generations 12000 --output output/art/evolisa
```

Inspired by [Károly Zsolnai-Fehér’s parallel genetic algorithm for Roger Alsing’s EvoLisa problem](https://users.cg.tuwien.ac.at/zsolnai/gfx/mona_lisa_parallel_genetic_algorithm/).
Each triangle stores six coordinate genes, RGB, and alpha. The genome has 50
triangles by default, configurable with `--shapes 4..256` (the gallery uses 96).
Layers composite over RGB `(24,24,24)`. A mixture of fine, medium, and coarse
mutations changes vertices, whole-triangle translations and alpha. Residual-guided
restarts target poorly reconstructed regions; occasional block crossover imports
one shape from another parent. Least-squares color fitting accounts for alpha
and occlusion by later layers. Integer blending can differ from its affine
approximation, so the normal fitness/replacement step still decides acceptance.
Geometry is normalized into `[0,255]` independently on each axis.

`--threads 1..32` controls a C++20 CPU worker pool. Candidate raster fitnesses
and deterministic color refinements are evaluated independently. Random decisions remain on the solver thread, so
changing worker counts does not change the fitness history for the same build.
`tests/check_examples.py` exercises that contract.

### Targets and fidelity

Both portrait demos accept `--input target.ppm` (P3/P6, maximum sample <=255).
Comments, grayscale channel scaling, and binary pixel whitespace are handled.
Dimensions are bounded at 4096 per axis. The reference retains a longest edge
of up to 600 pixels; the bundled reference is 396 × 599 rather than 40 × 60.
Fitness uses a separate, area-averaged raster with `--resolution 32..256`
(default longest edge 128). Neither path upsamples small inputs. Convert PNG/JPEG to PPM with your usual
image tool. P6 uses one-byte channels, not 16-bit PPM.

Initial geometry is random and colors are target-informed, then fitted.
These are hybrid/memetic searches: geometry evolves while colors receive local
analytical refinement. The mutation mixture always retains broad proposals,
rather than annealing every mutation down to a tiny radius and getting stuck.
Fitness is negative mean squared error in encoded RGB, not perceptual color
distance. Triangle fitness uses pixel-center coverage without antialiasing;
browser SVG rendering is antialiased. The exported PPM is the exact scored image.
These are independent educational CPU implementations, not reproductions of the
original programs or their benchmark performance. More generations help but do
not guarantee a recognizable portrait or a global optimum.

The bundled public-domain Mona Lisa target is described in
[asset provenance](../src/demo/showcase/assets/README.md). No image is fetched at runtime.

## Travelling salesperson

`star_tour` searches a 48-point three-arm spiral galaxy. A chromosome is a visit
permutation, fitness is negative closed tour length, permutation crossover
preserves all stops, and swap mutation untangles crossings. The final-to-first
edge is included. The SVG overlays the route on the point field.

## N-queens

`queens --size 20` searches row permutations: one queen per column and row by
construction, with diagonal pairs as the cost. Threatened queens are highlighted.
Stops at zero conflicts or the generation budget. `--size` supports 4–48; some
small search spaces cannot support large distinct populations, which now fail
with an informative replenishment exception rather than hanging.

## Palette image

`pixel_art` evolves a 32 × 24 palette-index image against a procedural ringed
planet. Its 768 genes choose from 16 colors. Four-point crossover and sparse
uniform mutation minimize total RGB squared error. The target is generated in
C++; there is no image asset or machine-learning dependency.

## Random maze

```sh
./output/release/src/demo/maze --seed 42 --size 18 --generations 1000 \
  --output output/art/maze
```

`--size 4..32` sets the square grid. A uint32-seeded xorshift generator carves a
connected maze with randomized depth-first search, then sparsely opens extra
walls to allow alternate routes. Changing the seed creates another maze; seed
zero is supported. Walls are symmetric and boundaries cannot wrap.

Each genome has `2 × size²` cardinal-direction genes. A blocked move stays put.
Fitness prefers the smallest **corridor distance** reached, then fewer moves
and collisions. A reverse breadth-first distance field supplies this informed
score and the optimal-length stopping bound, **not a route injected into the
population**. Crossover preserves a promising prefix and exchanges tails;
mutation explores short legal corridor segments, with occasional earlier-prefix
exploration. Loop-erasure repair removes repeated visits and blocked moves.
After reaching the exit, evolution continues shortening the route until it
meets the bound or exhausts its budget. There is no convergence guarantee.

`docs/site/maze.html` adds an actual browser-side JavaScript GA, population
animation, pause/step/restart, new-random-maze and seed controls, speed/exploration
sliders, and an optional distance-field overlay. Its maze generator matches C++
bit-for-bit, tested at uint32 seed boundaries. Selection/RNG histories differ
between the browser and GeneiAL; the page labels that distinction explicitly.
It needs no build step or network service. `node --test tests/maze.test.cjs`
checks topology, determinism, legal moves and convergence across 20 mazes.

Inspired by the [GeneticML maze demo](http://geneticml.com/demos/maze/maze.html).
The reference site was unreachable during implementation; this is an independent
interpretation, not a verified visual replica or a port of its original source.

## Gallery and legacy examples

`cmake --build --preset release --target website` builds all six replays and
copies the static site to `output/release/site`. The source is `docs/site/`.
`demo1`, `demo2`, `demo3`, `tsp_solver`, and `benchmark_suite` remain available for
API/history exploration; the newer showcases are the recommended starting point.
