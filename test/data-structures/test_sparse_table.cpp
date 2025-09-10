#include "../test_runner.h"
#include "data-structures/sparse_table.hpp"
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// Naive range reduce using a binary function
template <typename T, typename F>
T naive_range_reduce(const vector<T>& a, int l, int r, F f) {
    T res = a[l];
    for (int i = l + 1; i <= r; i++) res = f(res, a[i]);
    return res;
}

void test_min_rmq(TestRunner& runner) {
    runner.set_module("SparseTable - Min RMQ");

    runner.test("Basic min queries", []() {
        vector<int> arr = {5, 2, 7, 3, 6, 1, 4};
        auto fmin = [](int x, int y) { return min(x, y); };
        RMQ<int, decltype(fmin)> st(arr, fmin);

        ASSERT_EQ(st.get(0, 0), 5);
        ASSERT_EQ(st.get(0, 1), 2);
        ASSERT_EQ(st.get(1, 3), 2);
        ASSERT_EQ(st.get(2, 4), 3);
        ASSERT_EQ(st.get(0, 6), 1);
        ASSERT_EQ(st.get(5, 6), 1);
        return true;
    });

    runner.test("Min on monotonic arrays", []() {
        vector<int> inc = {1, 2, 3, 4, 5};
        vector<int> dec = {5, 4, 3, 2, 1};
        auto fmin = [](int x, int y) { return min(x, y); };
        RMQ<int, decltype(fmin)> st_inc(inc, fmin);
        RMQ<int, decltype(fmin)> st_dec(dec, fmin);
        ASSERT_EQ(st_inc.get(1, 3), 2);
        ASSERT_EQ(st_dec.get(1, 3), 2);
        ASSERT_EQ(st_inc.get(0, 4), 1);
        ASSERT_EQ(st_dec.get(0, 4), 1);
        return true;
    });
}

void test_max_rmq(TestRunner& runner) {
    runner.set_module("SparseTable - Max RMQ");

    runner.test("Basic max queries", []() {
        vector<int> arr = {5, 2, 7, 3, 6, 1, 4};
        auto fmax = [](int x, int y) { return max(x, y); };
        RMQ<int, decltype(fmax)> st(arr, fmax);
        ASSERT_EQ(st.get(0, 0), 5);
        ASSERT_EQ(st.get(0, 1), 5);
        ASSERT_EQ(st.get(1, 3), 7);
        ASSERT_EQ(st.get(2, 4), 7);
        ASSERT_EQ(st.get(0, 6), 7);
        ASSERT_EQ(st.get(5, 6), 4);
        return true;
    });
}

void test_gcd_rmq(TestRunner& runner) {
    runner.set_module("SparseTable - GCD RMQ");

    runner.test("GCD queries", []() {
        vector<int> arr = {12, 18, 24, 30, 6};
        auto fgcd = [](int a, int b) { return std::gcd(a, b); };
        RMQ<int, decltype(fgcd)> st(arr, fgcd);
        ASSERT_EQ(st.get(0, 1), 6);
        ASSERT_EQ(st.get(0, 2), 6);
        ASSERT_EQ(st.get(1, 3), 6);
        ASSERT_EQ(st.get(0, 4), 6);
        ASSERT_EQ(st.get(4, 4), 6);
        return true;
    });
}

void stress_test_sparse_table(TestRunner& runner) {
    runner.set_module("SparseTable - Stress Testing");

    runner.test("Compare with naive (min)", []() {
        StressTester stresser;
        for (int t = 0; t < 50; ++t) {
            int n = stresser.random_int(1, 200);
            auto arr = stresser.random_array(n, -1000, 1000);
            auto fmin = [](int x, int y) { return min(x, y); };
            RMQ<int, decltype(fmin)> st(arr, fmin);
            for (int q = 0; q < 200; ++q) {
                auto [l, r] = stresser.random_range(n);
                int fast = st.get(l, r);
                int naive = naive_range_reduce(arr, l, r, fmin);
                if (fast != naive) return false;
            }
        }
        return true;
    });

    runner.test("Compare with naive (max)", []() {
        StressTester stresser;
        for (int t = 0; t < 50; ++t) {
            int n = stresser.random_int(1, 200);
            auto arr = stresser.random_array(n, -1000, 1000);
            auto fmax = [](int x, int y) { return max(x, y); };
            RMQ<int, decltype(fmax)> st(arr, fmax);
            for (int q = 0; q < 200; ++q) {
                auto [l, r] = stresser.random_range(n);
                int fast = st.get(l, r);
                int naive = naive_range_reduce(arr, l, r, fmax);
                if (fast != naive) return false;
            }
        }
        return true;
    });
}

int main() {
    TestRunner runner;
    test_min_rmq(runner);
    test_max_rmq(runner);
    test_gcd_rmq(runner);
    stress_test_sparse_table(runner);
    runner.summary();
    return runner.get_exit_code();
}

