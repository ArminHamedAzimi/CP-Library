#include "../test_runner.h"
#include "data-structures/convex_hull_trick.hpp"
#include <vector>
#include <algorithm>

using namespace std;

static long long eval_naive(const vector<pair<long long,long long>>& lines, long long x) {
    long long best = LLONG_MIN;
    for (auto [k, m] : lines) best = max(best, k * x + m);
    return best;
}

void test_basic_convex_hull_trick(TestRunner& runner) {
    runner.set_module("Convex Hull Trick - Basics");

    runner.test("Insert and query simple lines", [](){
        LineContainer cht;
        // y = 2x + 3, y = -x + 10
        cht.add(2, 3);
        cht.add(-1, 10);
        ASSERT_EQ(cht.query(0), 10);
        ASSERT_EQ(cht.query(1), max(2LL*1 + 3, -1LL*1 + 10));
        ASSERT_EQ(cht.query(5), max(2LL*5 + 3, -1LL*5 + 10));
        return true;
    });

    runner.test("Dominated line does not affect result", [](){
        LineContainer cht;
        cht.add(1, 0);     // y = x
        cht.add(1, -100);  // worse same slope
        ASSERT_EQ(cht.query(10), 10);
        ASSERT_EQ(cht.query(-10), -10);
        return true;
    });

    runner.test("Parallel lines keep the best intercept", [](){
        LineContainer cht;
        cht.add(2, 1);
        cht.add(2, 5);
        ASSERT_EQ(cht.query(100), 2LL*100 + 5);
        ASSERT_EQ(cht.query(-100), -200 + 5);
        return true;
    });
}

void stress_test_convex_hull_trick(TestRunner& runner) {
    runner.set_module("Convex Hull Trick - Stress");

    runner.test("Random lines vs naive max", [](){
        StressTester stress;
        for (int tc = 0; tc < 200; ++tc) {
            LineContainer cht;
            vector<pair<long long,long long>> lines;
            int n = stress.random_int(1, 50);
            for (int i = 0; i < n; ++i) {
                long long k = stress.random_ll(-20, 20);
                long long m = stress.random_ll(-50, 50);
                cht.add(k, m);
                lines.push_back({k, m});
            }
            for (int q = 0; q < 100; ++q) {
                long long x = stress.random_ll(-50, 50);
                long long got = cht.query(x);
                long long exp = eval_naive(lines, x);
                if (got != exp) {
                    cout << "Mismatch on tc=" << tc << ", x=" << x << " got=" << got << " exp=" << exp << "\n";
                    return false;
                }
            }
        }
        return true;
    });
}

int main() {
    TestRunner runner;
    test_basic_convex_hull_trick(runner);
    stress_test_convex_hull_trick(runner);
    runner.summary();
    return runner.get_exit_code();
}

