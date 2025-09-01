CXX = g++-15
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Itest -Isrc
DEBUGFLAGS = -g -fsanitize=address -fsanitize=undefined -DLOCAL

.PHONY: build test unit_tests stress clean template help structure

all: build

build:
	@echo "🔨 Building with CMake..."
	mkdir -p build
	cd build && cmake .. && make

test: build
	@echo "🧪 Running all tests..."
	cd build && make all_tests

unit_tests: build
	@echo "📋 Running unit tests..."
	cd build && make unit_tests

data-structures-tests: build
	@echo "📊 Running data structures tests..."
	cd build && make data_structures_tests

graph-tests: build
	@echo "🔗 Running graph algorithm tests..."
	cd build && make graph_tests

math-tests: build
	@echo "🔢 Running math algorithm tests..."
	cd build && make math_tests

string-tests: build
	@echo "🔤 Running string algorithm tests..."
	cd build && make string_tests

stress: build
	@echo "💪 Running stress tests..."
	cd build && make stress

clean:
	@echo "🧹 Cleaning build files..."
	rm -rf build/
	rm -f template_test template_debug
	rm -f test_* *_test

install: build
	@echo "📦 Installing library headers..."
	cd build && make install

docs:
	@echo "📚 Generating documentation..."
	@if command -v doxygen >/dev/null 2>&1; then \
		doxygen Doxyfile 2>/dev/null || echo "Create Doxyfile first"; \
	else \
		echo "❌ Doxygen not installed"; \
	fi

format:
	@echo "✨ Formatting code..."
	@if command -v clang-format >/dev/null 2>&1; then \
		find src test -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i; \
		echo "✅ Code formatted"; \
	else \
		echo "❌ clang-format not installed"; \
	fi

help:
	@echo "🚀 Competitive Programming Library - Available Commands"
	@echo "======================================================"
	@echo ""
	@echo "🔨 Build Commands:"
	@echo "  make build          - Build entire project with CMake"
	@echo "  make clean          - Remove all build files"
	@echo ""
	@echo "🧪 Testing Commands:"
	@echo "  make test           - Run ALL tests (unit + stress)"
	@echo "  make unit_tests     - Run only unit tests"
	@echo "  make stress         - Run only stress tests"
	@echo ""
	@echo "📊 Module-Specific Tests:"
	@echo "  make data-structures-tests  - Data structures module"
	@echo "  make graph-tests           - Graph algorithms module"  
	@echo "  make math-tests            - Math algorithms module"
	@echo "  make string-tests          - String algorithms module"
	@echo ""
	@echo "🔧 Development Tools:"
	@echo "  make format                - Format code (requires clang-format)"
	@echo "  make docs                  - Generate docs (requires doxygen)"
	@echo "  make install               - Install library headers"
	@echo ""
	@echo "💡 Quick Start:"
	@echo "  1. make setup              # Create project structure"
	@echo "  2. make test-bits          # Verify g++-15 and bits/stdc++.h work"  
	@echo "  3. make template           # Test contest template"
	@echo "  4. make build              # Build everything"
	@echo "  5. make test               # Run all tests"
	@echo ""
	@echo "📁 After 'make setup', add your implementations to src/ and tests to test/"

# Show available CMake targets
cmake-help: build
	@echo "🎯 Available CMake Targets:"
	@echo "(run these from build/ directory after 'make build')"
	cd build && make help