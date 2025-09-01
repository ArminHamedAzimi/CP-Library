#include "test_runner.h"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;

TestRunner::TestRunner() : tests_run(0), tests_passed(0) {}

void TestRunner::set_module(const string& module) {
    current_module = module;
    cout << "\n=== Testing " << module << " ===\n";
}

void TestRunner::test(const string& test_name, function<bool()> test_func) {
    tests_run++;
    cout << "Test: " << test_name << " ... ";
    cout.flush();
    
    auto start = chrono::high_resolution_clock::now();
    
    try {
        if (test_func()) {
            tests_passed++;
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            cout << "✅ PASSED (" << duration.count() << "μs)\n";
        } else {
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            cout << "❌ FAILED (" << duration.count() << "μs)\n";
        }
    } catch (const exception& e) {
        cout << "❌ FAILED (Exception: " << e.what() << ")\n";
    } catch (...) {
        cout << "❌ FAILED (Unknown exception)\n";
    }
}

void TestRunner::summary() {
    cout << "\n=== Test Summary for " << current_module << " ===\n";
    cout << "Tests run: " << tests_run << "\n";
    cout << "Tests passed: " << tests_passed << "\n";
    cout << "Tests failed: " << (tests_run - tests_passed) << "\n";
    
    if (tests_passed == tests_run) {
        cout << "🎉 All tests PASSED!\n";
    } else {
        cout << "💥 " << (tests_run - tests_passed) << " tests FAILED\n";
    }
    
    double success_rate = tests_run > 0 ? (double)tests_passed / tests_run * 100.0 : 0.0;
    cout << "Success rate: " << fixed << setprecision(1) << success_rate << "%\n";
}

int TestRunner::get_exit_code() {
    return (tests_passed == tests_run) ? 0 : 1;
}

StressTester::StressTester() : seed(chrono::steady_clock::now().time_since_epoch().count()) {
    rng.seed(seed);
    cout << "🧪 Stress tester initialized (seed: " << seed << ")\n";
}

int StressTester::random_int(int min_val, int max_val) {
    uniform_int_distribution<int> dist(min_val, max_val);
    return dist(rng);
}

long long StressTester::random_ll(long long min_val, long long max_val) {
    uniform_int_distribution<long long> dist(min_val, max_val);
    return dist(rng);
}

vector<int> StressTester::random_array(int size, int min_val, int max_val) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = random_int(min_val, max_val);
    }
    return arr;
}

pair<int, int> StressTester::random_range(int n) {
    int l = random_int(0, n - 1);
    int r = random_int(0, n - 1);
    if (l > r) swap(l, r);
    return {l, r};
}

string StressTester::random_string(int length, char min_char, char max_char) {
    string s(length, ' ');
    for (int i = 0; i < length; i++) {
        s[i] = static_cast<char>(random_int(min_char, max_char));
    }
    return s;
}

vector<pair<int, int>> StressTester::random_edges(int n, int m, bool allow_self_loops) {
    vector<pair<int, int>> edges;
    set<pair<int, int>> used;
    
    while (edges.size() < m) {
        int u = random_int(0, n - 1);
        int v = random_int(0, n - 1);
        
        if (!allow_self_loops && u == v) continue;
        if (u > v) swap(u, v);
        
        if (used.find({u, v}) == used.end()) {
            used.insert({u, v});
            edges.push_back({u, v});
        }
    }
    
    return edges;
}

void StressTester::print_progress(int current, int total) {
    if (current % (total / 10 + 1) == 0 || current == total - 1) {
        int percent = (100 * current) / total;
        cout << "Progress: " << current + 1 << "/" << total 
             << " (" << percent << "%)\n";
    }
}