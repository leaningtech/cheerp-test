# Cheerp Test Suite

This directory contains the test suite for the Cheerp compiler. The tests use LLVM's `lit` (LLVM Integrated Tester) framework to verify compiler functionality across different compilation targets.

## Quick Reference

```bash
# Run all jsexport tests (19 tests, all passing)
lit -v unit/jsexport/

# Run tests with specific compilation target(s)
lit --param TARGETS=js unit/jsexport/              # JavaScript only
lit --param TARGETS=wasm,asmjs unit/jsexport/      # WebAssembly + AsmJS only

# Run tests with custom compiler flags
CHEERP_FLAGS="-O3" lit unit/jsexport/              # High optimization
EXTRA_FLAGS="-cheerp-pretty-code" lit unit/jsexport/ # Readable output

# Combine target selection and custom flags
CHEERP_FLAGS="-O2" lit --param TARGETS=js,wasm unit/jsexport/

# Run a single test
lit -v unit/jsexport/namespaces.cpp

# Clean up generated files
find . -type d -name "Output" -exec rm -rf {} +
```

## Next Development Steps

 - Increase general flexibility and functionality of the testing platform
 - Output info on failed test target when testing multiple compilation targets


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

### Selecting Compilation Targets

By default, tests run against **all three compilation targets** (js, wasm, asmjs). You can selectively enable/disable targets using the `TARGETS` parameter:

```bash
# Run tests only for JavaScript target
lit --param TARGETS=js unit/jsexport/

# Run tests for WebAssembly and AsmJS only (skip JavaScript)
lit --param TARGETS=wasm,asmjs unit/jsexport/

# Run tests for a single target
lit --param TARGETS=wasm unit/jsexport/

# Default: all targets (equivalent to TARGETS=js,wasm,asmjs)
lit unit/jsexport/
```

**Available targets:**
- `js` - GenericJS (pure JavaScript output)
- `wasm` - WebAssembly with JavaScript loader
- `asmjs` - AsmJS backend (WebAssembly with asm.js fallback)

### Adding Custom Compilation Flags

You can add custom compiler flags via environment variables:

```bash
# Add optimization flags
CHEERP_FLAGS="-O3" lit unit/jsexport/

# Add extra flags
EXTRA_FLAGS="-cheerp-pretty-code" lit unit/jsexport/

# Combine both
CHEERP_FLAGS="-O2" EXTRA_FLAGS="-cheerp-bounds-check" lit unit/jsexport/

# Example: test with different optimization levels
CHEERP_FLAGS="-O0" lit -v unit/memory/test1.cpp
CHEERP_FLAGS="-O3" lit -v unit/memory/test1.cpp
```

**Common useful flags:**
- `-O0`, `-O1`, `-O2`, `-O3` - Optimization levels
- `-cheerp-pretty-code` - Generate human-readable JavaScript
- `-cheerp-bounds-check` - Enable array bounds checking
- `-cheerp-fix-wrong-func-casts` - Fix function pointer cast issues
- `-frtti` - Enable run-time type information

### Common Test Commands

#### Working Tests (Recommended)

```bash
# Quick smoke test (one test per subdirectory) - DEFAULT
make lit-smoke

# Memory management tests
make lit-memory

# JavaScript export tests
make lit-jsexport

# New self-made tests used for testing the test suite
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
lit -v unit/jsexport/        # JavaScript export tests (working)
lit -v experimental/         # Experimental tests (working)

# Run a single test
lit -v unit/memory/test1.cpp

# Run with specific number of jobs
lit -v -j 8 unit/memory/

# Show all output (verbose)
lit -v -a unit/memory/

# Run tests with specific targets only
lit -v --param TARGETS=js unit/jsexport/
lit -v --param TARGETS=wasm,asmjs unit/memory/

# Run tests with custom flags
CHEERP_FLAGS="-O3" lit -v unit/jsexport/
EXTRA_FLAGS="-cheerp-pretty-code" lit -v unit/jsexport/

# Combine target selection and custom flags
CHEERP_FLAGS="-O2" lit -v --param TARGETS=js,wasm unit/jsexport/

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
- `%cheerp_clang` - Substituted with Cheerp compiler path (includes CHEERP_FLAGS and EXTRA_FLAGS)
- `%s` - Source file path
- `%t` - Temporary output file
- `%node` - Node.js executable
- `%FileCheck` - Pattern matching tool
- `%S` - Directory containing the test

### Conditional Compilation by Target

Tests can conditionally execute based on selected targets:

```cpp
// Run only if 'js' target is selected
// RUN: %if_js %cheerp_clang -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js | %FileCheck %s %endif

// Run only if 'wasm' target is selected
// RUN: %if_wasm %cheerp_clang -target cheerp-wasm %s -o %t.js %endif
// RUN: %if_wasm %node %t.js | %FileCheck %s %endif

// Run only if 'asmjs' target is selected
// RUN: %if_asmjs %cheerp_clang -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t.js %endif

// Use REQUIRES to skip test if target not available
// REQUIRES: js
// REQUIRES: wasm
```

### CHECK Directives
- `// CHECK:` - Expected output pattern
- `// CHECK-DAG:` - Pattern can appear in any order
- `// CHECK-NOT:` - Pattern that should NOT appear
- `// CHECK-NEXT:` - Must appear on the next line
- FileCheck supports regex patterns

## Test Suites

### JavaScript Export Tests (unit/jsexport/)

The `unit/jsexport/` directory contains **19 comprehensive tests** for Cheerp's JavaScript export functionality (`[[cheerp::jsexport]]` attribute). These tests verify that C++ classes, functions, and variables are correctly exported to JavaScript.

**All 19 tests passing (100%):**

```bash
# Run all jsexport tests
lit -v unit/jsexport/

# Run with specific target
lit -v --param TARGETS=js unit/jsexport/

# Test a specific feature
lit -v unit/jsexport/namespaces.cpp
lit -v unit/jsexport/inheritance.cpp
```

**Test coverage includes:**
- Basic exports (empty classes, simple functions)
- Class features (constructors, destructors, member variables, static variables)
- Inheritance and polymorphism
- Namespaces and nested structures
- Parameter types (builtin types, client library types)
- Pointer comparisons and arrays of structs
- Unsafe exports (`[[cheerp::jsexport_unsafe]]`)
- Factory patterns and function invocation
- Pimpl pattern with modules

### Memory Tests (unit/memory/)

Tests for memory management, allocation, and pointer operations.

```bash
lit -v unit/memory/
```

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

### Manual Cleanup

Lit stores test outputs in `Output/` directories:
```bash
# Remove all Output directories
find . -type d -name "Output" -exec rm -rf {} +

# View generated files before cleaning
ls -la unit/jsexport/Output/
```

**Note:** Output directories are useful for debugging failed tests. Consider keeping them until tests pass.

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
