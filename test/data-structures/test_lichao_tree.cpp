#include "../test_runner.h"
#include "data-structures/lichao_tree.hpp"
#include <vector>
#include <algorithm>

using namespace std;

static long long eval_max_naive(const vector<pair<long long,long long>>& lines, long long x) {
    long long best = LLONG_MIN;
    for (auto [a, b] : lines) best = max(best, a * x + b);
    return best;
}

static long long eval_min_naive(const vector<pair<long long,long long>>& lines, long long x) {
    long long best = LLONG_MAX;
    for (auto [a, b] : lines) best = min(best, a * x + b);
    return best;
}

void test_basic_lichao(TestRunner& runner) {
    runner.set_module("Li Chao Tree - Basics");

    runner.test("Max mode basic queries", [](){
        // Domain kept small to ensure deterministic checks
        LiChaoTree lc(0, 100);
        // y = 2x + 3, y = -x + 10
        lc.add_line(2, 3);
        lc.add_line(-1, 10);
        ASSERT_EQ(lc.query(0), max(3LL, 10LL));
        ASSERT_EQ(lc.query(5), max(2LL*5 + 3, -1LL*5 + 10));
        ASSERT_EQ(lc.query(100), max(2LL*100 + 3, -1LL*100 + 10));
        return true;
    });

    runner.test("Dominated line has no effect (max)", [](){
        LiChaoTree lc(0, 100);
        lc.add_line(1, 0);     // y = x
        lc.add_line(1, -100);  // dominated
        ASSERT_EQ(lc.query(0), 0);
        ASSERT_EQ(lc.query(50), 50);
        return true;
    });
}

void stress_test_lichao(TestRunner& runner) {
    runner.set_module("Li Chao Tree - Stress");

    runner.test("Random lines vs naive (max) [single instance]", [](){
        StressTester stress;
        const int XL = 0, XR = 1000;
        // Use a single Li Chao instance to avoid repeated large allocations
        LiChaoTree lc(XL, XR);
        vector<pair<long long,long long>> lines;
        int n = 120; // enough variety
        for (int i = 0; i < n; ++i) {
            long long a = stress.random_ll(-50, 50);
            long long b = stress.random_ll(-200, 200);
            lc.add_line(a, b);
            lines.push_back({a, b});
        }
        for (int q = 0; q < 400; ++q) {
            long long x = stress.random_ll(XL, XR);
            long long got = lc.query(x);
            long long exp = eval_max_naive(lines, x);
            if (got != exp) {
                cout << "Max mismatch x=" << x << " got=" << got << " exp=" << exp << "\n";
                return false;
            }
        }
        return true;
    });
}

int main() {
    TestRunner runner;
    test_basic_lichao(runner);
    stress_test_lichao(runner);
    runner.summary();
    return runner.get_exit_code();
}
