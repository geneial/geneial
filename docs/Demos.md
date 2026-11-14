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

## Chromatic cells: Voronoi genetic art

```sh
mkdir -p output/art
./output/release/src/demo/voronoi_art --shapes 24 --threads 4 \
  --generations 6000 --output output/art/voronoi
```

Inspired by [Sebastian Proost’s Minimalist Art Using a Genetic Algorithm](https://blog.4dcu.be/programming/2020/02/10/Genetic-Art-Algorithm-2.html).
Each five-gene block encodes a site `(x, y, red, green, blue)`. Pixels take the
nearest site’s color. Geometry/color mutation, site duplication, and site deletion
let the representation grow and simplify. Starts with `--shapes` sites (4–128),
and evolves between 4 and 128. Equal-position sites use the first site’s color.
Copying a site is initially neutral; perturbing it lets a new cell emerge.
The SVG exporter clips a rectangular canvas against pairwise bisector half-planes.
There is no dependency on SciPy or an external Voronoi package.

## EvoLisa: parallel triangle painting

```sh
./output/release/src/demo/triangle_art --shapes 50 --threads 4 \
  --generations 12000 --output output/art/evolisa
```

Inspired by [Károly Zsolnai-Fehér’s parallel genetic algorithm for Roger Alsing’s EvoLisa problem](https://users.cg.tuwien.ac.at/zsolnai/gfx/mona_lisa_parallel_genetic_algorithm/).
Each triangle stores six coordinate genes, RGB, and alpha. The genome has 50
triangles by default, configurable with `--shapes 4..128`. Layers composite over
RGB `(24,24,24)`. Local mutations alter coordinates/color/alpha; rare layer swaps
alter occlusion, and occasional block crossover imports one shape from another
parent. Geometry is normalized into `[0,255]` independently on each axis.

`--threads 1..32` controls a C++20 CPU worker pool. Candidate raster fitnesses
are evaluated independently. Random decisions remain on the solver thread, so
changing worker counts does not change the fitness history for the same build.
`tests/check_examples.py` exercises that contract.

### Targets and fidelity

Both portrait demos accept `--input target.ppm` (P3/P6, maximum sample <=255).
Comments, grayscale channel scaling, and binary pixel whitespace are handled.
Dimensions are bounded at 4096 per axis; fitness downsampling limits the target
to 40 × 60 while preserving aspect ratio. Convert PNG/JPEG to PPM with your usual
image tool. P6 uses one-byte channels, not 16-bit PPM.

Initial geometry is random; initial colors sample the target at site positions
or triangle centroids. Mutation radius gradually shrinks with the run budget.
Fitness is negative mean squared error in encoded RGB, not perceptual color
distance. Triangle fitness uses pixel-center coverage without antialiasing;
browser SVG rendering is antialiased. The exported PPM is the exact scored image.
These are independent educational CPU implementations, not reproductions of the
original programs or their benchmark performance. More generations help but do
not guarantee a recognizable portrait or a global optimum.

The bundled public-domain Mona Lisa target is described in
[asset provenance](../src/demo/showcase/assets/README.md). No image is fetched at runtime.

## The interstellar shortcut

`star_tour` searches a 48-point three-arm spiral galaxy. A chromosome is a visit
permutation, fitness is negative closed tour length, permutation crossover
preserves all stops, and swap mutation untangles crossings. The final-to-first
edge is included. The SVG overlays the route on the point field.

## A peaceful kingdom

`queens --size 20` searches row permutations: one queen per column and row by
construction, with diagonal pairs as the cost. Threatened queens are highlighted.
Stops at zero conflicts or the generation budget. `--size` supports 4–48; some
small search spaces cannot support large distinct populations, which now fail
with an informative replenishment exception rather than hanging.

## Noise becomes a world

`pixel_art` evolves a 32 × 24 palette-index image against a procedural ringed
planet. Its 768 genes choose from 16 colors. Four-point crossover and sparse
uniform mutation minimize total RGB squared error. The target is generated in
C++; there is no image asset or machine-learning dependency.

## Gallery and legacy examples

`cmake --build --preset release --target website` builds all five replays and
copies the static site to `output/release/site`. The source is `docs/site/`.
`demo1`, `demo2`, `demo3`, `tsp_solver`, and `benchmark_suite` remain available for
API/history exploration; the newer showcases are the recommended starting point.
