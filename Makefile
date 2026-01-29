# This Makefile is only for convenience.
# The actual build is enabled solely by CMake.

ifeq (, $(shell command -v ninja))
	GENERATOR ?= Unix Makefiles
else
	GENERATOR ?= Ninja
endif

# if clang is available, use it as the default compiler
ifneq (, $(shell command -v clang))
	COMPILER ?= clang
else ifneq (, $(shell command -v gcc))
	COMPILER ?= gcc
else
	COMPILER ?=
endif

# If CC is not specified, it will be populated by make to "cc" (and CXX to "g++").
# Override that only if the a specific compiler is requested.
# To override the automatic selection, set CC and CXX directly when invoking make and leave COMPILER empty.
ifeq (clang, $(COMPILER))
	# compiler specified, overwrite CC and CXX
	CC := clang
	CXX := clang++
else ifeq (gcc, $(COMPILER))
	# compiler specified, overwrite CC and CXX
	CC := gcc
	CXX := g++
else ifneq (, $(COMPILER))
	# compiler specified, overwrite CC and CXX
	CC := $(COMPILER)
	CXX := $(COMPILER)
endif


# Default build type
BUILD_TYPE ?= Release

# Default build and source directories
BUILD_DIR ?= build
APP_DIR ?= app
LIB_DIR ?= lib
TEST_DIR ?= tests
BINDINGS_DIR ?= bindings

# Default clang-format/-tidy binaries/wrappers
CLANG_FORMAT ?= clang-format-18
CLANG_TIDY ?= run-clang-tidy-18

# Arguments for running executable
ARGS ?= --help


# Find all CMake files (except those in build dir)
CMAKE_FILES = $(shell find . \
  -path ./$(BUILD_DIR) -prune -o \
  \( -name CMakeLists.txt -o -name "*.cmake" \) -print)

# Find all source files
SRC_FILES = $(shell find $(APP_DIR) $(LIB_DIR) $(BINDINGS_DIR) -name "*.c" -o -name "*.cpp" -o -name "*.cc")
HEADER_FILES = $(shell find $(APP_DIR) $(LIB_DIR) $(BINDINGS_DIR) -name "*.h" -o -name "*.hpp")
TEST_SRC_FILES = $(shell find $(TEST_DIR) -name "*.c" -o -name "*.cpp" -o -name "*.cc")
TEST_HEADER_FILES = $(shell find $(TEST_DIR) -name "*.h" -o -name "*.hpp")

# Stamp file to indicate when configuration was done
CONFIGURED := $(BUILD_DIR)/CMakeFiles/cmake.check_cache

# Setup binary targets
EXE := bf2c
LIB := libbf2c_lib.a
BINDINGS := bf2c_python_bindings

.PHONY: build
build: $(CONFIGURED)
	@cmake --build $(BUILD_DIR) --parallel

$(CONFIGURED): $(CMAKE_FILES)
	@echo "Configuring project in $(BUILD_DIR) with '$(GENERATOR)' using CC=$(CC) CXX=$(CXX) and BUILD_TYPE=$(BUILD_TYPE)"
	@cmake -S . -B $(BUILD_DIR) -G "$(GENERATOR)" \
		-DCMAKE_C_COMPILER=$(CC) \
		-DCMAKE_CXX_COMPILER=$(CXX) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

.PHONY: install
install: all
	@cmake --install $(BUILD_DIR)

# Various (sub)sets of build targets
.PHONY: all
all: build bindings

.PHONY: app
app: $(CONFIGURED)
	@cmake --build build/ --target $(EXE) --parallel

.PHONY: lib
lib: $(CONFIGURED)
	@cmake --build build/ --target $(LIB) --parallel

.PHONY: bindings
bindings: $(CONFIGURED)
	@if [ -n "$(BINDINGS)" ]; then \
		cmake --build $(BUILD_DIR) --target $(BINDINGS) --parallel; \
	fi

.PHONY: run
run: app
	./$(BUILD_DIR)/$(APP_DIR)/$(EXE) $(ARGS)

# Configuration specific targets
.PHONY: configure
configure:
	@BUILD_TYPE=$(BUILD_TYPE) $(MAKE) --no-print-directory -B $(CONFIGURED)

.PHONY: debug
debug: enable-debug configure app lib

.PHONY: enable-debug
enable-debug:
	$(eval BUILD_TYPE := "Debug")

.PHONY: enable-release
enable-release:
	$(eval BUILD_TYPE := "Release")

.PHONY: release
release: enable-release configure all

# Clean up
.PHONY: clean
clean:
	@if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi
	@if [ -d bin/ ]; then rm -rf bin; fi
	@if [ -f a.out ]; then rm a.out; fi

.PHONY: clean-build
clean-build:
	@cmake --build $(BUILD_DIR) --target clean

# Formatting and linting
.PHONY: check-format
check-format:
	@$(CLANG_FORMAT) --dry-run -Werror $(SRC_FILES) $(HEADER_FILES) $(TEST_SRC_FILES) $(TEST_HEADER_FILES)
	@echo "SUCCESS: No formatting errors found."

.PHONY: format
format:
	@$(CLANG_FORMAT) -i $(SRC_FILES) $(HEADER_FILES) $(TEST_SRC_FILES) $(TEST_HEADER_FILES)

.PHONY: lint
lint: ${CONFIGURED}
	@$(CLANG_TIDY) -quiet -p build/ -use-color 1 $(SRC_FILES)

.PHONY: fixes
fixes: ${CONFIGURED}
	@$(CLANG_TIDY) -quiet -p build/ -use-color 1 -fix $(SRC_FILES)

.PHONY: test
test: $(CONFIGURED)
	@echo "Test target not implemented yet."

.PHONY: help
help:
	@echo "Convenience Makefile for C/C++ Project wrapping CMake"
	@echo ""
	@echo "Convenience targets:"
	@echo "  build          - Configure and build the project (default target)"
	@echo "  install        - Install the built targets"
	@echo "  all            - Build all targets (app, lib, bindings, etc.)"
	@echo "  app            - Build the application"
	@echo "  lib            - Build the library"
	@echo "  bindings       - Build the language bindings (if any)"
	@echo "  run            - Run the built application with ARGS"
	@echo "  configure      - Configure the project with CMake"
	@echo "  debug          - Configure and build in Debug mode"
	@echo "  release        - Configure and build in Release mode"
	@echo "  clean          - Clean the build directory and binaries"
	@echo "  clean-build    - Clean the build artifacts"
	@echo "  check-format   - Check code formatting with clang-format"
	@echo "  format         - Format code with clang-format"
	@echo "  lint           - Run clang-tidy for linting"
	@echo "  fixes          - Apply fixes suggested by clang-tidy"
	@echo "  test           - Run tests"
	@echo "  help           - Show this help message"
	@echo ""
	@echo "Variables that can be set when invoking make:"
	@echo "  GENERATOR      - CMake generator to use (Ninja, Unix Makefiles, etc.)"
	@echo "  BUILD_TYPE     - Build type (Debug, Release, etc.), default is 'Release'"
	@echo "  BUILD_DIR      - Build directory, default is 'build'"
	@echo "  APP_DIR        - Application source directory, default is 'app'"
	@echo "  LIB_DIR        - Library source directory, default is 'lib'"
	@echo "  TEST_DIR       - Test source directory, default is 'tests'"
	@echo "  BINDINGS_DIR   - Language bindings source directory"
	@echo "  ARGS           - Arguments to pass when running the application"
	@echo "  COMPILER       - Compiler to use (clang, gcc, etc.), overrides CC and CXX if set, default is automatic selection"
	@echo "  CC             - C compiler to use (overridden by COMPILER if set)"
	@echo "  CXX            - C++ compiler to use (overridden by COMPILER if set)"
	@echo "  CLANG_FORMAT   - clang-format binary or wrapper, default is clang-format-18"
	@echo "  CLANG_TIDY     - clang-tidy binary or wrapper, default is run-clang-tidy-18"


