# Cheerp Test Suite

This directory contains the test suite for the Cheerp compiler. The tests use LLVM's `lit` (LLVM Integrated Tester) framework to verify compiler functionality across different compilation targets.

## Current Status

⚠️ **Note**: This test suite is under active development. Currently working tests:
- ✅ **Smoke tests** - Quick sanity check (one test per subdirectory)
- ✅ **Memory tests** - Memory management tests in `unit/memory/`
- ✅ **Experimental tests** - Experimental tests in `experimental/`

Other test categories are available but may have issues. We recommend starting with the working tests above.

## Prerequisites

### Required Software

1. **Cheerp Compiler** - Must be installed at `/opt/cheerp/`
   - Download from: https://leaningtech.com/cheerp/
   - Or install via package manager

2. **Node.js** - Required to run compiled JavaScript/WebAssembly output
   ```bash
   # Ubuntu/Debian
   sudo apt install nodejs
   
   # Or use nvm
   curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.0/install.sh | bash
   nvm install node
   ```

3. **Python 3** and **pip** - For installing lit
   ```bash
   sudo apt install python3 python3-pip
   ```

4. **LLVM lit** - The test runner
   ```bash
   pip3 install lit
   # Or install locally
   pip3 install --user lit
   ```

5. **FileCheck** - LLVM's pattern matching tool (usually comes with lit)
   ```bash
   # If not available, install llvm tools
   sudo apt install llvm
   ```

## Setup

1. **Clone or download this test directory**
   ```bash
   git clone <repository-url> cheerp-tests
   cd cheerp-tests
   ```

2. **Verify Cheerp installation**
   ```bash
   ls /opt/cheerp/bin/clang++
   /opt/cheerp/bin/clang++ --version
   ```

3. **Verify lit installation**
   ```bash
   which lit
   lit --version
   ```

4. **Configure test paths (if needed)**
   
   The test suite uses `lit.cfg` which defines substitutions like `%cheerp_clang`. If your Cheerp installation is not at `/opt/cheerp/`, edit `lit.cfg`:
   ```python
   config.substitutions.append(('%cheerp_clang', '/your/path/to/cheerp/bin/clang++'))
   ```

## Running Tests

### Quick Start

Run smoke tests (default - quick sanity check):
```bash
make
# or
make all
```

Run smoke + experimental tests:
```bash
make quick
```

### Common Test Commands

#### Working Tests (Recommended)

```bash
# Quick smoke test (one test per subdirectory) - DEFAULT
make lit-smoke

# Memory management tests
make lit-memory

# Experimental tests
make lit-experimental

# Run smoke + experimental together
make quick

# Control parallelism
make JOBS=4            # Run with 4 parallel jobs
make JOBS=1            # Run sequentially
```

#### Other Tests (Experimental - May Have Issues)

```bash
# Run all unit tests (WARNING: many tests may fail)
make lit-unit-parallel
make lit-unit-seq

# Run specific test categories (may not work correctly yet)
make lit-codegen       # Code generation tests
make lit-std           # Standard library tests
make lit-dom           # DOM API tests

# Run tests by compilation target (may not work correctly yet)
make lit-preexecutable # Tests all 3 targets
make lit-genericjs     # GenericJS-only tests
make lit-asmjs         # AsmJS backend tests
make lit-wasm-only     # Pure WebAssembly tests
make lit-common        # Common tests (all targets)
```

### Running Tests Directly with lit

You can also run lit directly:
```bash
# Run working tests
lit -v unit/memory/          # Memory tests (working)
lit -v experimental/         # Experimental tests (working)

# Run a single test
lit -v unit/memory/test1.cpp

# Run with specific number of jobs
lit -v -j 8 unit/memory/

# Show all output (verbose)
lit -v -a unit/memory/

# Run all unit tests (warning: many may fail)
lit -v unit/
```

### View Test Results

Test logs are saved to `*.log` files:
```bash
# View smoke test results
cat lit-smoke-output.log

# View memory test results
cat lit-memory-output.log

# View experimental test results
cat lit-experimental-output.log
```

## Test Structure

```
.
├── unit/                   # Main test suite (194 tests)
│   ├── memory/            # Memory management tests ✅ WORKING
│   ├── bitfield/          # Bitfield tests
│   ├── client/            # Client-side DOM API tests
│   ├── closures/          # Closure and lambda tests
│   ├── codegen/           # Code generation tests
│   ├── dom/               # DOM manipulation tests
│   ├── downcast/          # Downcast operation tests
│   ├── exceptions/        # Exception handling tests
│   ├── ffi/               # Foreign Function Interface tests
│   ├── globals/           # Global variable tests
│   ├── jsexport/          # JavaScript export tests
│   ├── randomcfg/         # Random control flow tests
│   ├── static/            # Static member tests
│   ├── std/               # Standard library tests
│   ├── threading/         # Threading tests
│   ├── types/             # Type system tests
│   ├── vbases/            # Virtual base class tests
│   └── virtual/           # Virtual function tests
├── experimental/          # Experimental/development tests ✅ WORKING
├── lit.cfg               # lit configuration file
├── Makefile              # Test automation
└── README.md             # This file
```

**Note**: Most subdirectories are in development. Start with `memory/` and `experimental/` directories.

## Understanding Test Files

Each test file is a C++ source with special comments that lit interprets:

```cpp
// RUN: %cheerp_clang -target cheerp %s -o %t.js
// RUN: %node %t.js | %FileCheck %s

#include <stdio.h>
int main() {
    printf("Hello World\n");
    return 0;
}

// CHECK: Hello World
```

### RUN Directives
- `// RUN:` - Commands to execute
- `%cheerp_clang` - Substituted with Cheerp compiler path
- `%s` - Source file path
- `%t` - Temporary output file
- `%node` - Node.js executable

### CHECK Directives
- `// CHECK:` - Expected output pattern
- `// CHECK-NOT:` - Pattern that should NOT appear
- `// CHECK-NEXT:` - Must appear on the next line
- FileCheck supports regex patterns

## Compilation Targets

Cheerp supports three compilation targets:

1. **cheerp** (GenericJS) - Pure JavaScript output
   ```bash
   /opt/cheerp/bin/clang++ -target cheerp source.cpp -o output.js
   ```

2. **cheerp-wasm** (WebAssembly) - WebAssembly with JS loader
   ```bash
   /opt/cheerp/bin/clang++ -target cheerp-wasm source.cpp -o output.js
   # Produces: output.js (loader) and output.wasm (binary)
   ```

3. **cheerp-wasm with asmjs** - WebAssembly with asm.js fallback
   ```bash
   /opt/cheerp/bin/clang++ -target cheerp-wasm -cheerp-linear-output=asmjs source.cpp -o output.js
   ```

## Test Categories

Tests are organized by functionality and compilation target:

### Currently Working
- **Smoke tests** (`make lit-smoke`) - One test per subdirectory for quick validation
- **Memory tests** (`make lit-memory`) - Memory management and allocation tests
- **Experimental tests** (`make lit-experimental`) - Development and experimental features

### In Development
These test categories are available but may have issues:
- **Pre-executable tests** - Tests compiled to all 3 targets
- **Common tests** - Shared between GenericJS and WebAssembly
- **GenericJS-only** - Tests that only work with pure JavaScript
- **AsmJS-only** - Tests specific to asm.js backend
- **WebAssembly-only** - Tests specific to pure WebAssembly

## Troubleshooting

### lit command not found
```bash
# Install lit
pip3 install --user lit

# Add to PATH
echo 'export PATH=$HOME/.local/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

### FileCheck not found
```bash
# Install LLVM tools
sudo apt install llvm

# Or set FileCheck path in lit.cfg
# config.substitutions.append(('%FileCheck', '/path/to/FileCheck'))
```

### Cheerp compiler not found
```bash
# Verify installation
ls /opt/cheerp/bin/clang++

# If installed elsewhere, update lit.cfg
```

### Tests fail with "No such file or directory"
```bash
# Clean and retry
make clean
make
```

### Node.js version issues
Some tests may require a recent Node.js version:
```bash
node --version  # Should be v14+ recommended
```

## Clean Up

Remove all generated files and logs:
```bash
make clean
```

This removes:
- Test output files (`unit/*/Output/`, `experimental/Output/`)
- Log files (`*.log`)
- Temporary compilation artifacts

## Contributing

When adding new tests:

1. Place them in the appropriate `unit/` subdirectory or `experimental/`
2. Follow the existing test format with RUN and CHECK directives
3. Test all relevant compilation targets
4. Ensure tests pass before committing:
   ```bash
   # Test your specific changes
   lit -v unit/memory/your-new-test.cpp
   
   # Or run the working test suite
   make quick
   ```
5. Update this README if you fix additional test categories

## Help

View all available make targets:
```bash
make help
```

## License

These tests are part of the Cheerp project.
Copyright Leaning Technologies.

## Links

- Cheerp Website: https://leaningtech.com/cheerp/
- Cheerp Documentation: https://docs.leaningtech.com/cheerp/
- LLVM lit Documentation: https://llvm.org/docs/CommandGuide/lit.html
