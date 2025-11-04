# TP2 — Scheduling Simulation & Graph Tools (C++)

This repository contains TP2: a C++ educational project focused on scheduling simulations, graph data structures, and analysis utilities. It bundles simulation cores, extended variants, configuration generators, and helper scripts to produce and analyze experimental data. The project is structured to support experiments, metrics collection, and graph generation for algorithmic studies.

## Table of contents

- [Project overview](#project-overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Build](#build)
- [Run / Usage](#run--usage)
- [Project structure](#project-structure)
- [Development notes](#development-notes)
- [Testing & Experiments](#testing--experiments)
- [Contributing](#contributing)
- [License](#license)
- [Authors & Contact](#authors--contact)

## Project overview

TP2 implements scheduling simulations and supporting data-structures to run experiments and collect metrics. It contains two main application entry points (regular and extended simulations), configuration generators, and utilities for logging and graph generation. The code is intended for coursework and research experiments in algorithms, scheduling, and simulation.

## Features

- Simulation core and an extended simulation variant (`Scheduler`, `SchedulerExtended`).
- Configuration data and generation utilities (`ConfigData`, `ConfigGenerator`).
- Graph and extended graph data structures and algorithms (`Graph`, `GraphExtended`).
- Common data-structures: linked lists, min-heap, queue, stack.
- Experiment runners and metrics collection (`SimulationMetrics`, `ExperimentRunner`).
- Helper Python scripts for graph generation: `generate_graphs.py`, `generate_extended_graphs.py`.
- Example input files and documentation in `data/` and `docs/`.

## Prerequisites

- Linux or other POSIX-like OS.
- GNU Make and a C++ compiler (g++). Project targets modern C++ (C++11 or later).
- Python 3 (for helper scripts that generate graphs).

On Debian/Ubuntu you can install the basics with:

```bash
sudo apt update
sudo apt install build-essential make python3
```

## Build

Build the project using the included `Makefile` from the `TP2` directory:

```bash
cd /path/to/TP2
make
```

The Makefile will compile sources in `src/` and typically places object files under `obj/`. If the Makefile produces executables, they will usually be in a `bin/` directory or in the project root — check the Makefile for exact target names and output locations.

## Run / Usage

There are two main application entry points implemented in `src/app`:

- `main.cpp` — the base simulation driver.
- `mainExtended.cpp` — the extended simulation driver.

After building, run the binary produced by your build. Example invocations (adjust the executable name or path to match your Makefile):

```bash
# Run base simulation (example)
./bin/main < data/test.txt

# Run extended simulation (example)
./bin/mainExtended < data/test2.txt
```

If the build places executables in the project root or a different folder, update the path accordingly. The program reads input files from `data/` — inspect the `src/app/main.cpp` and `src/app/mainExtended.cpp` to understand the expected input format and command-line options.

### Graph generation scripts

Two Python helpers produce graphs used by experiments:

```bash
python3 generate_graphs.py  # small graphs
python3 generate_extended_graphs.py  # larger / extended graphs
```

Check the scripts' headers or `--help` (if implemented) for options to control output size and location.

## Project structure

Top-level layout and important folders:

- `Makefile` — build rules.
- `data/` — sample input logs and test data (e.g. `test.txt`, `sched1.log`).
- `docs/` — reports and documentation (e.g. `gabriel_santos_faria_2024013990.pdf`).
- `include/` — public headers organized by domain.
- `src/` — implementation files mirroring the headers.

Notable sub-directories inside `src/` and `include/`:

- `analysis/` — comparison and experiment runners, configuration generation.
- `app/` — `main.cpp` and `mainExtended.cpp` application entry points.
- `core/` — simulation core types (`Event`, `Scheduler`, `ConfigData`).
- `dataStructures/` — `Graph`, `LinkedList`, `MinHeap`, `Queue`, `Stack`.
- `domains/` — domain-specific types like `Package`, `Warehouse` (and extended versions).
- `utils/` — logging and utility entry points.

## Development notes

- Keep headers in `include/` and implementations in `src/` to preserve separation of interface and implementation.
- Follow the `.clang-format` if present to keep consistent style.
- If you add new source files, update the `Makefile` so they are compiled and linked into the final targets.

Edge cases and suggestions:

- Input validation: the simulation code may assume well-formed input for experiments. Add validation for production or external data.
- Performance: when running large-scale experiments, watch memory usage and runtime. Use `SimulationMetrics` to collect timing and load stats.

## Testing & Experiments

- Manual tests: use files placed in `data/` to run example scenarios.
- Automated tests: there are no unit tests included by default. Consider adding a test framework (Catch2 or GoogleTest) and adding `make test` to the `Makefile`.
- Reproducible experiments: store configuration files and seeds for random generators so results are repeatable.

## Contributing

Contributions are welcome. Good contributions include:

- Adding unit tests and CI integration.
- Improving documentation and example configurations.
- Adding new experiment runners or analysis visualization tools.

Contribution workflow:

1. Fork the repository.
2. Create a feature branch and add tests for new behavior.
3. Open a pull request with clear description and rationale.

## License

This repository snapshot does not include a LICENSE file. If you plan to publish or share the code, add an appropriate open-source license (for example MIT or Apache-2.0) and include it in the repository root.

## Authors & Contact

Primary author (from repository documents):

- Gabriel Santos Faria — see reports in `docs/` (e.g. `gabriel_santos_faria_2024013990.pdf`).

If you need to contact the maintainer, reference the repo and the document in `docs/` that contains the student ID.

## Acknowledgements

Built as an educational assignment for coursework and experiments in scheduling and graph algorithms. The repository contains components useful for teaching, experiments, and reproducible benchmarking.

---

Files added/edited by this change:

- `README.md` — new file added to the `TP2` folder documenting the project and usage.
