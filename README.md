# Cheerp Test Suite

This repository contains the Cheerp lit-based test suite for validating the Cheerp compiler toolchain.

## Prerequisites

- Python 3
- `lit` (`pip install lit`)
- Node.js
- `FileCheck` available in PATH (or at the path configured by `lit.cfg`)
- A working Cheerp toolchain install (default `/opt/cheerp`, or override with `COMPILER`/`CHEERP_PREFIX`)

## Quick Start

```bash
# Run everything with default targets (wasm, asmjs, genericjs)
python3 run_lit_tests.py

# Run a specific directory
python3 run_lit_tests.py /unit/jsexport

# Run a single test
python3 run_lit_tests.py tests/memory/test1.cpp

# Debug a failing test (keeps logs)
python3 run_lit_tests.py --keep --prefix debug tests/memory/test1.cpp
```

## Recommended way to run tests

Use `run_lit_tests.py`.

This is the **preferred and safer** interface because it:
- translates high-level options into the correct lit parameters,
- keeps target/mode selection consistent,
- handles preexecute and determinism flows,
- generates merged test reports (`litTestReport.test`).

From the repository root:

```bash
python3 run_lit_tests.py --help
```

### Common usage examples

#### Basic test runs

```bash
# Default targets (wasm, asmjs, genericjs) on the whole suite
python3 run_lit_tests.py --suite ./cheerp-test

# Run a specific subtree
python3 run_lit_tests.py /memory

# Run a single test
python3 run_lit_tests.py tests/memory/test1.cpp

# Run with all targets and modes (equivalent to --wasm --asmjs --genericjs --preexecute --preexecute-asmjs)
python3 run_lit_tests.py --all .
```

#### Target selection

```bash
# Select targets explicitly
python3 run_lit_tests.py --wasm --asmjs /unit
python3 run_lit_tests.py --genericjs /unit/jsexport

# WebAssembly only
python3 run_lit_tests.py --wasm .

# All linear-memory targets
python3 run_lit_tests.py --wasm --asmjs .
```

#### Compiler and optimization

```bash
# Use a non-default compiler installation
python3 run_lit_tests.py \
  --compiler /opt/cheerp2/bin/clang++ \
  --cheerp-prefix /opt/cheerp2 \
  tests

# Parallel jobs and optimization level
python3 run_lit_tests.py -j8 -O2 .

# Test with optimization level O3
python3 run_lit_tests.py -O3 /unit
```

#### Preexecute modes

```bash
# Preexecute (wasm/js target)
python3 run_lit_tests.py --preexecute .

# Preexecute asmjs target
python3 run_lit_tests.py --preexecute-asmjs .

# Both preexecute modes
python3 run_lit_tests.py --preexecute --preexecute-asmjs .
```

#### Quick mode (efficient combined testing)

Quick mode runs regular and preexecution modes in a single lit invocation, reducing overhead:

```bash
# Run all modes in parallel (regular + preexec)
python3 run_lit_tests.py --quick .

# Quick mode with specific targets
python3 run_lit_tests.py --quick --wasm --asmjs .
```

**Note:** Quick mode is ignored when `--determinism-only` is set.

#### Determinism testing

Determinism checks verify that the compiler produces identical output when it compiles the same test twice. They work by running lit a second time over a sampled subset of tests into `Outputs-det/`, then byte-diffing those artifacts against the ones from the regular run in `Outputs/`. Mismatches are copied to `determinism_failures_compile_only/` for inspection.

Sampling uses a seeded RNG. If `--determinism-seed` isn't passed, a seed is auto-generated and printed so you can reproduce a given run's sample by passing it back.

```bash
# Enable the determinism check (20% of tests sampled, fresh seed per run)
python3 run_lit_tests.py --determinism --determinism-probability 0.2 tests

# Reproduce a specific run by passing back the seed it printed
python3 run_lit_tests.py --determinism --determinism-seed=abcdef1234567890 tests

# Determinism only (skip the regular run; do two determinism passes A/B)
python3 run_lit_tests.py --determinism-only tests

# Exclude specific directories from sampling
python3 run_lit_tests.py --determinism \
  --determinism-exclude-dir threading \
  --determinism-exclude-dir dom tests
```

#### Compiler flags and features

```bash
# Pretty-printed output
python3 run_lit_tests.py --pretty-code .

# Disable LTO
python3 run_lit_tests.py --no-lto .

# Generate TypeScript definitions
python3 run_lit_tests.py --typescript .

# AddressSanitizer (only for wasm/asmjs targets)
python3 run_lit_tests.py --asan --wasm --asmjs .

# High memory mode (heap start at 2GB)
python3 run_lit_tests.py --himem --wasm .

# Run with valgrind
python3 run_lit_tests.py --valgrind .
```

#### Debugging and diagnostics

```bash
# Keep log files for failed tests
python3 run_lit_tests.py --keep .

# Save test outputs with prefix
python3 run_lit_tests.py --prefix mybuild .

# Print commands as they're executed
python3 run_lit_tests.py --print-cmd .

# Print test statistics summary
python3 run_lit_tests.py --print-stats .

# Show per-test timing information
python3 run_lit_tests.py --time .

# Dump LLVM IR after each pass
python3 run_lit_tests.py --ir /unit/memory
```

## Direct lit usage (advanced)

Direct `lit` invocation is supported, but is less forgiving and easier to misconfigure.
Use this when you need fine-grained lit control.

From the repository root:

```bash
# Basic run
lit tests

# Verbose + parallel
lit -v -j8 tests/unit/memory

# Single test
lit -v tests/unit/jsexport/namespaces.cpp

# Target selection (IMPORTANT: parameter name is TARGET)
lit --param TARGET=js tests/unit/jsexport
lit --param TARGET=wasm,asmjs tests/unit

# Compiler and prefix override
lit \
  --param COMPILER=/opt/cheerp2/bin/clang++ \
  --param CHEERP_PREFIX=/opt/cheerp2 \
  tests

# Extra compile options
lit --param OPT_LEVEL=O2 --param CHEERP_FLAGS='-cheerp-pretty-code' ./std/test1.cpp

# Combined regular + preexec mode
lit --param TARGET=wasm,js --param REG=1 --param PRE_EX=1 tests

# Preexec-only mode
lit --param TARGET=js --param REG=0 --param PRE_EX=j tests
```

## Key lit parameters used by this suite

| Parameter | Description | Example Values |
|-----------|-------------|----------------|
| `TARGET` | Comma-separated targets to test | `js`, `wasm`, `asmjs`, `wasm,asmjs` |
| `OPT_LEVEL` | Optimization level | `O0`, `O1`, `O2`, `O3` |
| `CHEERP_FLAGS` | Additional compiler flags | `-cheerp-pretty-code` |
| `EXTRA_FLAGS` | Extra flags appended to compile commands | Any compiler flags |
| `PRE_EX` | Preexecute mode | `0` (off), `j` (js), `a` (asmjs), `1` (both) |
| `REG` | Regular mode enable/disable | `0` (off), `1` (on, default) |
| `COMPILER` | Compiler alias/path | `cheerp`, `local`, or explicit path |
| `CHEERP_PREFIX` | Toolchain prefix | `/opt/cheerp`, `/opt/altcompiler` |
| `KEEP_LOGS` | Keep individual test logs | `0`, `1` |
| `OUTPUT_PREFIX` | Prefix for saved outputs | Any string |
| `PRINT_STATS` | Print test statistics | `0`, `1` |
| `TIME_TESTS` | Per-test timing | `0`, `1` |
| `HIMEM` | High memory mode | `0`, `1` |
| `IR` | Dump LLVM IR | `0`, `1` |

## Reports and outputs

When using `run_lit_tests.py`, these files are generated in `cheerp-test/`:
- `litTestReport.xml` (regular targets)
- `litTestReport_preexec.xml` (preexecute js mode)
- `litTestReport_preexec_asmjs.xml` (preexecute asmjs mode)
- `litTestReport.test` (merged summary report)

Temporary artifacts are placed under `Outputs/<relpath>/Output/<testname>.tmp/` (mirroring the source tree) by lit; keep them when debugging failures using `--keep`.

When using `--prefix <name>` (or `--param OUTPUT_PREFIX=<name>`), the output tree is written to `Outputs-<name>/` instead of `Outputs/`, so multiple configurations can coexist for diffing.

## Writing tests

### Test file structure

Tests are C/C++ source files (`.c`, `.cpp`) with special RUN directives in comments that tell lit how to compile and verify the test:

```cpp
// RUN: %compile_mode_js %s -o %t/j && %then_run_js | %FileCheck %s
// RUN: %compile_mode_wasm %s -o %t/w && %then_run_wasm | %FileCheck %s

#include <cheerp/clientlib.h>

void webMain() {
    // CHECK: Hello from Cheerp!
    client::console.log("Hello from Cheerp!");
}
```

### Available test features (REQUIRES directive)

Use `REQUIRES:` to specify which configurations a test needs:

```cpp
// REQUIRES: wasm
// REQUIRES: linear-memory
// REQUIRES: no-asan
```

**Available features:**
- **Targets:** `js`, `wasm`, `asmjs`, `linear-memory` (wasm or asmjs), `packed_tests` (wasm or js)
- **Modes:** `regular`, `preexecution`
- **Sanitizers:** `asan`, `no-asan`
- **Tools:** `valgrind`, `no-valgrind`

### Compile macros

Use these substitutions in RUN directives to compile for specific targets:

| Macro | Description |
|-------|-------------|
| `%compile_mode_js` | Compile for genericjs target |
| `%compile_mode_wasm` | Compile for WebAssembly target |
| `%compile_mode_asmjs` | Compile for asm.js target |

For explicit mode control (combined regular + preexec mode):

| Macro | Description |
|-------|-------------|
| `%compile_mode_regular_js` | Compile for js in regular mode only |
| `%compile_mode_regular_wasm` | Compile for wasm in regular mode only |
| `%compile_mode_regular_asmjs` | Compile for asmjs in regular mode only |
| `%compile_mode_preexec_js` | Compile for js in preexec mode only |
| `%compile_mode_preexec_asmjs` | Compile for asmjs in preexec mode only |

### Run macros

After compilation, use these to execute the generated code:

| Macro | Description |
|-------|-------------|
| `%then_run_js` | Run genericjs output with node |
| `%then_run_wasm` | Run WebAssembly output with node |
| `%then_run_asmjs` | Run asm.js output with node |
| `%then_run_regular_js` | Run js output (regular mode only) |
| `%then_run_regular_wasm` | Run wasm output (regular mode only) |
| `%then_run_regular_asmjs` | Run asmjs output (regular mode only) |
| `%then_run_preexec_js` | Run js output (preexec mode - usually no-op) |
| `%then_run_preexec_asmjs` | Run asmjs output (preexec mode - usually no-op) |

**Note:** In preexecution mode, the compiler itself executes the code during compilation and outputs CHECK-able results to stderr, so `%then_run_preexec_*` macros typically resolve to no-op.

### Mode-specific test lines

When writing tests that behave differently in regular vs preexecution mode, use these prefixes:

```cpp
// Regular mode: compile and run with node
// RUN: regular_only %compile_mode_js %s -o %t/j %then_run_regular_js | %FileCheck %s

// Preexec mode: compiler executes during compilation
// RUN: preexec_only %compile_mode_js %s -o %t/j %then_run_preexec_js 2>&1 | %FileCheck %s
```

The `regular_only` and `preexec_only` prefixes control which lines execute based on the test mode:
- **Combined mode** (`REG=1 PRE_EX=1`): Both types of lines execute
- **Regular-only mode** (default): Only `regular_only` lines execute
- **Preexec-only mode** (`REG=0 PRE_EX=j/a`): Only `preexec_only` lines execute

### Conditional execution by target

Use `run_if_<target>` to execute commands only when a specific target is enabled:

```cpp
// RUN: run_if_wasm %compile_mode_wasm %s -o %t/w && node %t/w
// RUN: run_if_js %compile_mode_js %s -o %t/j && node %t/j
```

If the target is disabled, the entire line is replaced with `true` (no-op).

### Other substitutions

| Macro | Description |
|-------|-------------|
| `%s` | Source file path |
| `%t` | Temporary output directory |
| `%FileCheck` | Path to FileCheck tool |
| `%node` | Node.js executable |
| `%valgrind` | Valgrind command (if enabled) |

### FileCheck patterns

Use `CHECK:` comments to verify compiler output or program behavior:

```cpp
void webMain() {
    // CHECK: result: 42
    client::console.log("result:", 42);
    
    // CHECK-NEXT: done
    client::console.log("done");
}
```

**Common FileCheck directives:**
- `CHECK:` - Must appear in output
- `CHECK-NEXT:` - Must appear on the next line
- `CHECK-NOT:` - Must not appear in output
- `CHECK-DAG:` - Must appear, order doesn't matter
- `CHECK-LABEL:` - Marks a labeled section

### Example test template

```cpp
// RUN: %compile_mode_js %s -o %t/j %then_run_js | %FileCheck %s
// RUN: %compile_mode_wasm %s -o %t/w %then_run_wasm | %FileCheck %s
// RUN: %compile_mode_asmjs %s -o %t/a %then_run_asmjs | %FileCheck %s

#include <cheerp/clientlib.h>

// Test description: verify basic console output functionality

void webMain() {
    // CHECK: Test output: 123
    client::console.log("Test output:", 123);
}
```

### Finding test artifacts

Lit creates temporary directories for each test under `Outputs/<relpath>/Output/<testname>.tmp/`, mirroring the source layout. For example, artifacts for `tests/memory/test1.cpp` land in `Outputs/memory/Output/test1.cpp.tmp/`.

```bash
# List recent test artifacts
find Outputs -path '*/Output/*.tmp' -type d -mmin -5

# Inspect a specific test's output
ls -la Outputs/memory/Output/test1.cpp.tmp/
```

### Keeping logs for debugging

By default, logs are cleaned up. Use `--keep` to preserve them:

```bash
python3 run_lit_tests.py --keep tests/memory/test1.cpp
```

### Extracting test outputs

Use `--prefix` to write the output tree into `Outputs-<prefix>/` instead of `Outputs/`, so you can keep results from multiple runs side-by-side for diffing:

```bash
python3 run_lit_tests.py --prefix debug tests/memory/test1.cpp

# Artifacts land under Outputs-debug/
ls -la Outputs-debug/memory/Output/test1.cpp.tmp/
```

### Verbose lit output

For detailed test execution information:

```bash
# With run_lit_tests.py
python3 run_lit_tests.py --print-cmd .

# With direct lit
lit -a tests/memory/test1.cpp
```

## Reference: All run_lit_tests.py options

```
Options:
  -h, --help            Show help message and exit
  -O OPTLEVEL           Optimization level (default -O1)
  -j JOBS               Number of parallel jobs (default 1)
  
  Target selection:
  --wasm                Run tests in wasm mode
  --asmjs               Run tests in asmjs mode
  --genericjs           Run tests in genericjs mode
  --all                 Run all test kinds [genericjs/asmjs/wasm/preexecute/preexecute-asmjs]
  
  Execution modes:
  --preexecute          Run tests inside PreExecuter (js/wasm)
  --preexecute-asmjs    Run tests inside PreExecuter in asmjs mode
  --quick               Run all modes in parallel (combined regular+preexec)
  
  Compiler configuration:
  --compiler PATH       Compiler alias/path (e.g. cheerp, local, /opt/cheerp2/bin/clang++)
  --cheerp-prefix PATH  Cheerp install prefix (e.g. /opt/cheerp)
  
  Compiler flags:
  --pretty-code         Compile with -cheerp-pretty-code
  --no-lto              Compile with -cheerp-no-lto
  --typescript          Also generate .d.ts files
  --asan                Test using AddressSanitizer (only asmjs/wasm)
  --himem               Run tests with heap start at 2GB
  --valgrind            Run with valgrind activated
  
  Determinism testing:
  --determinism         Recompile a sampled subset after the regular run and
                        diff artifacts; reports pass/fail per test
  --determinism-probability P
                        Probability that a test is included in the sample (0.0-1.0)
  --determinism-only    Skip the regular run; do two determinism passes A/B
  --determinism-exclude-dir DIR
                        Exclude directory name from sampling (repeatable)
  --determinism-seed STR
                        Seed string for the sampling RNG (auto-generated
                        and printed if omitted)
  
  Debugging:
  --keep                Don't delete log files for individual tests
  --prefix PREFIX       Write outputs to Outputs-<PREFIX>/ instead of Outputs/
  --print-cmd           Print commands as they're executed
  --print-stats         Print a summary of test result numbers
  --time                Print compilation and run time for each test
  --ir                  Dump the LLVM IR after each pass
```

## Protocol for adding new tests

When adding a new test to the suite:

1. **Choose the appropriate directory** based on what you're testing (unit/, memory/, std/, etc.)

2. **Create a `.cpp` or `.c` file** with a descriptive name

3. **Add RUN directives** at the top:
   ```cpp
   // RUN: %compile_mode_js %s -o %t/j %then_run_js | %FileCheck %s
   // RUN: %compile_mode_wasm %s -o %t/w %then_run_wasm | %FileCheck %s
   ```

4. **Add REQUIRES directives** if your test needs specific features:
   ```cpp
   // REQUIRES: linear-memory
   // REQUIRES: no-asan
   ```

5. **Write the test code** with CHECK comments for verification:
   ```cpp
   void webMain() {
       // CHECK: expected output
       client::console.log("expected output");
   }
   ```

6. **Test locally** before committing:
   ```bash
   python3 run_lit_tests.py path/to/your/test.cpp
   ```

7. **Verify across all relevant targets**:
   ```bash
   python3 run_lit_tests.py --all path/to/your/test.cpp
   ```

8. **Check determinism** for compiler changes:
   ```bash
   python3 run_lit_tests.py --determinism --determinism-probability 1.0 path/to/your/test.cpp
   ```

9. **Document non-obvious behavior** in comments within the test file
