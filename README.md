# CPUScope

[![CI](https://github.com/codingwithmagga/cpu_scope/actions/workflows/ci.yml/badge.svg)](https://github.com/codingwithmagga/cpu_scope/actions)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-brightgreen.svg)](https://en.cppreference.com/w/cpp/20)

**Performance forensics and optimization toolkit for CPU-bound HPC workloads**

---

## Overview

Developers building high-performance computing systems face a fundamental challenge: bridging the gap between application behavior and CPU architecture bottlenecks. Performance profilers generate terabytes of data; architects spend days correlating cache misses with code regions; optimization efforts often miss critical hotspots entirely.

CPUScope automates this workflow. By combining low-level hardware counters, intelligent bottleneck detection, and actionable optimization suggestions, it transforms raw profiling data into concrete insights. The toolkit targets HPC developers, performance engineers, and system architects who demand reproducible, architecture-aware optimization.

This project exemplifies Developer Technology engineering: bridging the gap between hardware capabilities and software optimization through open, maintainable, production-grade tooling. Every feature prioritizes developer experience—from one-command profiling to reference kernels with before/after comparisons.

---

## Key Features

- **Low-level profiling via Linux `perf` and hardware counters** – Direct access to CPU events (cache misses, branch mispredictions, SIMD utilization)
- **Bottleneck detection** – Automatic classification of workloads as memory-bound, compute-bound, or branch-limited
- **Optimization advisor** – Context-aware suggestions with code annotations
- **Reference optimized kernels** – Before/after comparisons demonstrating optimization strategies
- **Reproducible benchmark harness** – Scaling analysis across problem sizes and thread counts
- **CPU architecture detection and dynamic adaptation** – Automatic tuning for target CPU (Intel, AMD, ARM)

---

## MVP Scope

| Feature | Status | Target Milestone |
|---------|--------|--------------|
| Project Infrastructure | ✅ | [#1](https://github.com/codingwithmagga/cpu_scope/milestone/1) |
| Core Profiling Engine | 🚧 | [#2](https://github.com/codingwithmagga/cpu_scope/milestone/2) |
| Benchmark Harness | 🚧 | [#3](https://github.com/codingwithmagga/cpu_scope/milestone/2) |
| Sample Kernels | 🚧 | [#4](https://github.com/codingwithmagga/cpu_scope/milestone/4) |
| Bottleneck Analyzer | 🚧 | [#5](https://github.com/codingwithmagga/cpu_scope/milestone/5) |
| CLI & MVP Integration | 🚧 | [#6](https://github.com/codingwithmagga/cpu_scope/milestone/6) |

See the [GitHub Issues](https://github.com/codingwithmagga/cpu_scope/issues) tracker for the full roadmap.

---

## Quick Start

### Prerequisites

- **CMake** ≥ 3.25
- **GCC** ≥ 13 or **Clang** ≥ 17
- **Linux** 5.x+ (for `perf` and hardware counter access)
- **Build essentials** (ninja, ccache recommended)
- Proper `perf` permissions (see [Troubleshooting](#troubleshooting))

### Clone & Build

```bash
git clone https://github.com/codingwithmagga/cpu_scope.git
cd cpu_scope
mkdir build
cd build
cmake -S .. -B . -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCPUSCOPE_BUILD_TESTS=ON
cmake --build .
```

### Run Tests

```bash
cd build
ctest --output-on-failure
```

### Run First Benchmark

Note: Currently not implemented

```bash
./build/cpuscope_cli run spmv --size 1000
```

---

## Development

### Code Formatting

Format all source files to project standards:

```bash
scripts/format.sh
```

Check formatting compliance (CI use):

```bash
scripts/check-format.sh
```

### Static Analysis

Run clang-tidy linting (requires compile_commands.json):

```bash
scripts/lint.sh
```

### Architecture

- **`lib/`** – Core library (profiling, detection, analysis)
- **`cli/`** – Command-line interface
- **`tests/`** – Unit tests (Google Test)

---

## Troubleshooting

### `perf` Permission Denied

Enable perf for non-root users:

```bash
echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid
```

Or run with elevated privileges:

```bash
sudo ./build/cpuscope_cli run spmv --size 1000
```

### CMake Configuration Failures

Ensure dependencies are installed:

```bash
# Ubuntu/Debian
sudo apt-get install -y cmake ninja-build ccache

# Fedora/RHEL
sudo dnf install -y cmake ninja-build ccache
```

---

## Contributing

Contributions are welcome! Please:

1. Follow the [C++ code style](https://google.github.io/styleguide/cppguide.html) (enforced by `.clang-format` and `.clang-tidy`)
2. Add tests for new features
3. Ensure all CI checks pass before submitting a PR
4. Include clear commit messages

---

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

---

## Contact

For questions or suggestions, open an [issue](https://github.com/codingwithmagga/cpu_scope/issues) on GitHub.
