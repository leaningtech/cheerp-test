CHEERP_CLANG = /opt/cheerp/bin/clang++
CHEERP_FLAGS ?= -O1
EXTRA_FLAGS ?= 
export CHEERP_FLAGS
export EXTRA_FLAGS

UNIT_TEST_SOURCES := $(shell find unit -name "*.cpp" -type f 2>/dev/null | wc -l)
EXPERIMENTAL_SOURCES := $(wildcard experimental/*.cpp)
JOBS ?= $(shell nproc 2>/dev/null || echo 4)
LIT ?= $(shell which lit 2>/dev/null || echo /home/alex/.local/bin/lit)
NODE ?= $(shell which node 2>/dev/null || echo node)

# List of unit test subdirectories
UNIT_DIRS = bitfield cli client closures codegen dom downcast exceptions ffi globals jsexport memory randomcfg static std threading types vbases virtual

# Generate phony targets for unit subdirectories
UNIT_TARGETS = $(addprefix lit-,$(UNIT_DIRS))

.PHONY: all test clean lit-unit lit-unit-parallel lit-unit-seq lit-smoke lit-preexecutable lit-common lit-genericjs lit-asmjs lit-wasm-only lit-experimental $(UNIT_TARGETS)

# Default target - run smoke tests (quick sanity check)
all: lit-smoke

# Alias for default
test: all

# Run experimental tests (formerly test_sources/)
lit-experimental:
	@if [ -d "experimental" ] && [ -n "$(EXPERIMENTAL_SOURCES)" ]; then \
		echo "Running LLVM lit tests on experimental/ ($(words $(EXPERIMENTAL_SOURCES)) files)..."; \
		$(LIT) -v -a -j$(JOBS) experimental 2>&1 | tee lit-experimental-output.log; \
	else \
		echo "No experimental tests found in experimental/"; \
	fi

# Clean all generated files
clean:
	rm -f lit-unit-output.log lit-smoke-output.log lit-experimental-output.log
	rm -f lit-preexecutable-output.log lit-common-output.log lit-genericjs-output.log
	rm -f lit-asmjs-output.log lit-wasm-only-output.log
	rm -f $(foreach dir,$(UNIT_DIRS),lit-$(dir)-output.log)
	rm -rf experimental/Output unit/*/Output out/
	@echo "Cleaned up generated files."

# Run unit tests (parallel by default for speed)
# WARNING: Most unit tests are not yet working - use lit-smoke or lit-memory instead
lit-unit: lit-unit-parallel

# Run unit tests sequentially
lit-unit-seq:
	@echo "Running LLVM lit tests (sequential) on all .cpp files in unit/..."
	@echo "Found $(UNIT_TEST_SOURCES) unit test files"
	@echo "Using lit: $(LIT)"
	@echo "---------------------------------------------------"
	@$(LIT) -v -a -j1 unit 2>&1 | tee lit-unit-output.log
	@echo "---------------------------------------------------"
	@echo "Test results saved to lit-unit-output.log"

# Run unit tests in parallel
lit-unit-parallel:
	@echo "Running LLVM lit tests (parallel with $(JOBS) jobs) on unit/"
	@echo "Found $(UNIT_TEST_SOURCES) unit test files"
	@echo "Using lit: $(LIT)"
	@echo "---------------------------------------------------"
	@$(LIT) -v -a -j$(JOBS) unit 2>&1 | tee lit-unit-output.log
	@echo "---------------------------------------------------"
	@echo "Test results saved to lit-unit-output.log"

# Specific targets for unit subdirectories
lit-bitfield:
	@echo "Running LLVM lit tests on unit/bitfield/..."
	@$(LIT) -v -a -j$(JOBS) unit/bitfield 2>&1 | tee lit-bitfield-output.log

lit-cli:
	@echo "Running LLVM lit tests on unit/cli/..."
	@$(LIT) -v -a -j$(JOBS) unit/cli 2>&1 | tee lit-cli-output.log

lit-client:
	@echo "Running LLVM lit tests on unit/client/..."
	@$(LIT) -v -a -j$(JOBS) unit/client 2>&1 | tee lit-client-output.log

lit-closures:
	@echo "Running LLVM lit tests on unit/closures/..."
	@$(LIT) -v -a -j$(JOBS) unit/closures 2>&1 | tee lit-closures-output.log

lit-codegen:
	@echo "Running LLVM lit tests on unit/codegen/..."
	@$(LIT) -v -a -j$(JOBS) unit/codegen 2>&1 | tee lit-codegen-output.log

lit-dom:
	@echo "Running LLVM lit tests on unit/dom/..."
	@$(LIT) -v -a -j$(JOBS) unit/dom 2>&1 | tee lit-dom-output.log

lit-downcast:
	@echo "Running LLVM lit tests on unit/downcast/..."
	@$(LIT) -v -a -j$(JOBS) unit/downcast 2>&1 | tee lit-downcast-output.log

lit-exceptions:
	@echo "Running LLVM lit tests on unit/exceptions/..."
	@$(LIT) -v -a -j$(JOBS) unit/exceptions 2>&1 | tee lit-exceptions-output.log

lit-ffi:
	@echo "Running LLVM lit tests on unit/ffi/..."
	@$(LIT) -v -a -j$(JOBS) unit/ffi 2>&1 | tee lit-ffi-output.log

lit-globals:
	@echo "Running LLVM lit tests on unit/globals/..."
	@$(LIT) -v -a -j$(JOBS) unit/globals 2>&1 | tee lit-globals-output.log

lit-jsexport:
	@echo "Running LLVM lit tests on unit/jsexport/..."
	@$(LIT) -v -a -j$(JOBS) unit/jsexport 2>&1 | tee lit-jsexport-output.log

lit-memory:
	@echo "Running LLVM lit tests on unit/memory/..."
	@$(LIT) -v -a -j$(JOBS) unit/memory 2>&1 | tee lit-memory-output.log

lit-randomcfg:
	@echo "Running LLVM lit tests on unit/randomcfg/..."
	@$(LIT) -v -a -j$(JOBS) unit/randomcfg 2>&1 | tee lit-randomcfg-output.log

lit-static:
	@echo "Running LLVM lit tests on unit/static/..."
	@$(LIT) -v -a -j$(JOBS) unit/static 2>&1 | tee lit-static-output.log

lit-std:
	@echo "Running LLVM lit tests on unit/std/..."
	@$(LIT) -v -a -j$(JOBS) unit/std 2>&1 | tee lit-std-output.log

lit-threading:
	@echo "Running LLVM lit tests on unit/threading/..."
	@$(LIT) -v -a -j$(JOBS) unit/threading 2>&1 | tee lit-threading-output.log

lit-types:
	@echo "Running LLVM lit tests on unit/types/..."
	@$(LIT) -v -a -j$(JOBS) unit/types 2>&1 | tee lit-types-output.log

lit-vbases:
	@echo "Running LLVM lit tests on unit/vbases/..."
	@$(LIT) -v -a -j$(JOBS) unit/vbases 2>&1 | tee lit-vbases-output.log

lit-virtual:
	@echo "Running LLVM lit tests on unit/virtual/..."
	@$(LIT) -v -a -j$(JOBS) unit/virtual 2>&1 | tee lit-virtual-output.log

# Pre-executable tests (tests all 3 targets: cheerp, cheerp-wasm+asmjs, cheerp-wasm)
lit-preexecutable:
	@echo "Running pre-executable tests (compile to all 3 targets)..."
	@test_files="unit/downcast/test1.cpp unit/virtual/test1.cpp unit/virtual/test2.cpp unit/virtual/test3.cpp \
		unit/virtual/rtti.cpp unit/virtual/constructor.cpp \
		unit/vbases/test1.cpp unit/vbases/test2.cpp unit/vbases/test3.cpp unit/vbases/test4.cpp unit/vbases/test5.cpp \
		unit/memory/test1.cpp unit/memory/test2.cpp unit/memory/test3.cpp unit/memory/test4.cpp unit/memory/test5.cpp \
		unit/memory/test6.cpp unit/memory/test7.cpp unit/memory/test8.cpp unit/memory/arraynew.cpp unit/memory/multiarray.cpp \
		unit/std/test1.cpp unit/std/test2.cpp unit/std/test3.cpp unit/std/test4.cpp unit/std/test5.cpp \
		unit/std/test6.cpp unit/std/test7.cpp unit/std/test8.cpp unit/std/test9.cpp unit/std/stringassign.cpp \
		unit/std/sscanf.cpp unit/std/locale.cpp unit/std/stdmemfuncs.cpp unit/std/sort.cpp unit/std/mapdestruction.cpp unit/std/memcmp.cpp \
		unit/bitfield/test1.cpp unit/bitfield/test2.cpp unit/bitfield/test3.cpp unit/bitfield/test4.cpp unit/bitfield/test5.cpp \
		unit/types/test1.cpp unit/types/test2.cpp unit/types/test4.cpp unit/types/test5.cpp unit/types/test6.cpp \
		unit/types/test8.cpp unit/types/test9.cpp unit/types/test10.cpp unit/types/test11.cpp unit/types/test12.cpp \
		unit/types/test13.cpp unit/types/test14.cpp unit/types/test16.cpp unit/types/cinheritance.cpp \
		unit/types/memberfunctions.cpp unit/types/memberfunctions2.cpp unit/types/union64.cpp \
		unit/globals/test1.cpp unit/globals/test2.cpp unit/globals/test3.cpp unit/globals/test4.cpp unit/globals/test5.cpp \
		unit/globals/test6.cpp unit/globals/test7.cpp unit/globals/betterconst.cpp unit/globals/globaluint16.cpp \
		unit/codegen/test1.cpp unit/codegen/test3.cpp unit/codegen/test4.cpp unit/codegen/test5.cpp unit/codegen/test6.cpp \
		unit/codegen/test7.cpp unit/codegen/test8.cpp unit/codegen/test9.cpp unit/codegen/test10.cpp unit/codegen/test11.cpp \
		unit/codegen/test13.cpp unit/codegen/test14.cpp unit/codegen/test15.cpp unit/codegen/test16.cpp unit/codegen/test17.cpp \
		unit/codegen/test18.cpp unit/codegen/test19.cpp unit/codegen/test22.cpp unit/codegen/test23.cpp unit/codegen/test24.cpp \
		unit/codegen/bswap.cpp unit/codegen/64bitenum.cpp unit/codegen/64bitpointerarith.cpp unit/codegen/floattoint.cpp \
		unit/codegen/variadic.cpp unit/codegen/dynalloca.cpp unit/static/test1.cpp unit/codegen/dynstack.cpp \
		unit/codegen/byval.cpp unit/codegen/fptoi.cpp unit/codegen/64bitrewrite.cpp unit/codegen/reverse_iter.cpp \
		unit/codegen/structbase.cpp unit/randomcfg/size5times20.cpp unit/randomcfg/size10times10.cpp \
		unit/randomcfg/size20times10.cpp unit/randomcfg/size50times2.cpp unit/randomcfg/swap5by5.cpp \
		unit/randomcfg/swap10by10.cpp unit/randomcfg/swap25by25.cpp unit/randomcfg/swapOnPointers.cpp \
		unit/randomcfg/combOnDouble.cpp unit/randomcfg/comb10by10.cpp"; \
	$(LIT) -v -a -j$(JOBS) $$test_files 2>&1 | tee lit-preexecutable-output.log

# Common tests (compile to all 3 targets, shared between genericjs and wasm)
lit-common:
	@echo "Running common tests (compile to all 3 targets)..."
	@test_files="unit/std/gettimeofday.cpp unit/std/chrono.cpp unit/ffi/test3.cpp unit/closures/test4.cpp \
		unit/jsexport/cheerp_pimpl.cpp unit/jsexport/factory.cpp unit/jsexport/invoke_functions.cpp \
		unit/jsexport/namespaces.cpp unit/jsexport/parameters_builtin.cpp unit/jsexport/unsafe_gnu.c \
		unit/jsexport/unsafe_clang.cpp unit/client/class.cpp unit/client/nested-namespaces.cpp \
		unit/client/setter-getter.cpp unit/client/static-methods.cpp unit/client/typeof.cpp \
		unit/client/instanceof.cpp unit/codegen/nested_lambda.cpp unit/types/padding1.cpp \
		unit/types/padding2.cpp unit/types/padding3.cpp unit/globals/initializers.cpp"; \
	$(LIT) -v -a -j$(JOBS) $$test_files 2>&1 | tee lit-common-output.log

# GenericJS only tests (pure JavaScript target only)
lit-genericjs:
	@echo "Running GenericJS-only tests (JavaScript target only)..."
	@test_files="unit/dom/test1.cpp unit/dom/test2.cpp unit/dom/test3.cpp unit/dom/test4.cpp unit/dom/test5.cpp \
		unit/dom/test6.cpp unit/dom/test7.cpp unit/dom/test8.cpp unit/dom/test9.cpp unit/dom/noconstructor.cpp \
		unit/dom/utf8.cpp unit/dom/objapi.cpp unit/dom/max.cpp unit/memory/typedarrays_operator.cpp \
		unit/memory/kinds.cpp unit/std/stringhashing.cpp unit/types/test7.cpp unit/codegen/test20.cpp \
		unit/codegen/escapes.cpp unit/codegen/advancedasm.cpp unit/std/tostring.cpp unit/codegen/test2.cpp \
		unit/codegen/test12.cpp unit/closures/test1.cpp unit/closures/test2.cpp unit/closures/test3.cpp \
		unit/jsexport/parameters_client.cpp"; \
	$(LIT) -v -a -j$(JOBS) $$test_files 2>&1 | tee lit-genericjs-output.log

# AsmJS only tests (WebAssembly with asm.js backend)
lit-asmjs:
	@echo "Running AsmJS-only tests (cheerp-wasm with asmjs backend)..."
	@test_files="unit/ffi/test1.cpp unit/ffi/test2.cpp unit/std/malloc.cpp"; \
	$(LIT) -v -a -j$(JOBS) $$test_files 2>&1 | tee lit-asmjs-output.log

# WebAssembly only tests (pure wasm target)
lit-wasm-only:
	@echo "Running WebAssembly-only tests (pure wasm target)..."
	@test_files="unit/types/packed.cpp unit/types/padding4.cpp unit/types/padding5.cpp unit/types/padding6.cpp"; \
	$(LIT) -v -a -j$(JOBS) $$test_files 2>&1 | tee lit-wasm-only-output.log

# Smoke test - run one test per unit subdirectory
lit-smoke:
	@echo "Running smoke tests (one test per unit subdirectory)..."
	@echo "Using lit: $(LIT)"
	@echo "---------------------------------------------------"
	@test_files=""; \
	for dir in $(UNIT_DIRS); do \
		if [ -f "unit/$$dir/test1.cpp" ]; then \
			test_files="$$test_files unit/$$dir/test1.cpp"; \
			echo "  $$dir: test1.cpp"; \
		else \
			alt_test=$$(find "unit/$$dir" -maxdepth 1 -type f -name "*.cpp" 2>/dev/null | sort | head -1); \
			if [ -n "$$alt_test" ]; then \
				test_files="$$test_files $$alt_test"; \
				echo "  $$dir: $$(basename $$alt_test)"; \
			else \
				echo "  $$dir: [no tests found]"; \
			fi; \
		fi; \
	done; \
	echo "---------------------------------------------------"; \
	if [ -n "$$test_files" ]; then \
		$(LIT) -v -a -j$(JOBS) $$test_files 2>&1 | tee lit-smoke-output.log; \
	else \
		echo "Error: No test files found"; \
		exit 1; \
	fi
	
help:
	@echo "Cheerp Test Suite"
	@echo "================="
	@echo ""
	@echo "NOTE: Currently only smoke, memory, and experimental tests are fully working."
	@echo "      Other tests are available but may have issues."
	@echo ""
	@echo "Main Targets:"
	@echo "  all               - Run smoke tests (default)"
	@echo "  test              - Alias for 'all'"
	@echo "  quick             - Run smoke + experimental tests"
	@echo "  clean             - Remove all generated files and logs"
	@echo ""
	@echo "Working Test Targets:"
	@echo "  lit-smoke         - Run one test per subdirectory (quick sanity check) ✓"
	@echo "  lit-memory        - Run memory management tests ✓"
	@echo "  lit-experimental  - Run experimental tests ✓"
	@echo ""
	@echo "Unit Test Targets (experimental - may have issues):"
	@echo "  lit-unit          - Run all unit tests in parallel"
	@echo "  lit-unit-seq      - Run all unit tests sequentially"
	@echo "  lit-unit-parallel - Run all unit tests in parallel (JOBS=$(JOBS))"
	@echo ""
	@echo "Test Category Targets (experimental - may have issues):"
	@echo "  lit-preexecutable - Tests compiled to all 3 targets"
	@echo "  lit-common        - Common tests shared between genericjs and wasm"
	@echo "  lit-genericjs     - GenericJS-only tests (pure JavaScript)"
	@echo "  lit-asmjs         - AsmJS-only tests (cheerp-wasm with asmjs backend)"
	@echo "  lit-wasm-only     - WebAssembly-only tests (pure wasm)"
	@echo ""
	@echo "Unit Subdirectory Targets (experimental - may have issues):"
	@echo "  $(foreach dir,$(UNIT_DIRS),lit-$(dir) )"
	@echo "  Note: Only lit-memory is known to work currently"
	@echo ""
	@echo "Variables:"
	@echo "  JOBS=N              - Set number of parallel jobs (current: $(JOBS))"
	@echo "  LIT=<path>          - Set lit executable path (current: $(LIT))"
	@echo "  NODE=<path>         - Set node executable path (current: $(NODE))"
	@echo "  CHEERP_FLAGS='...'  - Add compiler flags to all tests (current: $(CHEERP_FLAGS))"
	@echo "  EXTRA_FLAGS='...'   - Add extra compiler flags to all tests"
	@echo ""
	@echo "Test Statistics:"
	@echo "  unit/             - $(UNIT_TEST_SOURCES) test files"
	@echo "  experimental/     - $(words $(EXPERIMENTAL_SOURCES)) test files"
	@echo ""
	@echo "Examples:"
	@echo "  make                                    - Run smoke tests (default)"
	@echo "  make quick                              - Run smoke + experimental tests"
	@echo "  make lit-memory                         - Run memory tests"
	@echo "  make lit-experimental                   - Run experimental tests"
	@echo "  make JOBS=8                             - Run with 8 parallel jobs"
	@echo "  make lit-memory CHEERP_FLAGS='-g -O0'   - Run with debug flags"
	@echo "  make EXTRA_FLAGS='-DDEBUG'              - Run with extra preprocessor flags"
	@echo "  make CHEERP_FLAGS='-cheerp-bounds-check' - Add bounds checking"
