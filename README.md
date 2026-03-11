# Cheerp Test Suite

This repository contains the Cheerp lit-based test suite.

## Prerequisites

- Python 3
- `lit` (`pip install lit`)
- Node.js
- `FileCheck` available in PATH (or at the path configured by `lit.cfg`)
- A working Cheerp toolchain install (default `/opt/cheerp`, or override with `COMPILER`/`CHEERP_PREFIX`)

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

### Common script examples

```bash
# Default targets (wasm, asmjs, genericjs) on the whole suite
python3 run_lit_tests.py --suite ./cheerp-test

# Run a specific subtree
python3 run_lit_tests.py /memory

# Run a single test
python3 run_lit_tests.py memory/test1.cpp

# Select targets explicitly
python3 run_lit_tests.py --wasm --asmjs /unit
python3 run_lit_tests.py --genericjs /unit/jsexport

# Use a non-default compiler installation
python3 run_lit_tests.py \
  --compiler /opt/cheerp2/bin/clang++ \
  --cheerp-prefix /opt/cheerp2 \
  tests

# Parallel jobs and optimization level
python3 run_lit_tests.py -j8 -O2 .

# Preexecute modes
python3 run_lit_tests.py --preexecute .
python3 run_lit_tests.py --preexecute-asmjs .

# Determinism checks (in addition to selected target modes)
python3 run_lit_tests.py --determinism 5 --determinism-probability 0.2 .
```

## Direct lit usage (advanced)

Direct `lit` invocation is supported, but is less forgiving and easier to misconfigure.
Use this when you need fine-grained lit control.

From `root/`:

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
```


## Key lit parameters used by this suite

- `TARGET` : comma-separated subset of `js,wasm,asmjs,preexecute,   preexecute-asmjs`
- `OPT_LEVEL` : e.g. `O0`, `O1`, `O2`, `O3`
- `CHEERP_FLAGS` : additional compiler flags (-)
- `EXTRA_FLAGS` : extra flags appended to compile commands
- `PRE_EX` : `j`/`a`/`1` for preexecute modes
- `COMPILER` : compiler alias/path (`cheerp`, `local`, or explicit path)
- `CHEERP_PREFIX` : toolchain prefix (e.g. `/opt/altcompiler`)

## Reports and outputs

When using `run_lit_tests.py`, these files are generated in `cheerp-test/`:
- `litTestReport.xml` (regular targets)
- `litTestReport_preexec.xml` (preexecute js mode)
- `litTestReport_preexec_asmjs.xml` (preexecute asmjs mode)
- `litTestReport.test` (merged summary report)

Temporary artifacts are placed in test output directories by lit; keep them when debugging failures.

## Protocol for adding new tests: