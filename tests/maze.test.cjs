const assert = require("node:assert/strict");
const test = require("node:test");
const { MazeModel, MazeEvolution } = require("../docs/site/maze.js");

test("maze generator matches native topology fixtures, including uint32 boundaries", () => {
  for (const [seed, expected] of [
    [0, 2753865642],
    [1, 555423777],
    [42, 1714881153],
    [4294967295, 2238746772],
  ]) {
    const maze = new MazeModel(18, seed);
    let hash = 2166136261;
    for (const wall of maze.walls)
      hash = Math.imul(hash ^ wall, 16777619) >>> 0;
    assert.equal(hash, expected);
    assert.deepEqual(maze.walls, new MazeModel(18, seed).walls);
  }
  assert.notDeepEqual(new MazeModel(18, 1).walls, new MazeModel(18, 42).walls);
});

test("every cell is reachable; walls are symmetric and prevent wrapping", () => {
  for (const size of [4, 12, 18, 32])
    for (const seed of [0, 1, 42, 4294967295]) {
      const maze = new MazeModel(size, seed);
      assert.equal(maze.maximumDistance, Math.max(...maze.distances));
      for (let cell = 0; cell < maze.walls.length; cell++) {
        assert.ok(maze.distances[cell] >= 0);
        for (let direction = 0; direction < 4; direction++) {
          const next = maze.advance(cell, direction);
          assert.ok(next >= 0 && next < size * size);
          if (next !== cell) {
            assert.equal(maze.advance(next, (direction + 2) % 4), cell);
            assert.ok(
              Math.abs(maze.distances[cell] - maze.distances[next]) <= 1,
            );
          }
        }
      }
      assert.equal(maze.advance(0, 0), 0);
      assert.equal(maze.advance(0, 3), 0);
      assert.equal(maze.advance(size * size - 1, 1), size * size - 1);
      assert.equal(maze.advance(size * size - 1, 2), size * size - 1);
    }
  for (const [size, seed] of [
    [0, 42],
    [33, 42],
    [12.5, 42],
    [18, -1],
    [18, 4294967296],
    [18, NaN],
  ])
    assert.throws(() => new MazeModel(size, seed), RangeError);
  assert.throws(() => new MazeModel(4, 42).advance(0, 4), RangeError);
});

test("seeded evolution is monotonic, deterministic and finds valid routes across 20 mazes", () => {
  for (let seed = 0; seed < 20; seed++) {
    const maze = new MazeModel(18, seed);
    const first = new MazeEvolution(maze, seed),
      second = new MazeEvolution(maze, seed);
    let previous = first.best.trace.cost;
    for (let generation = 0; generation < 500 && first.step(); generation++) {
      assert.equal(second.step(), true);
      assert.equal(first.best.trace.cost, second.best.trace.cost);
      assert.deepEqual(first.best.genome, second.best.genome);
      assert.ok(first.best.trace.cost <= previous);
      previous = first.best.trace.cost;
    }
    assert.equal(first.best.trace.remaining, 0, `seed ${seed}`);
    assert.equal(first.best.trace.positions.at(-1), maze.walls.length - 1);
    for (const direction of first.best.genome)
      assert.ok(direction >= 0 && direction < 4);
    const compact = maze.compactPrefix(first.best.genome, first.best.trace);
    const repaired = maze.trace(compact);
    assert.equal(repaired.remaining, 0);
    assert.ok(repaired.bestStep <= first.best.trace.bestStep);
    assert.ok(repaired.bestStep >= maze.distances[0]);
  }
});
