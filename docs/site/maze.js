"use strict";

class MazeRandom {
  constructor(seed) {
    this.state = seed >>> 0 || 0x9e3779b9;
  }
  next() {
    let state = this.state;
    state ^= state << 13;
    state ^= state >>> 17;
    state ^= state << 5;
    this.state = state >>> 0;
    return this.state;
  }
  pick(count) {
    return this.next() % count;
  }
}

class MazeModel {
  constructor(size, seed) {
    if (
      !Number.isInteger(size) ||
      size < 4 ||
      size > 32 ||
      !Number.isInteger(seed) ||
      seed < 0 ||
      seed > 4294967295
    )
      throw new RangeError("Invalid maze settings");
    this.size = size;
    this.walls = new Uint8Array(size * size).fill(15);
    const random = new MazeRandom(seed);
    const visited = new Uint8Array(this.walls.length);
    const stack = [0];
    visited[0] = 1;
    while (stack.length) {
      const cell = stack[stack.length - 1];
      const choices = [];
      for (let direction = 0; direction < 4; direction++) {
        const next = this.neighbor(cell, direction);
        if (next !== cell && !visited[next]) choices.push(direction);
      }
      if (!choices.length) stack.pop();
      else {
        const direction = choices[random.pick(choices.length)];
        const next = this.neighbor(cell, direction);
        this.open(cell, direction);
        visited[next] = 1;
        stack.push(next);
      }
    }
    for (let cell = 0; cell < this.walls.length; cell++)
      for (const direction of [1, 2])
        if (this.neighbor(cell, direction) !== cell && random.pick(64) === 0)
          this.open(cell, direction);
    this.distances = new Int32Array(this.walls.length).fill(-1);
    const pending = [this.walls.length - 1];
    this.distances[pending[0]] = 0;
    for (let head = 0; head < pending.length; head++) {
      const cell = pending[head];
      for (let direction = 0; direction < 4; direction++) {
        const next = this.advance(cell, direction);
        if (this.distances[next] < 0) {
          this.distances[next] = this.distances[cell] + 1;
          pending.push(next);
        }
      }
    }
    this.maximumDistance = Math.max(...this.distances);
  }
  neighbor(cell, direction) {
    const row = Math.floor(cell / this.size),
      column = cell % this.size;
    if (direction === 0 && row > 0) return cell - this.size;
    if (direction === 1 && column + 1 < this.size) return cell + 1;
    if (direction === 2 && row + 1 < this.size) return cell + this.size;
    if (direction === 3 && column > 0) return cell - 1;
    return cell;
  }
  open(cell, direction) {
    const next = this.neighbor(cell, direction);
    this.walls[cell] &= ~(1 << direction);
    this.walls[next] &= ~(1 << (direction + 2) % 4);
  }
  advance(cell, direction) {
    if (
      !Number.isInteger(cell) ||
      cell < 0 ||
      cell >= this.walls.length ||
      !Number.isInteger(direction) ||
      direction < 0 ||
      direction > 3
    )
      throw new RangeError("Invalid maze move");
    return this.walls[cell] & (1 << direction)
      ? cell
      : this.neighbor(cell, direction);
  }
  trace(genome) {
    const positions = [0];
    let remaining = this.distances[0],
      bestStep = 0,
      hits = 0,
      collisions = 0;
    for (let step = 0; step < genome.length && remaining !== 0; step++) {
      const previous = positions[positions.length - 1];
      const cell = this.advance(previous, genome[step]);
      if (cell === previous) hits++;
      positions.push(cell);
      if (this.distances[cell] < remaining) {
        remaining = this.distances[cell];
        bestStep = step + 1;
        collisions = hits;
      }
    }
    return {
      positions,
      remaining,
      bestStep,
      cost:
        remaining * (genome.length + 1) +
        bestStep +
        collisions / (genome.length + 1),
    };
  }
  compactPrefix(genome, trace) {
    const route = [],
      positions = [0],
      seen = new Int32Array(this.walls.length).fill(-1);
    seen[0] = 0;
    for (let step = 1; step <= trace.bestStep; step++) {
      const cell = trace.positions[step];
      if (cell === positions[positions.length - 1]) continue;
      if (seen[cell] >= 0) {
        const end = seen[cell];
        for (let index = end + 1; index < positions.length; index++)
          seen[positions[index]] = -1;
        positions.length = end + 1;
        route.length = end;
      } else {
        route.push(genome[step - 1]);
        seen[cell] = positions.length;
        positions.push(cell);
      }
    }
    return route;
  }
}

class MazeEvolution {
  constructor(maze, seed, population = 100) {
    this.maze = maze;
    this.random = new MazeRandom((seed ^ 0xa511e9b3) >>> 0);
    this.generation = 0;
    this.population = [];
    for (let index = 0; index < population; index++) {
      const genome = new Uint8Array(maze.size * maze.size * 2);
      for (let step = 0; step < genome.length; step++)
        genome[step] = this.random.pick(4);
      this.population.push(this.specimen(genome));
    }
    this.sort();
  }
  specimen(genome) {
    return { genome, trace: this.maze.trace(genome) };
  }
  sort() {
    this.population.sort(
      (first, second) => first.trace.cost - second.trace.cost,
    );
    this.best = this.population[0];
  }
  parent() {
    let best = this.population[this.random.pick(this.population.length)];
    for (let round = 0; round < 3; round++) {
      const candidate =
        this.population[this.random.pick(this.population.length)];
      if (candidate.trace.cost < best.trace.cost) best = candidate;
    }
    return best;
  }
  step(exploration = 0.15) {
    if (
      this.best.trace.cost <= this.maze.distances[0] ||
      this.generation >= 3000
    )
      return false;
    const next = this.population.slice(0, 8);
    while (next.length < this.population.length) {
      const parent = this.parent(),
        donor = this.parent();
      const genome = parent.genome.slice();
      if (this.random.pick(100) < 25)
        genome.set(
          donor.genome.subarray(parent.trace.bestStep),
          parent.trace.bestStep,
        );
      let trace = this.maze.trace(genome);
      if (this.random.pick(100) < 35) {
        const compact = this.maze.compactPrefix(genome, trace),
          original = genome.slice();
        genome.set(compact);
        genome.set(original.subarray(trace.bestStep), compact.length);
        trace = this.maze.trace(genome);
      }
      if (trace.cost <= this.maze.distances[0]) {
        next.push(this.specimen(genome));
        continue;
      }
      const revisit =
        trace.remaining === 0 || this.random.pick(10000) < exploration * 10000;
      const lastMutable =
        trace.remaining === 0 ? trace.bestStep - 1 : trace.bestStep;
      const cut = revisit ? this.random.pick(lastMutable + 1) : trace.bestStep;
      let cell = trace.positions[cut],
        previous = cut ? trace.positions[cut - 1] : cell;
      for (let step = cut; step < Math.min(genome.length, cut + 6); step++) {
        let choices = [];
        for (let direction = 0; direction < 4; direction++) {
          const candidate = this.maze.advance(cell, direction);
          if (candidate !== cell && candidate !== previous)
            choices.push(direction);
        }
        if (!choices.length)
          for (let direction = 0; direction < 4; direction++)
            if (this.maze.advance(cell, direction) !== cell)
              choices.push(direction);
        const direction = choices[this.random.pick(choices.length)];
        genome[step] = direction;
        previous = cell;
        cell = this.maze.advance(cell, direction);
      }
      next.push(this.specimen(genome));
    }
    this.population = next;
    this.sort();
    this.generation++;
    return true;
  }
}

if (typeof module !== "undefined" && module.exports)
  module.exports = { MazeRandom, MazeModel, MazeEvolution };

if (typeof document !== "undefined") {
  const control = (name) => document.getElementById(name);
  const canvas = control("maze"),
    context = canvas.getContext("2d");
  const parameters = new URLSearchParams(location.search);
  const seedParameter = parameters.get("seed"),
    sizeParameter = parameters.get("size");
  if (
    seedParameter !== null &&
    /^\d+$/.test(seedParameter) &&
    Number(seedParameter) <= 4294967295
  )
    control("seed").value = seedParameter;
  if (["12", "18", "24", "32"].includes(sizeParameter))
    control("size").value = sizeParameter;
  let search,
    running = false,
    lastGeneration = 0,
    lastDraw = 0,
    animationStart = 0;
  const reducedMotion = matchMedia("(prefers-reduced-motion: reduce)").matches;

  function update() {
    const trace = search.best.trace,
      solved = trace.remaining === 0,
      optimal = trace.cost <= search.maze.distances[0],
      exhausted = search.generation >= 3000;
    control("generation").textContent = search.generation.toLocaleString();
    control("remaining").textContent = trace.remaining;
    control("length").textContent = trace.bestStep;
    control("gain").textContent =
      `${Math.round(100 * (1 - trace.remaining / search.maze.distances[0]))}%`;
    control("state").textContent = optimal
      ? "Shortest route evolved"
      : exhausted
        ? "Generation limit reached"
        : solved
          ? "Exit found · shortening route"
          : running
            ? "Population exploring"
            : "Paused / ready";
    control("route-summary").textContent = solved
      ? `Exit reached in ${trace.bestStep} moves · shortest possible: ${search.maze.distances[0]}`
      : `${search.population.length} explorers · ${search.maze.size} × ${search.maze.size} maze`;
    control("play").textContent =
      optimal || exhausted
        ? "Run again ↻"
        : running
          ? "Pause evolution ❚❚"
          : "Start evolution ▶";
    control("step").disabled = optimal || exhausted;
  }
  function pause() {
    running = false;
    update();
  }
  function reset() {
    const seed = Number(control("seed").value),
      size = Number(control("size").value);
    if (!control("settings").reportValidity()) return;
    search = new MazeEvolution(new MazeModel(size, seed), seed);
    running = false;
    animationStart = performance.now();
    lastGeneration = animationStart;
    history.replaceState(null, "", `?seed=${seed}&size=${size}`);
    control("announcement").textContent =
      `New ${size} by ${size} maze, seed ${seed}. Ready to evolve.`;
    update();
    draw(animationStart);
  }
  function advance(now) {
    search.step(Number(control("exploration").value) / 100);
    animationStart = now;
    if (search.best.trace.cost <= search.maze.distances[0]) {
      running = false;
      control("announcement").textContent =
        `Shortest route evolved in generation ${search.generation}, using ${search.best.trace.bestStep} moves. No route was supplied to the population.`;
    } else if (search.generation >= 3000) {
      running = false;
      control("announcement").textContent =
        "Generation budget exhausted. Try more prefix exploration or a new maze.";
    }
    update();
  }
  function draw(now) {
    const model = search.maze,
      tile = 660 / model.size,
      origin = 30;
    context.fillStyle = "#0b1120";
    context.fillRect(0, 0, 720, 720);
    if (control("field").checked)
      for (let cell = 0; cell < model.walls.length; cell++) {
        const proximity = 1 - model.distances[cell] / model.maximumDistance;
        context.fillStyle = `rgba(138,233,204,${0.02 + 0.18 * proximity})`;
        context.fillRect(
          origin + (cell % model.size) * tile,
          origin + Math.floor(cell / model.size) * tile,
          tile,
          tile,
        );
      }
    const center = (cell) => [
      origin + ((cell % model.size) + 0.5) * tile,
      origin + (Math.floor(cell / model.size) + 0.5) * tile,
    ];
    context.lineJoin = "round";
    context.lineCap = "round";
    for (const specimen of search.population.slice(0, 24).reverse()) {
      const trace = specimen.trace;
      context.beginPath();
      for (let step = 0; step <= trace.bestStep; step++) {
        const point = center(trace.positions[step]);
        if (step === 0) context.moveTo(...point);
        else context.lineTo(...point);
      }
      context.strokeStyle = "rgba(174,143,244,.075)";
      context.lineWidth = tile * 0.35;
      context.stroke();
    }
    const best = search.best.trace;
    context.beginPath();
    for (let step = 0; step <= best.bestStep; step++) {
      const point = center(best.positions[step]);
      if (step === 0) context.moveTo(...point);
      else context.lineTo(...point);
    }
    context.strokeStyle = "#8ae9cc";
    context.lineWidth = Math.max(2, tile * 0.17);
    context.stroke();
    context.beginPath();
    for (let cell = 0; cell < model.walls.length; cell++) {
      const left = origin + (cell % model.size) * tile,
        top = origin + Math.floor(cell / model.size) * tile;
      if (model.walls[cell] & 1) {
        context.moveTo(left, top);
        context.lineTo(left + tile, top);
      }
      if (model.walls[cell] & 8) {
        context.moveTo(left, top);
        context.lineTo(left, top + tile);
      }
      if (Math.floor(cell / model.size) + 1 === model.size) {
        context.moveTo(left, top + tile);
        context.lineTo(left + tile, top + tile);
      }
      if ((cell % model.size) + 1 === model.size) {
        context.moveTo(left + tile, top);
        context.lineTo(left + tile, top + tile);
      }
    }
    context.strokeStyle = "#52627d";
    context.lineWidth = 1.5;
    context.stroke();
    const circle = (point, radius, color) => {
      context.beginPath();
      context.arc(...point, radius, 0, Math.PI * 2);
      context.fillStyle = color;
      context.fill();
    };
    const progress =
      reducedMotion || !running
        ? 1
        : Math.min(
            1,
            (now - animationStart) / (1000 / Number(control("speed").value)),
          );
    for (const specimen of search.population.slice(0, 32)) {
      const trace = specimen.trace,
        point = center(trace.positions[Math.floor(progress * trace.bestStep)]);
      circle(point, Math.max(2, tile * 0.12), "#b39bef");
    }
    circle(center(0), tile * 0.28, "#8ae9cc");
    circle(center(model.walls.length - 1), tile * 0.28, "#f2bb68");
    circle(center(best.positions[best.bestStep]), tile * 0.2, "#f4fffb");
  }
  function tick(now) {
    if (!document.hidden) {
      if (
        running &&
        now - lastGeneration >= 1000 / Number(control("speed").value)
      ) {
        advance(now);
        lastGeneration = now;
      }
      if (now - lastDraw > 33 && (running || lastDraw < animationStart)) {
        draw(now);
        lastDraw = now;
      }
    }
    requestAnimationFrame(tick);
  }
  control("play").addEventListener("click", () => {
    if (
      search.best.trace.cost <= search.maze.distances[0] ||
      search.generation >= 3000
    )
      reset();
    running = !running;
    lastGeneration = performance.now();
    update();
    draw(lastGeneration);
  });
  control("step").addEventListener("click", () => {
    pause();
    advance(performance.now());
    draw(performance.now());
  });
  control("restart").addEventListener("click", reset);
  control("settings").addEventListener("submit", (event) => {
    event.preventDefault();
    reset();
  });
  control("randomize").addEventListener("click", () => {
    control("seed").value = crypto.getRandomValues(new Uint32Array(1))[0];
    reset();
  });
  control("speed").addEventListener("input", () => {
    control("speed-value").textContent =
      `${control("speed").value} generations/s`;
  });
  control("exploration").addEventListener("input", () => {
    control("exploration-value").textContent =
      `${control("exploration").value}%`;
  });
  control("field").addEventListener("change", () => draw(performance.now()));
  document.addEventListener("visibilitychange", () => {
    if (document.hidden) pause();
  });
  reset();
  requestAnimationFrame(tick);
}
