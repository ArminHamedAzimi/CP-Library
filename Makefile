CXX = g++-15
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Itest -Isrc
DEBUGFLAGS = -g -fsanitize=address -fsanitize=undefined -DLOCAL

.PHONY: build test unit_tests cses_tests stress clean template help structure

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

cses_tests: build
	@echo "🏆 Running CSES problem tests..."
	cd build && make cses_tests

data-structures-tests: build
	@echo "📊 Running data structures tests..."
	cd build && make data_structures_tests

data-structures-cses: build
	@echo "📊🏆 Running data structures CSES tests..."
	cd build && make data_structures_cses

graph-tests: build
	@echo "🔗 Running graph algorithm tests..."
	cd build && make graph_tests

graph-cses: build
	@echo "🔗🏆 Running graph CSES tests..."
	cd build && make graph_cses

math-tests: build
	@echo "🔢 Running math algorithm tests..."
	cd build && make math_tests

math-cses: build
	@echo "🔢🏆 Running math CSES tests..."
	cd build && make math_cses

string-tests: build
	@echo "🔤 Running string algorithm tests..."
	cd build && make string_tests

string-cses: build
	@echo "🔤🏆 Running string CSES tests..."
	cd build && make string_cses

cses: build
	@if [ -z "$(PROBLEM)" ]; then \
		echo "❌ Usage: make cses PROBLEM=cses1651"; \
		exit 1; \
	fi
	@echo "🏆 Running $(PROBLEM) tests..."
	cd build && ctest --output-on-failure -R "$(PROBLEM)"

new-cses:
	@if [ -z "$(PROBLEM)" ] || [ -z "$(MODULE)" ]; then \
		echo "❌ Usage: make new-cses MODULE=data-structures PROBLEM=cses1651"; \
		exit 1; \
	fi
	@echo "📁 Creating CSES problem structure for $(PROBLEM) in $(MODULE)..."
	mkdir -p test/$(MODULE)/$(PROBLEM)/data
	@if [ ! -f test/$(MODULE)/$(PROBLEM)/main.cpp ]; then \
		echo '#include <bits/stdc++.h>' > test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo 'using namespace std;' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo 'void solve() {' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '    ' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '}' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo 'int main() {' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '    int q = 1;' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '    cin >> q;' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '    while (q--) {' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '        solve();' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '    }' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo '}' >> test/$(MODULE)/$(PROBLEM)/main.cpp; \
		echo "✅ Created main.cpp template"; \
	fi
	@echo "📄 Place your test cases in test/$(MODULE)/$(PROBLEM)/data/"
	@echo "   - Input files: 1.in, 2.in, 3.in, ..."
	@echo "   - Output files: 1.out, 2.out, 3.out, ..."

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
	@echo "  make test           - Run ALL tests (unit + CSES)"
	@echo "  make unit_tests     - Run only unit tests"
	@echo "  make cses_tests     - Run all CSES problem tests"
	@echo ""
	@echo "📊 Module-Specific Tests:"
	@echo "  make data-structures-tests  - Data structures unit tests"
	@echo "  make data-structures-cses   - Data structures CSES tests"
	@echo "  make graph-tests           - Graph algorithms unit tests"  
	@echo "  make graph-cses            - Graph algorithms CSES tests"
	@echo "  make math-tests            - Math algorithms unit tests"
	@echo "  make math-cses             - Math algorithms CSES tests"
	@echo "  make string-tests          - String algorithms unit tests"
	@echo "  make string-cses           - String algorithms CSES tests"
	@echo ""
	@echo "🏆 CSES-Specific Commands:"
	@echo "  make cses PROBLEM=cses1651 - Run specific CSES problem tests"
	@echo "  make new-cses MODULE=data-structures PROBLEM=cses1651 - Create new CSES problem structure"
	@echo ""
	@echo "🔧 Development Tools:"
	@echo "  make format                - Format code (requires clang-format)"
	@echo "  make docs                  - Generate docs (requires doxygen)"
	@echo "  make install               - Install library headers"
	@echo ""
	@echo "📁 CSES Problem Structure:"
	@echo "  test/MODULE/csesXXXX/main.cpp     - Your solution"
	@echo "  test/MODULE/csesXXXX/data/N.in    - Input files"
	@echo "  test/MODULE/csesXXXX/data/N.out   - Expected output files"