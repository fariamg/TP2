# Logistics Scheduling Simulation Framework

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Python](https://img.shields.io/badge/Python-3.8%2B-blue.svg)](https://www.python.org/)
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com)
[![License](https://img.shields.io/badge/license-Educational-orange.svg)](LICENSE)

> An advanced event-driven simulation framework for logistics scheduling, package routing, and warehouse optimization with comprehensive graph algorithms and performance analysis tools.

## 📋 Table of Contents

- [Overview](#-overview)
- [Key Features](#-key-features)
- [Quick Start](#-quick-start)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Basic Usage](#basic-usage)
- [Architecture](#-architecture)
- [Build System](#-build-system)
- [Usage Guide](#-usage-guide)
  - [Running Simulations](#running-simulations)
  - [Running Experiments](#running-experiments)
  - [Generating Visualizations](#generating-visualizations)
- [Project Structure](#-project-structure)
- [Data Structures](#-data-structures)
- [Algorithms](#-algorithms)
- [Configuration](#-configuration)
- [Performance Analysis](#-performance-analysis)
- [Development](#-development)
- [Experiments](#-experiments)
- [Testing](#-testing)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [License](#-license)
- [Authors](#-authors)

## 🎯 Overview

This project implements a **discrete event-driven simulation framework** for modeling complex logistics operations including package routing, warehouse management, and delivery scheduling. It features two implementation variants (base and extended) with comprehensive performance analysis and visualization capabilities.

### What This Framework Does

- **Package Routing**: Optimal path calculation using graph algorithms
- **Warehouse Scheduling**: Event-driven simulation of package processing
- **Performance Analysis**: Detailed metrics collection and statistical analysis
- **Scalability Testing**: Configurable experiments for different problem sizes
- **Visualization**: Automated graph generation from experimental results

### Use Cases

- Algorithm complexity analysis and comparison
- Logistics optimization research
- Educational demonstrations of scheduling algorithms
- Performance benchmarking of graph algorithms
- Discrete event simulation studies

## ✨ Key Features

| Category            | Features                                                          |
| ------------------- | ----------------------------------------------------------------- |
| **Simulations**     | Base & Extended scheduling engines with event-driven architecture |
| **Data Structures** | Custom Graph, MinHeap, Queue, Stack, LinkedList implementations   |
| **Algorithms**      | Dijkstra's shortest path, optimal routing, warehouse scheduling   |
| **Analysis Tools**  | Experiment runners, metrics collection, statistical analysis      |
| **Visualization**   | Python scripts for generating performance graphs                  |
| **Configuration**   | Automated config generation, flexible parameter tuning            |
| **Logging**         | Comprehensive logging system for debugging and analysis           |

## 🚀 Quick Start

### Prerequisites

**Required:**

- **OS**: Linux (Debian/Ubuntu recommended) or other POSIX-compatible system
- **C++ Compiler**: GCC/G++ with C++17 support (version 7.0+)
- **Build System**: GNU Make 4.0+
- **Python**: Python 3.8+ (for visualization scripts)

**Python Dependencies:**

```bash
pip install pandas matplotlib numpy
```

**System Installation (Debian/Ubuntu):**

```bash
sudo apt update
sudo apt install build-essential make g++ python3 python3-pip
```

### Installation

1. **Navigate to the project directory:**

```bash
cd /path/to/TP2
```

2. **Build all components:**

```bash
make all
```

This compiles:

- Base simulation engine
- Extended simulation variant
- Experiment runners
- Comparison tools
- Configuration generator

3. **Verify the build:**

```bash
ls -lh bin/
# Should show: simulation_base, simulation_extended, experiment_base, etc.
```

### Basic Usage

**Run base simulation:**

```bash
./bin/simulation_base data/test.txt
```

**Run extended simulation:**

```bash
./bin/simulation_extended data/test2.txt
```

**Generate and use configurations:**

```bash
make configs                          # Generate config files
./bin/simulation_base configs/test_config.txt
```

**Run experiments and generate visualizations:**

```bash
make run-experiments                  # Run all experiments
python3 generate_graphs.py           # Generate performance graphs
```

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────┐
│           Application Layer                     │
│   main.cpp (Base) | mainExtended.cpp (Ext.)    │
└──────────────┬──────────────────────────────────┘
               │
┌──────────────▼──────────────────────────────────┐
│              Core Components                    │
│  • Scheduler / SchedulerExtended               │
│  • ConfigData / ConfigDataExtended             │
│  • Event Management System                     │
└────┬─────────────────────────────┬──────────────┘
     │                             │
┌────▼─────────────┐    ┌──────────▼──────────────┐
│  Domain Model    │    │   Data Structures       │
│  • Package       │    │  • Graph                │
│  • Warehouse     │    │  • MinHeap              │
│  • Event         │    │  • Queue, Stack         │
│  (Base/Extended) │    │  • LinkedList           │
└──────────────────┘    └─────────┬───────────────┘
                                  │
                        ┌─────────▼───────────────┐
                        │    Algorithms           │
                        │  • Routing              │
                        │  • Shortest Path        │
                        │  • Graph Traversal      │
                        └─────────────────────────┘

┌─────────────────────────────────────────────────┐
│           Analysis & Visualization              │
│  • ExperimentRunner                             │
│  • SimulationMetrics                            │
│  • Python Visualization Scripts                 │
└─────────────────────────────────────────────────┘
```

## 🔨 Build System

The project uses a sophisticated Makefile with multiple targets:

### Build Targets

| Target                  | Description                    |
| ----------------------- | ------------------------------ |
| `make all`              | Build all components (default) |
| `make base`             | Build base simulation only     |
| `make extended`         | Build extended simulation only |
| `make experiments`      | Build experiment runners       |
| `make comparison`       | Build comparison analysis tool |
| `make config-generator` | Build configuration generator  |
| `make clean`            | Remove all build artifacts     |

### Test & Run Targets

| Target                 | Description                        |
| ---------------------- | ---------------------------------- |
| `make configs`         | Generate configuration files       |
| `make test-base`       | Build and test base simulation     |
| `make test-extended`   | Build and test extended simulation |
| `make run-experiments` | Run all experiments                |
| `make run-comparison`  | Run comparative analysis           |
| `make help`            | Display all available targets      |

### Build Configuration

- **Compiler**: `g++` with C++17 standard
- **Optimization**: `-O2` for performance
- **Warnings**: `-Wall -Wextra` for code quality
- **Include Path**: `-Iinclude` for headers

## 📖 Usage Guide

### Running Simulations

**Base Simulation:**

```bash
# Using test data
./bin/simulation_base data/test.txt

# Using custom config
./bin/simulation_base configs/custom_config.txt
```

**Extended Simulation (with advanced features):**

```bash
./bin/simulation_extended data/test2.txt
```

### Running Experiments

**Individual Experiment Runners:**

```bash
# Base experiments
./bin/experiment_base > results/base_results.csv

# Extended experiments
./bin/experiment_extended > results/extended_results.csv
```

**Automated Workflow:**

```bash
make run-experiments    # Runs both base and extended
```

### Generating Visualizations

**Generate all graphs:**

```bash
python3 generate_graphs.py
```

**Generate extended analysis graphs:**

```bash
python3 generate_extended_graphs.py
```

**Output:** Graphs are saved to `plots/` directory with various performance metrics.

## 📁 Project Structure

```
TP2/
├── Makefile                          # Build system configuration
├── README.md                         # This file
├── .clang-format                     # Code formatting rules
├── .gitignore                        # Git ignore patterns
│
├── bin/                              # Compiled executables (generated)
│   ├── simulation_base
│   ├── simulation_extended
│   ├── experiment_base
│   ├── experiment_extended
│   ├── comparison
│   └── config_generator
│
├── data/                             # Input data and test files
│   ├── test.txt                      # Base test input
│   ├── test2.txt                     # Extended test input
│   ├── test3.txt
│   ├── sched1.log                    # Sample scheduling logs
│   └── sched2.log
│
├── docs/                             # Documentation
│   ├── _ED_2025_1_TP2-1.pdf         # Project specification
│   └── gabriel_santos_faria_2024013990.pdf
│
├── include/                          # Header files
│   ├── analysis/
│   │   ├── ConfigGenerator.h         # Config file generator
│   │   └── SimulationMetrics.h       # Performance metrics
│   ├── core/
│   │   ├── ConfigData.h              # Configuration data structure
│   │   ├── ConfigDataExtended.h      # Extended configuration
│   │   ├── Event.h                   # Event system
│   │   ├── Scheduler.h               # Base scheduler
│   │   └── SchedulerExtended.h       # Extended scheduler
│   ├── dataStructures/
│   │   ├── Graph.h                   # Graph implementation
│   │   ├── GraphExtended.h           # Extended graph features
│   │   ├── LinkedList.h              # Linked list
│   │   ├── MinHeap.h                 # Min-heap for events
│   │   ├── Queue.h                   # Queue implementation
│   │   └── Stack.h                   # Stack implementation
│   ├── domains/
│   │   ├── Package.h                 # Package entity
│   │   ├── PackageExtended.h         # Extended package
│   │   ├── Warehouse.h               # Warehouse entity
│   │   └── WarehouseExtended.h       # Extended warehouse
│   └── utils/
│       └── Logger.h                  # Logging utilities
│
├── src/                              # Implementation files
│   ├── analysis/
│   │   ├── ComparisonRunner.cpp      # Algorithm comparison
│   │   ├── ConfigGenerator.cpp
│   │   ├── ConfigGeneratorExtended.cpp
│   │   ├── ExperimentRunner.cpp      # Base experiments
│   │   ├── ExperimentRunnerExtended.cpp
│   │   └── SimulationMetrics.cpp
│   ├── app/
│   │   ├── main.cpp                  # Base simulation entry
│   │   └── mainExtended.cpp          # Extended simulation entry
│   ├── core/
│   ├── dataStructures/
│   ├── domains/
│   └── utils/
│       ├── ConfigGeneratorMain.cpp
│       ├── Logger.cpp
│       └── LoggerExtended.cpp
│
├── obj/                              # Object files (generated)
├── results/                          # Experiment results (generated)
├── configs/                          # Configuration files (generated)
├── plots/                            # Generated graphs (created by scripts)
│
├── generate_graphs.py                # Visualization script
└── generate_extended_graphs.py       # Extended analysis graphs
```

## 🗂️ Data Structures

### Graph

- **Implementation**: Adjacency list representation
- **Features**: Weighted edges, directed/undirected support
- **Algorithms**: Shortest path, traversal, routing

### MinHeap

- **Purpose**: Event queue management
- **Complexity**: O(log n) insertion/deletion
- **Use Case**: Priority-based event scheduling

### Queue & Stack

- **Purpose**: Package processing, routing
- **Implementation**: Dynamic array-based

### LinkedList

- **Purpose**: Route storage, dynamic collections
- **Features**: Efficient insertion/deletion

## 🔧 Algorithms

### Routing Algorithms

- **Optimal Route Calculation**: Dijkstra's shortest path
- **Multi-destination Routing**: Extended routing for complex deliveries
- **Path Optimization**: Cost-based route selection

### Scheduling Algorithms

- **Event-Driven Simulation**: Discrete event processing
- **Priority Scheduling**: Heap-based event queue
- **Warehouse Load Balancing**: Distributed package assignment

## ⚙️ Configuration

### Input File Format

```
<num_warehouses>
<num_packages>
<num_vertices>
<num_edges>
<warehouse_1_info>
<warehouse_2_info>
...
<package_1_info>
<package_2_info>
...
<edge_1_info>
<edge_2_info>
...
```

### Generating Configurations

```bash
make config-generator
./bin/config_generator --size medium --output configs/my_config.txt
```

## 📊 Performance Analysis

### Metrics Collected

- **Execution Time**: Total simulation runtime
- **Event Processing**: Events processed per second
- **Package Throughput**: Packages delivered per time unit
- **Warehouse Utilization**: Load distribution across warehouses
- **Memory Usage**: Peak memory consumption

### Experiment Types

1. **Warehouse Scaling**: Performance vs. number of warehouses
2. **Package Volume**: Throughput vs. package count
3. **Graph Complexity**: Runtime vs. graph size
4. **Algorithm Comparison**: Base vs. Extended implementation

### Visualization Examples

```bash
# Generate all performance graphs
python3 generate_graphs.py

# Output includes:
# - Execution time vs. input size
# - Throughput analysis
# - Warehouse utilization
# - Comparative performance
```

## 💻 Development

### Code Style

- **Standard**: C++17
- **Formatting**: Use `.clang-format` for consistency
- **Naming**: CamelCase for classes, snake_case for functions
- **Documentation**: Inline comments for complex logic

**Apply formatting:**

```bash
find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

### Adding New Components

1. **Add header** in `include/<category>/`
2. **Add implementation** in `src/<category>/`
3. **Update Makefile** to include new source files
4. **Rebuild**: `make clean && make all`

### Debugging

**Enable debug symbols:**

```bash
# Edit Makefile: CXXFLAGS = -std=c++17 -Wall -Wextra -g
make clean && make all
gdb ./bin/simulation_base
```

## 🧪 Experiments

### Running Scalability Tests

```bash
# Generate configs for different sizes
./bin/config_generator --preset small
./bin/config_generator --preset medium
./bin/config_generator --preset large

# Run experiments
make run-experiments

# Analyze results
python3 generate_graphs.py
```

### Custom Experiments

Create custom experiment configurations by modifying `src/analysis/ExperimentRunner.cpp` or using the config generator with custom parameters.

## 🧪 Testing

### Manual Testing

```bash
# Test base simulation
make test-base

# Test extended simulation
make test-extended

# Compare outputs
diff results/base_output.txt results/expected_base.txt
```

### Regression Testing

```bash
# Run all tests and compare with baseline
for config in configs/*.txt; do
    ./bin/simulation_base "$config" > "results/$(basename $config).out"
done
```

### Future Enhancements

- Unit tests with Google Test
- CI/CD with GitHub Actions
- Automated performance regression testing

## ❗ Troubleshooting

### Common Issues

**Build fails with "file not found":**

```bash
# Ensure you're in the TP2 directory
pwd  # Should end with /TP2
make clean && make all
```

**Missing Python dependencies:**

```bash
pip install pandas matplotlib numpy
# or
pip install -r requirements.txt  # if provided
```

**Segmentation fault during simulation:**

- Verify input file format matches specification
- Check array bounds in configuration files
- Enable debug symbols and run with gdb

**Empty or missing results:**

```bash
# Recreate results directory
mkdir -p results configs plots
make run-experiments
```

**Permission denied:**

```bash
chmod +x bin/*
```

## 🤝 Contributing

Contributions are welcome! Here's how to get involved:

### Contribution Ideas

- **New Algorithms**: Implement alternative routing or scheduling algorithms
- **Optimization**: Performance improvements for large-scale simulations
- **Visualization**: Enhanced graphs and analysis tools
- **Documentation**: Tutorials, examples, API documentation
- **Testing**: Unit tests, integration tests, benchmarks

### Workflow

1. **Fork** the repository
2. **Create** a feature branch: `git checkout -b feature/new-algorithm`
3. **Implement** your changes with tests
4. **Format** code: `make format` (if target exists)
5. **Test**: `make all && make test-base && make test-extended`
6. **Commit**: `git commit -m 'Add new routing algorithm'`
7. **Push**: `git push origin feature/new-algorithm`
8. **Open** a Pull Request with detailed description

## 📄 License

This project is developed for **educational purposes** as part of the Data Structures and Algorithms coursework at UFMG (Universidade Federal de Minas Gerais).

For academic use, research, or distribution inquiries, please contact the author.

## 👨‍💻 Authors

**Gabriel Santos Faria**

- **Student ID**: 2024013990
- **Institution**: UFMG - Federal University of Minas Gerais
- **Course**: Data Structures and Algorithms (EDD)
- **Email**: Available in documentation (`docs/`)

### Academic Context

This project (TP2) implements concepts from:

- Discrete event simulation
- Graph algorithms and optimization
- Data structures implementation
- Algorithm analysis and experimentation

---

<div align="center">

**🚀 Built for Learning & Research 📊**

_Combining theoretical knowledge with practical implementation_

[Report Issue](https://github.com/username/repo/issues) · [Request Feature](https://github.com/username/repo/issues) · [View Documentation](docs/)

**Star ⭐ this repo if you find it helpful!**

</div>
