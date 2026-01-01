# This Makefile is only for convenience.
# The actual build is enabled solely by CMake.

ifeq (, $(shell command -v ninja))
	GENERATOR := "Unix Makefiles"
else
	GENERATOR := "Ninja"
endif

ifeq (, $(shell command -v clang))
	COMPILER := "cc"
else
	COMPILER := "clang"
endif

# Default build type and directory
BUILD_TYPE ?= "Release"
BUILD_DIR ?= build

# Default clang-format/-tidy binaries/wrappers
CLANG_FORMAT ?= clang-format-18
CLANG_TIDY ?= run-clang-tidy-18

# If CC is not specified, it will be populated by make to "cc".
# In that case, I want to use "clang" (if available) instead.
# But if it is user-specified, keep it as-is.
CC := $(or $(filter-out cc,$(CC)),$(COMPILER))
CXX := $(or $(filter-out cc,$(CC)),$(COMPILER))

# Find all CMake files (except those in build dir)
CMAKE_FILES = $(shell find . \
  -path ./$(BUILD_DIR) -prune -o \
  \( -name CMakeLists.txt -o -name "*.cmake" \) -print)

# Find all source files
SRC_FILES = $(shell find app/ lib/ bindings/ -name "*.c" -o -name "*.cpp" -o -name "*.cc")
HEADER_FILES = $(shell find app/ lib/ bindings/ -name "*.h" -o -name "*.hpp")

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
	@echo "Configuring project in $(BUILD_DIR) with $(GENERATOR) using CC=$(CC) CXX=$(CXX) and BUILD_TYPE=$(BUILD_TYPE)"
	@cmake -S . -B $(BUILD_DIR) -G $(GENERATOR) \
		-DCMAKE_C_COMPILER=$(CC) \
		-DCMAKE_CXX_COMPILER=$(CXX) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

.PHONY: install
install: $(BINS)
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
	@cmake --build $(BUILD_DIR) --target $(BINDINGS) --parallel

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
	@$(CLANG_FORMAT) --dry-run -Werror $(SRC_FILES) $(HEADER_FILES)
	@echo "SUCCESS: No formatting errors found."

.PHONY: format
format:
	@$(CLANG_FORMAT) -i $(SRC_FILES) $(HEADER_FILES)

.PHONY: lint
lint: ${CONFIGURED}
	@$(CLANG_TIDY) -quiet -p build/ -use-color 1 $(SRC_FILES)

.PHONY: fixes
fixes: ${CONFIGURED}
	@$(CLANG_TIDY) -quiet -p build/ -use-color 1 -fix $(SRC_FILES)
