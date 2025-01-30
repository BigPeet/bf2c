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

BUILD_TYPE := "Release"

# If CC is not specified, it will be populated by make to "cc".
# In that case, I want to use "clang" (if available) instead.
# But if it is user-specified, keep it as-is.
CC := $(or $(filter-out cc,$(CC)),$(COMPILER))
CXX := $(or $(filter-out cc,$(CC)),$(COMPILER))

.PHONY: all enable-debug debug release configure build install clean check-format format lint

BINS := build/app/bf2c

all: $(BINS)

$(BINS): build

debug: enable-debug configure $(BINS)

enable-debug:
	$(eval BUILD_TYPE := "Debug")

release: all

configure:
	@cmake -S . -B build/ -G $(GENERATOR) \
		-DCMAKE_C_COMPILER=$(CC) \
		-DCMAKE_CXX_COMPILER=$(CXX) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

# technically, CMakeCache.txt or build.ninja would be a better indicator...
build/compile_commands.json: configure

build: build/compile_commands.json
	@cmake --build build/ --parallel

$(INSTALL_FILE): configure

install: $(BINS)
	@cmake --install build/

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
