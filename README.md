# Cheerp Test Suite

This repository contains the Cheerp lit-based test suite for validating the Cheerp compiler toolchain.

## Prerequisites

- Python 3
- `lit` (`pip install lit`)
- Node.js
- `FileCheck` available in PATH (or at the path configured by `lit.cfg`)
- A working Cheerp `clang++` (default `/opt/cheerp/bin/clang++`, override with `--compiler`/`COMPILER`)

## Quick Start

```bash
# Run everything with default targets (wasm, asmjs, genericjs) in regular mode
python3 run_lit_tests.py

# Run a specific directory
python3 run_lit_tests.py tests/jsexport

# Run a single test
python3 run_lit_tests.py tests/memory/kinds.cpp

# Debug a failing test (separate output tree to diff against the main run)
python3 run_lit_tests.py --prefix debug tests/memory/kinds.cpp
```

## Recommended way to run tests

Use `run_lit_tests.py`.

This is the **preferred and safer** interface because it:
- translates high-level options into the correct lit parameters,
- runs one lit invocation per `(target, mode)` combination,
- handles preexecute and determinism flows,
- prints a cumulative summary across combos and writes a per-combo xunit
  report at `Outputs[-<prefix>]/<target>-<mode>/litTestReport.xml`.

From the repository root:

```bash
python3 run_lit_tests.py --help
```

### Common usage examples

#### Basic test runs

```bash
# Default targets (wasm, asmjs, genericjs) in regular mode
python3 run_lit_tests.py

# Run a specific subtree
python3 run_lit_tests.py tests/memory

# Run a single test
python3 run_lit_tests.py tests/memory/kinds.cpp
```

#### Target selection

There are five `(target, mode)` combos, each with its own flag. If no flag is passed, all five run.

| Flag | Combo |
|------|-------|
| `--genericjs` | `(js, regular)` |
| `--wasm` | `(wasm, regular)` |
| `--asmjs` | `(asmjs, regular)` |
| `--preexecute-genericjs` | `(js, preexec)` |
| `--preexecute-asmjs` | `(asmjs, preexec)` |

```bash
# Default: all five combos
python3 run_lit_tests.py

# WebAssembly only
python3 run_lit_tests.py --wasm

# Both linear-memory targets in regular mode
python3 run_lit_tests.py --wasm --asmjs

# JS in regular plus asmjs in preexec
python3 run_lit_tests.py --genericjs --preexecute-asmjs

# Just preexec for js
python3 run_lit_tests.py --preexecute-genericjs
```

#### Compiler and optimization

```bash
# Use a non-default compiler installation
python3 run_lit_tests.py --compiler /opt/cheerp2/bin/clang++

# Parallel jobs and optimization level
python3 run_lit_tests.py -j8 -O2

# Test with optimization level O3
python3 run_lit_tests.py -O3 tests
```

#### Determinism testing

Determinism checks verify that the compiler produces identical output when it compiles the same test twice. They work by re-running lit a second time over a sampled subset of tests with `COMPILE_ONLY=1` into `Outputs-det/`, then byte-diffing those artifacts against the ones from the regular run in `Outputs/`. Mismatches are reported with the differing file list; the two trees stay in `Outputs/` and `Outputs-det/` for `diff -r` inspection.

The check is repeated per `(target, mode)` combination so that a determinism issue surfaces as a `(target, mode, file)` triple. Sampling uses a seeded RNG; if `--determinism-seed` isn't passed, a seed is auto-generated and printed so you can reproduce a given run's sample.

```bash
# Enable the determinism check (20% of tests sampled, fresh seed per run)
python3 run_lit_tests.py --determinism --determinism-probability 0.2

# Reproduce a specific run by passing back the seed it printed
python3 run_lit_tests.py --determinism --determinism-seed=abcdef1234567890

# Determinism only (skip the regular run; do two determinism passes A/B per combo)
python3 run_lit_tests.py --determinism-only

# Exclude specific directories from sampling
python3 run_lit_tests.py --determinism \
  --determinism-exclude-dir threading \
  --determinism-exclude-dir dom
```

#### Compiler flags and features

```bash
# Pretty-printed output
python3 run_lit_tests.py --pretty-code

# Disable LTO
python3 run_lit_tests.py --no-lto

# Generate TypeScript definitions
python3 run_lit_tests.py --typescript

# AddressSanitizer (only for wasm/asmjs targets)
python3 run_lit_tests.py --asan --wasm --asmjs

# High memory mode (heap start at 2GB)
python3 run_lit_tests.py --himem --wasm

# Run with valgrind
python3 run_lit_tests.py --valgrind
```

#### Debugging and diagnostics

```bash
# Save outputs into Outputs-mybuild/ instead of Outputs/
python3 run_lit_tests.py --prefix mybuild

# Dump LLVM IR after each pass
python3 run_lit_tests.py --ir tests/memory
```

## Direct lit usage (advanced)

Direct `lit` invocation is supported, but is less forgiving and easier to misconfigure. Use this when you need fine-grained lit control.

A lit invocation pins exactly one `(target, mode)` combination — both `TARGET` and `MODE` are required.

```bash
# Single target, regular mode
lit --param TARGET=wasm --param MODE=regular tests

# Verbose + parallel
lit -v -j8 --param TARGET=wasm --param MODE=regular tests/memory

# Single test
lit -v --param TARGET=js --param MODE=regular tests/jsexport/namespaces.cpp

# Preexec mode (wasm + preexec is rejected)
lit --param TARGET=js --param MODE=preexec tests
lit --param TARGET=asmjs --param MODE=preexec tests

# Compiler and prefix override
lit \
  --param TARGET=wasm --param MODE=regular \
  --param COMPILER=/opt/cheerp2/bin/clang++ \
  tests

# Extra compile options
lit --param TARGET=js --param MODE=regular \
    --param OPT_LEVEL=O2 --param CHEERP_FLAGS='-cheerp-pretty-code' tests/std
```

## Key lit parameters used by this suite

| Parameter | Description | Example Values |
|-----------|-------------|----------------|
| `TARGET` | Single target to test (required) | `js`, `wasm`, `asmjs` |
| `MODE` | Compile mode (required) | `regular`, `preexec` |
| `OPT_LEVEL` | Optimization level | `O0`, `O1`, `O2`, `O3` |
| `CHEERP_FLAGS` | Additional compiler flags | `-cheerp-pretty-code` |
| `COMPILER` | Path to clang++ (default `/opt/cheerp/bin/clang++`) | `/opt/cheerp2/bin/clang++` |
| `OUTPUT_PREFIX` | Prefix for the output tree | Any string |
| `COMPILE_ONLY` | Skip node/FileCheck (used by determinism pass) | `0`, `1` |

## Reports and outputs

`run_lit_tests.py` writes one xunit report per `(target, mode)` combination at
`Outputs[-<prefix>]/<target>-<mode>/litTestReport.xml`. After the run, a
cumulative summary is printed when more than one combo ran.

Test artifacts (and their xunit report) are placed under

```
Outputs[-<prefix>]/<target>-<mode>/<relpath>/Output/<testname>.tmp.*
Outputs[-<prefix>]/<target>-<mode>/litTestReport.xml
```

so that runs across `(target, mode)` don't collide and `rm -rf Outputs/` cleans both. With `--prefix mybuild` the root becomes `Outputs-mybuild/` instead of `Outputs/`. Determinism passes write into `Outputs[-<prefix>]-det/...`.

For example, the default (all-five-combos) run produces:

```
Outputs/js-regular/bitfield/Output/test1.cpp.tmp.js
Outputs/wasm-regular/bitfield/Output/test1.cpp.tmp.js
Outputs/asmjs-regular/bitfield/Output/test1.cpp.tmp.js
Outputs/js-preexec/bitfield/Output/test1.cpp.tmp.log
Outputs/asmjs-preexec/bitfield/Output/test1.cpp.tmp.log
```

## Writing tests

### Test file structure

Tests are C/C++ source files (`.c`, `.cpp`) with special RUN directives in comments that tell lit how to compile and verify the test. The canonical form is a single RUN line:

```cpp
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include "tests.h"

void webMain() {
    // CHECK: result: 42
    assertPrint("result:", 42);
}
```

That one line works in all five `(target, mode)` combinations:
- `(js, regular)`, `(wasm, regular)`, `(asmjs, regular)` — `%compile` is the
  per-target compile command and `%run` is `node %t.js 2>&1`.
- `(js, preexec)`, `(asmjs, preexec)` — `%compile` is the preexec compile
  command (which prints test output to stderr); `2>%t.log` captures it and
  `%run` is `cat %t.log`.

### Substitutions

| Macro | Meaning |
|-------|---------|
| `%compile` | Compile command for this invocation's `(target, mode)` |
| `%run` | `node %t.js 2>&1` in regular mode; `cat %t.log` in preexec mode |
| `%target` | Literal target name: `js`, `wasm`, or `asmjs`. Useful for embedding the target into a per-target FileCheck prefix. |
| `%FileCheck` | Path to FileCheck |
| `%node` | Node.js executable |
| `%helpers` | Path to repo `helpers/` directory |
| `%s` | Source file path |
| `%t` | Per-test unique base path; `%t.js`, `%t.log`, `%t-<suffix>.js` are siblings |

### REQUIRES (test feature gating)

Use `// REQUIRES:` to restrict a test to specific features:

```cpp
// REQUIRES: regular            // skip in preexec mode
// REQUIRES: js                 // run only when TARGET=js
// REQUIRES: linear-memory      // run only on wasm or asmjs
// REQUIRES: regular, no-asan   // both must hold
```

**Available features (set per invocation by `lit.cfg`):**
- Targets: `js`, `wasm`, `asmjs`
- Mode: `regular` or `preexecution`
- Linear-memory subset: `linear-memory` (= wasm or asmjs)
- Packed-tests subset: `packed_tests` (= wasm or js)
- Sanitizers: `no-asan` (set when ASAN is not in effect)

### Per-target divergence

For tests where the only per-target divergence is the FileCheck prefix, embed
`%target` into the prefix and add per-target CHECK lines (using
`--allow-unused-prefixes` so targets without a matching CHECK line don't
error):

```cpp
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck --check-prefixes=CHECK,CHECK_%target --allow-unused-prefixes %s

// CHECK: common output
// CHECK_js:   js-only output
// CHECK_wasm: wasm-only output
```

For multi-artifact tests (e.g. jsexport modules tested in vanilla / es6 /
commonjs / closure forms), use one set of RUN lines that runs unchanged for
every target. Name the artifacts explicitly so they coexist; don't use `%run`
because the driver isn't `%t.js`:

```cpp
// REQUIRES: regular
// RUN: %compile -o %t-vanilla.js %s
// RUN: %node %t-vanilla.js 2>&1 | %FileCheck %s
// RUN: %compile -cheerp-make-module=es6      -o %t-es6.mjs       %s
// RUN: python3 %helpers/run_cheerp_module.py --module=es6      %t-es6.mjs      2>&1 | %FileCheck %s
// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js  %s
// RUN: python3 %helpers/run_cheerp_module.py --module=commonjs %t-commonjs.js 2>&1 | %FileCheck %s
// RUN: %compile -cheerp-make-module=closure  -o %t-closure.js   %s
// RUN: %node %t-closure.js 2>&1 | %FileCheck %s
```

`%t-X.js` is a sibling of `%t.js` (lit guarantees `%T` exists), so no `mkdir`
is needed.

### FileCheck patterns

Use `CHECK:` comments to verify program output:

```cpp
void webMain() {
    // CHECK: result: 42
    assertPrint("result:", 42);

    // CHECK-NEXT: done
    assertPrint("done");
}
```

**Common FileCheck directives:**
- `CHECK:` — must appear in output
- `CHECK-NEXT:` — must appear on the next line
- `CHECK-NOT:` — must not appear in output
- `CHECK-DAG:` — must appear, order doesn't matter
- `CHECK-LABEL:` — marks a labeled section

### Finding test artifacts

Lit creates a per-test layout:

```
Outputs/<target>-<mode>/<relpath>/Output/<testname>.tmp.{js,log,wasm,...}
```

For example, artifacts for `tests/memory/kinds.cpp` in `(js, regular)`:

```bash
ls Outputs/js-regular/memory/Output/kinds.cpp.tmp*
```

### Writing outputs into a separate tree

Use `--prefix` to write into `Outputs-<prefix>/` so multiple runs can coexist for diffing:

```bash
python3 run_lit_tests.py --prefix debug tests/memory/kinds.cpp
ls Outputs-debug/wasm-regular/memory/Output/kinds.cpp.tmp*
```

### Verbose lit output

```bash
lit -a --param TARGET=wasm --param MODE=regular tests/memory/kinds.cpp
```

## Reference: All run_lit_tests.py options

```
Options:
  -h, --help            Show help message and exit
  -O OPTLEVEL           Optimization level (default -O1)
  -j JOBS               Number of parallel jobs (default 1)

  Target selection (each flag is one (target, mode) combo;
  no flags = all five combos):
  --wasm                  Run the wasm target
  --asmjs                 Run the asmjs target
  --genericjs             Run the generic js target
  --preexecute-genericjs  Run the generic js target inside the PreExecuter
  --preexecute-asmjs      Run the asmjs target inside the PreExecuter

  Compiler configuration:
  --compiler PATH       Path to clang++ (default /opt/cheerp/bin/clang++)

  Compiler flags:
  --pretty-code         Compile with -cheerp-pretty-code
  --no-lto              Compile with -cheerp-no-lto
  --typescript          Also generate .d.ts files
  --asan                Test using AddressSanitizer (only asmjs/wasm)
  --himem               Run tests with heap start at 2GB
  --valgrind            Run with valgrind activated

  Determinism testing:
  --determinism         Recompile a sampled subset per (target, mode) and
                        diff artifacts; reports pass/fail per test
  --determinism-probability P
                        Probability that a test is included in the sample
                        (0.0-1.0)
  --determinism-only    Skip the regular run; do two determinism passes A/B
                        per (target, mode)
  --determinism-exclude-dir DIR
                        Exclude directory name from sampling (repeatable)
  --determinism-seed STR
                        Seed string for the sampling RNG (auto-generated
                        and printed if omitted)

  Debugging:
  --prefix PREFIX       Write outputs to Outputs-<PREFIX>/ instead of Outputs/
  --ir                  Dump the LLVM IR after each pass
```

## Protocol for adding new tests

When adding a new test to the suite:

1. **Choose the appropriate directory** (e.g. `tests/memory/`, `tests/std/`, `tests/jsexport/`).

2. **Create a `.cpp` or `.c` file** with a descriptive name.

3. **Add a single RUN directive** at the top:
   ```cpp
   // RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s
   ```

4. **Add REQUIRES directives** if the test isn't valid for every `(target, mode)` it would otherwise run in:
   ```cpp
   // REQUIRES: linear-memory
   // REQUIRES: no-asan
   ```

5. **Write the test code** with CHECK comments for verification:
   ```cpp
   void webMain() {
       // CHECK: expected output
       assertPrint("expected output");
   }
   ```

6. **Test locally** before committing:
   ```bash
   python3 run_lit_tests.py path/to/your/test.cpp
   ```

7. **Verify across all relevant (target, mode) combos**:
   ```bash
   python3 run_lit_tests.py path/to/your/test.cpp   # default = all five combos
   ```

8. **Check determinism** for compiler changes:
   ```bash
   python3 run_lit_tests.py --determinism --determinism-probability 1.0 path/to/your/test.cpp
   ```

9. **Document non-obvious behavior** in comments within the test file.
