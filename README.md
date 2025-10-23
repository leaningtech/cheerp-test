# Cheerp Test Suite

This directory contains the test suite for the Cheerp compiler. The tests use LLVM's `lit` (LLVM Integrated Tester) framework to verify compiler functionality across different compilation targets.

## Current Status

⚠️ **Note**: This test suite is under active development. Currently working tests:
- ✅ **Smoke tests** - Quick sanity check (one test per subdirectory)
- ✅ **Memory tests** - Memory management tests in `unit/memory/`
- ✅ **Experimental tests** - Experimental tests in `experimental/`

Other test categories are available but may have issues. We recommend starting with the working tests above.

## next dev steps

 - expand test functionality to all unit test subdirectories
 - allow for flexible use of compilation flags
 - increase general flexibility and functionality of the testing platform 


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
   git clone github.com/leaningtech/cheerp-test cheerp-tests
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

## Running Tests

### Quick Start

Run smoke tests (default - quick sanity check):
```bash
make
# or
make all
```


### Common Test Commands

#### Working Tests (Recommended)

```bash
# Quick smoke test (one test per subdirectory) - DEFAULT
make lit-smoke

# Memory management tests
make lit-memory

# new self-made tests used for testing the test suite
make lit-experimental

# Control parallelism
make JOBS=4            # Run with 4 parallel jobs
make JOBS=1            # Run sequentially
```

#### Other Tests (Experimental - May Have Issues)

```bash
# Run all unit tests (WARNING: many tests may fail)
make lit-unit-parallel
make lit-unit-seq

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

1. Place them in the appropriate `unit/` subdirectory
2. Follow the existing test format with RUN and CHECK directives
3. Test all relevant compilation targetsgit 
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
