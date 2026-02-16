# Cheerp Test Suite - stable/
# Comprehensive Makefile for running tests with various configurations

# Tools
CHEERP_CLANG = /opt/cheerp/bin/clang++
LIT ?= $(shell which lit 2>/dev/null || echo lit)
NODE ?= $(shell which node 2>/dev/null || echo node)
PYTHON ?= python3

# Configuration
JOBS ?= $(shell nproc 2>/dev/null || echo 4)
TARGET ?= js
PREEXEC ?= 0
DETERMINISM ?= 0
DETERMINISM_RUNS ?= 3
VALGRIND ?= 0

# Flags
CHEERP_FLAGS ?= -O1
EXTRA_FLAGS ?=
export CHEERP_FLAGS
export EXTRA_FLAGS

# Directories
TEST_ROOT = .
DETERMINISM_WRAPPER = $(TEST_ROOT)/determinism_wrapper.py

# Test discovery
ALL_TESTS := $(shell find $(TEST_ROOT) -maxdepth 1 -name "*.cpp" -type f 2>/dev/null)
SUBDIRS := $(shell find $(TEST_ROOT) -maxdepth 1 -type d ! -path $(TEST_ROOT) ! -name Output ! -name determinism_failures 2>/dev/null)

# LIT parameters builder
define LIT_PARAMS
--param TARGET=$(TARGET) --param PRE-EX=$(PREEXEC)
endef

# Color output
COLOR_RESET = \033[0m
COLOR_GREEN = \033[0;32m
COLOR_YELLOW = \033[0;33m
COLOR_BLUE = \033[0;34m
COLOR_RED = \033[0;31m

.PHONY: all help clean test single dir determinism list

# Default target
all: help

# Help target
help:
	@echo "$(COLOR_BLUE)Cheerp Test Suite - stable/$(COLOR_RESET)"
	@echo "========================================="
	@echo ""
	@echo "$(COLOR_GREEN)Main Targets:$(COLOR_RESET)"
	@echo "  help              - Show this help message"
	@echo "  test              - Run all tests in current directory"
	@echo "  single FILE=path  - Run a single test file"
	@echo "  dir DIR=name      - Run all tests in a directory"
	@echo "  determinism       - Run determinism tests on specified files/dirs"
	@echo "  list              - List all test files and directories"
	@echo "  clean             - Clean generated output files"
	@echo ""
	@echo "$(COLOR_GREEN)Configuration Variables:$(COLOR_RESET)"
	@echo "  TARGET            - Compilation target: js|wasm|asmjs (default: $(TARGET))"
	@echo "  PREEXEC           - Preexecution mode: 0|1 (default: $(PREEXEC))"
	@echo "  DETERMINISM       - Determinism level: 0|1|2 (default: $(DETERMINISM))"
	@echo "  DETERMINISM_RUNS  - Number of determinism runs (default: $(DETERMINISM_RUNS))"
	@echo "  JOBS              - Parallel jobs (default: $(JOBS))"
	@echo "  VALGRIND          - Run with Valgrind: 0|1 (default: $(VALGRIND))"
	@echo "  FILE              - Specific test file to run"
	@echo "  DIR               - Specific directory to run"
	@echo "  CHEERP_FLAGS      - Compiler flags (default: $(CHEERP_FLAGS))"
	@echo "  EXTRA_FLAGS       - Extra compiler flags"
	@echo ""
	@echo "$(COLOR_YELLOW)Examples:$(COLOR_RESET)"
	@echo "  # Run single test"
	@echo "  make single FILE=test_print_case.cpp"
	@echo "  make single FILE=test_print_case.cpp TARGET=wasm"
	@echo "  make single FILE=test_print_case.cpp PREEXEC=1"
	@echo ""
	@echo "  # Run directory tests"
	@echo "  make dir DIR=downcast"
	@echo "  make dir DIR=downcast TARGET=js,wasm"
	@echo "  make dir DIR=downcast PREEXEC=1"
	@echo ""
	@echo "  # Run all tests"
	@echo "  make test"
	@echo "  make test TARGET=wasm JOBS=8"
	@echo ""
	@echo "  # Determinism testing"
	@echo "  make determinism FILE=test_print_case.cpp"
	@echo "  make determinism FILE=test_print_case.cpp DETERMINISM_RUNS=5"
	@echo "  make determinism DIR=downcast"
	@echo ""
	@echo "  # Multiple targets"
	@echo "  make single FILE=test_print_case.cpp TARGET=js,wasm,asmjs"
	@echo ""
	@echo "$(COLOR_GREEN)Available Subdirectories:$(COLOR_RESET)"
	@for dir in $(notdir $(SUBDIRS)); do \
		count=$$(find $(TEST_ROOT)/$$dir -name "*.cpp" -type f 2>/dev/null | wc -l); \
		echo "  $$dir ($$count tests)"; \
	done
	@echo ""
	@echo "$(COLOR_GREEN)Test Files in Root:$(COLOR_RESET)"
	@echo "  $(words $(ALL_TESTS)) test files"

# List all tests
list:
	@echo "$(COLOR_BLUE)Test files in $(TEST_ROOT):$(COLOR_RESET)"
	@for f in $(ALL_TESTS); do \
		echo "  $$(basename $$f)"; \
	done
	@echo ""
	@echo "$(COLOR_BLUE)Test directories:$(COLOR_RESET)"
	@for dir in $(notdir $(SUBDIRS)); do \
		count=$$(find $(TEST_ROOT)/$$dir -name "*.cpp" -type f 2>/dev/null | wc -l); \
		echo "  $$dir/ ($$count files)"; \
		find $(TEST_ROOT)/$$dir -name "*.cpp" -type f 2>/dev/null | sed 's|^|    |'; \
	done

# Run all tests in current directory (root level only)
test:
	@echo "$(COLOR_GREEN)Running all tests in $(TEST_ROOT)$(COLOR_RESET)"
	@echo "Configuration: TARGET=$(TARGET) PREEXEC=$(PREEXEC) JOBS=$(JOBS)"
	@echo "---------------------------------------------------"
	@if [ -z "$(ALL_TESTS)" ]; then \
		echo "$(COLOR_RED)No test files found$(COLOR_RESET)"; \
		exit 1; \
	fi
	@$(LIT) -v -j$(JOBS) $(LIT_PARAMS) $(ALL_TESTS)

# Run a single test file
single:
ifndef FILE
	@echo "$(COLOR_RED)Error: FILE not specified$(COLOR_RESET)"
	@echo "Usage: make single FILE=<test_file.cpp>"
	@echo "Example: make single FILE=test_print_case.cpp"
	@exit 1
endif
	@if [ ! -f "$(FILE)" ]; then \
		echo "$(COLOR_RED)Error: File '$(FILE)' not found$(COLOR_RESET)"; \
		exit 1; \
	fi
	@echo "$(COLOR_GREEN)Running single test: $(FILE)$(COLOR_RESET)"
	@echo "Configuration: TARGET=$(TARGET) PREEXEC=$(PREEXEC)"
	@echo "---------------------------------------------------"
	@$(LIT) -vv $(LIT_PARAMS) $(FILE)

# Run all tests in a directory
dir:
ifndef DIR
	@echo "$(COLOR_RED)Error: DIR not specified$(COLOR_RESET)"
	@echo "Usage: make dir DIR=<directory_name>"
	@echo "Example: make dir DIR=downcast"
	@echo ""
	@echo "Available directories:"
	@for d in $(notdir $(SUBDIRS)); do echo "  $$d"; done
	@exit 1
endif
	@if [ ! -d "$(DIR)" ]; then \
		echo "$(COLOR_RED)Error: Directory '$(DIR)' not found$(COLOR_RESET)"; \
		exit 1; \
	fi
	@test_count=$$(find $(DIR) -name "*.cpp" -type f 2>/dev/null | wc -l); \
	if [ "$$test_count" -eq 0 ]; then \
		echo "$(COLOR_RED)No test files found in $(DIR)/$(COLOR_RESET)"; \
		exit 1; \
	fi
	@echo "$(COLOR_GREEN)Running tests in $(DIR)/ ($$test_count files)$(COLOR_RESET)"
	@echo "Configuration: TARGET=$(TARGET) PREEXEC=$(PREEXEC) JOBS=$(JOBS)"
	@echo "---------------------------------------------------"
	@$(LIT) -v -j$(JOBS) $(LIT_PARAMS) $(DIR)

# Determinism testing - single file
determinism-single:
ifndef FILE
	@echo "$(COLOR_RED)Error: FILE not specified$(COLOR_RESET)"
	@echo "Usage: make determinism FILE=<test_file.cpp>"
	@exit 1
endif
	@if [ ! -f "$(FILE)" ]; then \
		echo "$(COLOR_RED)Error: File '$(FILE)' not found$(COLOR_RESET)"; \
		exit 1; \
	fi
	@if [ ! -f "$(DETERMINISM_WRAPPER)" ]; then \
		echo "$(COLOR_RED)Error: Determinism wrapper not found at $(DETERMINISM_WRAPPER)$(COLOR_RESET)"; \
		exit 1; \
	fi
	@echo "$(COLOR_GREEN)Running determinism test on: $(FILE)$(COLOR_RESET)"
	@echo "Configuration: TARGET=$(TARGET) RUNS=$(DETERMINISM_RUNS) PREEXEC=$(PREEXEC)"
	@echo "---------------------------------------------------"
	@$(PYTHON) $(DETERMINISM_WRAPPER) --level 1 --runs $(DETERMINISM_RUNS) \
		--target $(TARGET) $(if $(filter 1,$(PREEXEC)),--preexecute,) -v $(FILE)

# Determinism testing - directory
determinism-dir:
ifndef DIR
	@echo "$(COLOR_RED)Error: DIR not specified$(COLOR_RESET)"
	@echo "Usage: make determinism DIR=<directory_name>"
	@exit 1
endif
	@if [ ! -d "$(DIR)" ]; then \
		echo "$(COLOR_RED)Error: Directory '$(DIR)' not found$(COLOR_RESET)"; \
		exit 1; \
	fi
	@if [ ! -f "$(DETERMINISM_WRAPPER)" ]; then \
		echo "$(COLOR_RED)Error: Determinism wrapper not found at $(DETERMINISM_WRAPPER)$(COLOR_RESET)"; \
		exit 1; \
	fi
	@echo "$(COLOR_GREEN)Running determinism tests in $(DIR)/$(COLOR_RESET)"
	@echo "Configuration: TARGET=$(TARGET) RUNS=$(DETERMINISM_RUNS) PREEXEC=$(PREEXEC)"
	@echo "---------------------------------------------------"
	@$(PYTHON) $(DETERMINISM_WRAPPER) --level 1 --runs $(DETERMINISM_RUNS) \
		--target $(TARGET) $(if $(filter 1,$(PREEXEC)),--preexecute,) -v --suite $(DIR)

# Determinism dispatcher
determinism:
ifdef FILE
	@$(MAKE) determinism-single FILE=$(FILE) TARGET=$(TARGET) DETERMINISM_RUNS=$(DETERMINISM_RUNS) PREEXEC=$(PREEXEC)
else ifdef DIR
	@$(MAKE) determinism-dir DIR=$(DIR) TARGET=$(TARGET) DETERMINISM_RUNS=$(DETERMINISM_RUNS) PREEXEC=$(PREEXEC)
else
	@echo "$(COLOR_RED)Error: Must specify FILE or DIR$(COLOR_RESET)"
	@echo "Usage:"
	@echo "  make determinism FILE=<test_file.cpp>"
	@echo "  make determinism DIR=<directory_name>"
	@exit 1
endif

# Clean generated files
clean:
	@echo "$(COLOR_YELLOW)Cleaning generated files...$(COLOR_RESET)"
	@rm -rf Output
	@rm -rf */Output
	@rm -rf determinism_failures
	@rm -f *.log
	@find . -name "*.script" -delete 2>/dev/null || true
	@echo "$(COLOR_GREEN)Clean complete$(COLOR_RESET)"

# Convenience targets for common operations
.PHONY: test-js test-wasm test-asmjs test-all-targets
.PHONY: preexec-js preexec-wasm preexec-asmjs

test-js:
	@$(MAKE) test TARGET=js

test-wasm:
	@$(MAKE) test TARGET=wasm

test-asmjs:
	@$(MAKE) test TARGET=asmjs

test-all-targets:
	@$(MAKE) test TARGET=js,wasm,asmjs

preexec-js:
	@$(MAKE) test TARGET=js PREEXEC=1

preexec-wasm:
	@$(MAKE) test TARGET=wasm PREEXEC=1

preexec-asmjs:
	@$(MAKE) test TARGET=asmjs PREEXEC=1
