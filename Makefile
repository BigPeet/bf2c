# This Makefile is only for convenience.
# The actual build is enabled solely by CMake.

ifeq (, $(shell command -v ninja))
	GENERATOR := "Unix Makefiles"
else
	GENERATOR := "Ninja"
endif

# If CC is not specified, it will be populated by make to "cc".
# In that case, I want to use "clang" instead.
# But if it is user-specified, keep it as-is.
CC := $(or $(filter-out cc,$(CC)),clang)
CXX := $(or $(filter-out cc,$(CC)),clang)

.PHONY: clang-configure gcc-configure clean build all check-format format lint

BINS := build/app/bf2c

all: $(BINS)

configure:
	@cmake -S . -B build/ -G $(GENERATOR) -DCMAKE_C_COMPILER=$(CC) -DCMAKE_CXX_COMPILER=$(CXX)

# technically, CMakeCache.txt or build.ninja would be a better indicator...
build/compile_commands.json: configure

build: build/compile_commands.json
	@cmake --build build/

$(BINS): build

clean:
	@if [ -d build/ ]; then rm -rf build; fi
	@if [ -d bin/ ]; then rm -rf bin; fi

check-format:
	@find lib/ app/ -name "*.c" -or -name "*.h" -or -name "*.cpp" | xargs clang-format --dry-run -Werror
	@echo "SUCCESS: No formatting errors found."

format:
	@find lib/ app/ -name "*.c" -or -name "*.h" -or -name "*.cpp" | xargs clang-format -i

lint: build/compile_commands.json
	@find lib/ app/ -name "*.c" -or -name "*.cpp" | xargs run-clang-tidy -quiet -p build/ -use-color 1
