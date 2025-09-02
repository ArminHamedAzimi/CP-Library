#include "../test_runner.h"
#include "data-structures/fenwick_tree.hpp"
#include <vector>
#include <algorithm>

using namespace std;

// Naive implementation for comparison
template<typename T>
class NaiveFenwick {
    vector<T> arr;
public:
    NaiveFenwick(int n) : arr(n, T(0)) {}
    
    void add(int idx, T val) {
        arr[idx] += val;
    }
    
    T prefix_sum(int idx) const {
        if (idx < 0) return T(0);
        if (idx >= (int)arr.size()) idx = arr.size() - 1;
        T sum = 0;
        for (int i = 0; i <= idx; i++) {
            sum += arr[i];
        }
        return sum;
    }
    
    T range_sum(int l, int r) const {
        if (l > r) return T(0);
        return prefix_sum(r) - prefix_sum(l - 1);
    }
    
    int lower_bound(T target) const {
        if (target <= T(0)) return -1;
        for (int i = 0; i < (int)arr.size(); i++) {
            if (prefix_sum(i) >= target) return i;
        }
        return arr.size();
    }
    
    int upper_bound(T target) const {
        if (target < T(0)) return -1;
        for (int i = 0; i < (int)arr.size(); i++) {
            if (prefix_sum(i) > target) return i;
        }
        return arr.size();
    }
};

template<typename T>
class NaiveFenwickRangeAdd {
    vector<T> arr;
public:
    NaiveFenwickRangeAdd(int n) : arr(n, T(0)) {}
    
    void add_range(int l, int r, T val) {
        for (int i = l; i <= r; i++) {
            arr[i] += val;
        }
    }
    
    T prefix_sum(int idx) const {
        if (idx < 0) return T(0);
        T sum = 0;
        for (int i = 0; i <= idx; i++) {
            sum += arr[i];
        }
        return sum;
    }
    
    T range_sum(int l, int r) const {
        if (l > r) return T(0);
        return prefix_sum(r) - prefix_sum(l - 1);
    }
};

template<typename T>
class NaiveFenwickRangeAP {
    vector<T> arr;
public:
    NaiveFenwickRangeAP(int n) : arr(n, T(0)) {}
    
    void add_range_constant(int l, int r, T val) {
        for (int i = l; i <= r; i++) {
            arr[i] += val;
        }
    }
    
    void add_range_increasing_by_one(int l, int r) {
        for (int i = l; i <= r; i++) {
            arr[i] += T(i - l + 1);
        }
    }
    
    T prefix_sum(int idx) const {
        if (idx < 0) return T(0);
        T sum = 0;
        for (int i = 0; i <= idx; i++) {
            sum += arr[i];
        }
        return sum;
    }
    
    T range_sum(int l, int r) const {
        if (l > r) return T(0);
        return prefix_sum(r) - prefix_sum(l - 1);
    }
};

void test_basic_fenwick_tree(TestRunner& runner) {
    runner.set_module("FenwickTree - Basic Operations");
    
    runner.test("Construction and single point updates", []() {
        FenwickTree<int> ft(5);
        
        // Initially all zeros
        ASSERT_EQ(ft.prefix_sum(0), 0);
        ASSERT_EQ(ft.prefix_sum(4), 0);
        ASSERT_EQ(ft.range_sum(0, 4), 0);
        
        // Add some values
        ft.add(0, 1);
        ft.add(1, 3);
        ft.add(2, 5);
        ft.add(3, 7);
        ft.add(4, 9);
        
        ASSERT_EQ(ft.prefix_sum(0), 1);
        ASSERT_EQ(ft.prefix_sum(1), 4);
        ASSERT_EQ(ft.prefix_sum(2), 9);
        ASSERT_EQ(ft.prefix_sum(3), 16);
        ASSERT_EQ(ft.prefix_sum(4), 25);
        
        return true;
    });
    
    runner.test("Range sum queries", []() {
        FenwickTree<int> ft(5);
        ft.add(0, 1);
        ft.add(1, 3);
        ft.add(2, 5);
        ft.add(3, 7);
        ft.add(4, 9);
        
        ASSERT_EQ(ft.range_sum(0, 0), 1);
        ASSERT_EQ(ft.range_sum(1, 1), 3);
        ASSERT_EQ(ft.range_sum(0, 2), 9);
        ASSERT_EQ(ft.range_sum(2, 4), 21);
        ASSERT_EQ(ft.range_sum(1, 3), 15);
        ASSERT_EQ(ft.range_sum(0, 4), 25);
        
        return true;
    });
    
    runner.test("Lower bound functionality", []() {
        FenwickTree<int> ft(5);
        ft.add(0, 2);  // prefix: [2, 2, 2, 2, 2]
        ft.add(1, 3);  // prefix: [2, 5, 5, 5, 5]
        ft.add(2, 1);  // prefix: [2, 5, 6, 6, 6]
        ft.add(3, 4);  // prefix: [2, 5, 6, 10, 10]
        
        ASSERT_EQ(ft.lower_bound(0), -1);  // target <= 0
        ASSERT_EQ(ft.lower_bound(1), 0);   // prefix[0] = 2 >= 1
        ASSERT_EQ(ft.lower_bound(2), 0);   // prefix[0] = 2 >= 2
        ASSERT_EQ(ft.lower_bound(3), 1);   // prefix[1] = 5 >= 3
        ASSERT_EQ(ft.lower_bound(5), 1);   // prefix[1] = 5 >= 5
        ASSERT_EQ(ft.lower_bound(6), 2);   // prefix[2] = 6 >= 6
        ASSERT_EQ(ft.lower_bound(7), 3);   // prefix[3] = 10 >= 7
        ASSERT_EQ(ft.lower_bound(10), 3);  // prefix[3] = 10 >= 10
        ASSERT_EQ(ft.lower_bound(11), 5);  // no such index
        
        return true;
    });
    
    runner.test("Upper bound functionality", []() {
        FenwickTree<int> ft(5);
        ft.add(0, 2);  // prefix: [2, 2, 2, 2, 2]
        ft.add(1, 3);  // prefix: [2, 5, 5, 5, 5]
        ft.add(2, 1);  // prefix: [2, 5, 6, 6, 6]
        ft.add(3, 4);  // prefix: [2, 5, 6, 10, 10]
        
        ASSERT_EQ(ft.upper_bound(-1), -1);  // target < 0
        ASSERT_EQ(ft.upper_bound(0), 0);    // prefix[0] = 2 > 0
        ASSERT_EQ(ft.upper_bound(1), 0);    // prefix[0] = 2 > 1
        ASSERT_EQ(ft.upper_bound(2), 1);    // prefix[1] = 5 > 2
        ASSERT_EQ(ft.upper_bound(4), 1);    // prefix[1] = 5 > 4
        ASSERT_EQ(ft.upper_bound(5), 2);    // prefix[2] = 6 > 5
        ASSERT_EQ(ft.upper_bound(6), 3);    // prefix[3] = 10 > 6
        ASSERT_EQ(ft.upper_bound(9), 3);    // prefix[3] = 10 > 9
        ASSERT_EQ(ft.upper_bound(10), 5);   // no such index
        
        return true;
    });
    
    runner.test("Edge cases", []() {
        // Single element
        FenwickTree<int> ft1(1);
        ft1.add(0, 5);
        ASSERT_EQ(ft1.prefix_sum(0), 5);
        ASSERT_EQ(ft1.range_sum(0, 0), 5);
        ASSERT_EQ(ft1.lower_bound(5), 0);
        ASSERT_EQ(ft1.upper_bound(4), 0);
        
        // Empty range sum
        FenwickTree<int> ft2(5);
        ASSERT_EQ(ft2.range_sum(3, 2), 0);  // l > r
        
        // Negative prefix_sum queries
        ASSERT_EQ(ft2.prefix_sum(-1), 0);
        ASSERT_EQ(ft2.prefix_sum(-10), 0);
        
        return true;
    });
}

void test_fenwick_range_add(TestRunner& runner) {
    runner.set_module("FenwickRangeAdd - Range Operations");
    
    runner.test("Range addition operations", []() {
        FenwickRangeAdd<int> fr(5);
        
        // Initially all zeros
        ASSERT_EQ(fr.range_sum(0, 4), 0);
        
        // Add 5 to range [1, 3]
        fr.add_range(1, 3, 5);
        ASSERT_EQ(fr.prefix_sum(0), 0);
        ASSERT_EQ(fr.prefix_sum(1), 5);
        ASSERT_EQ(fr.prefix_sum(2), 10);
        ASSERT_EQ(fr.prefix_sum(3), 15);
        ASSERT_EQ(fr.prefix_sum(4), 15);
        
        ASSERT_EQ(fr.range_sum(0, 0), 0);
        ASSERT_EQ(fr.range_sum(1, 1), 5);
        ASSERT_EQ(fr.range_sum(2, 2), 5);
        ASSERT_EQ(fr.range_sum(3, 3), 5);
        ASSERT_EQ(fr.range_sum(4, 4), 0);
        ASSERT_EQ(fr.range_sum(1, 3), 15);
        
        return true;
    });
    
    runner.test("Multiple range operations", []() {
        FenwickRangeAdd<int> fr(6);
        
        fr.add_range(0, 2, 3);  // [3, 3, 3, 0, 0, 0]
        fr.add_range(2, 4, 2);  // [3, 3, 5, 2, 2, 0]
        fr.add_range(1, 5, 1);  // [3, 4, 6, 3, 3, 1]
        
        ASSERT_EQ(fr.range_sum(0, 0), 3);
        ASSERT_EQ(fr.range_sum(1, 1), 4);
        ASSERT_EQ(fr.range_sum(2, 2), 6);
        ASSERT_EQ(fr.range_sum(3, 3), 3);
        ASSERT_EQ(fr.range_sum(4, 4), 3);
        ASSERT_EQ(fr.range_sum(5, 5), 1);
        
        ASSERT_EQ(fr.range_sum(0, 5), 20);
        ASSERT_EQ(fr.range_sum(1, 4), 16);
        
        return true;
    });
    
    runner.test("Edge cases for range add", []() {
        // Single element
        FenwickRangeAdd<int> fr1(1);
        fr1.add_range(0, 0, 10);
        ASSERT_EQ(fr1.range_sum(0, 0), 10);
        
        // Full range
        FenwickRangeAdd<int> fr2(5);
        fr2.add_range(0, 4, 7);
        ASSERT_EQ(fr2.range_sum(0, 4), 35);
        
        // Empty range sum
        ASSERT_EQ(fr2.range_sum(3, 2), 0);
        
        return true;
    });
}

void test_fenwick_range_ap(TestRunner& runner) {
    runner.set_module("FenwickRangeAP - Arithmetic Progression");
    
    runner.test("Arithmetic progression range add", []() {
        FenwickRangeAP<long long> fa(5);
        
        // Add 1,2,3 to positions 1,2,3
        fa.add_range_increasing_by_one(1, 3);
        
        ASSERT_EQ(fa.range_sum(0, 0), 0);
        ASSERT_EQ(fa.range_sum(1, 1), 1);
        ASSERT_EQ(fa.range_sum(2, 2), 2);
        ASSERT_EQ(fa.range_sum(3, 3), 3);
        ASSERT_EQ(fa.range_sum(4, 4), 0);
        
        ASSERT_EQ(fa.range_sum(1, 3), 6);
        ASSERT_EQ(fa.range_sum(0, 4), 6);
        
        return true;
    });
    
    runner.test("Constant range add", []() {
        FenwickRangeAP<long long> fa(5);
        
        fa.add_range_constant(1, 3, 5);
        
        ASSERT_EQ(fa.range_sum(0, 0), 0);
        ASSERT_EQ(fa.range_sum(1, 1), 5);
        ASSERT_EQ(fa.range_sum(2, 2), 5);
        ASSERT_EQ(fa.range_sum(3, 3), 5);
        ASSERT_EQ(fa.range_sum(4, 4), 0);
        
        ASSERT_EQ(fa.range_sum(1, 3), 15);
        
        return true;
    });
    
    runner.test("Mixed operations", []() {
        FenwickRangeAP<long long> fa(6);
        
        // Add constant 10 to range [0, 2]
        fa.add_range_constant(0, 2, 10);
        // Add AP 1,2,3,4 to range [2, 5]
        fa.add_range_increasing_by_one(2, 5);
        
        ASSERT_EQ(fa.range_sum(0, 0), 10);
        ASSERT_EQ(fa.range_sum(1, 1), 10);
        ASSERT_EQ(fa.range_sum(2, 2), 11);  // 10 + 1
        ASSERT_EQ(fa.range_sum(3, 3), 2);   // 0 + 2
        ASSERT_EQ(fa.range_sum(4, 4), 3);   // 0 + 3
        ASSERT_EQ(fa.range_sum(5, 5), 4);   // 0 + 4
        
        return true;
    });
    
    runner.test("Large arithmetic progression", []() {
        FenwickRangeAP<long long> fa(10);
        
        // Add 1,2,3,4,5,6,7,8,9,10 to entire array
        fa.add_range_increasing_by_one(0, 9);
        
        // Sum should be 1+2+...+10 = 55
        ASSERT_EQ(fa.range_sum(0, 9), 55);
        
        // Partial sums
        ASSERT_EQ(fa.range_sum(0, 4), 15);  // 1+2+3+4+5
        ASSERT_EQ(fa.range_sum(5, 9), 40);  // 6+7+8+9+10
        
        return true;
    });
}

void stress_test_fenwick_trees(TestRunner& runner) {
    runner.set_module("Fenwick Trees - Stress Testing");
    
    runner.test("FenwickTree vs Naive", []() {
        StressTester stress;
        
        return stress.compare_simple<bool>(
            "FenwickTree vs Naive",
            [&](const vector<int>& arr) -> bool {
                int n = arr.size();
                FenwickTree<int> ft(n);
                NaiveFenwick<int> naive(n);
                
                for (int op = 0; op < 30; op++) {
                    int idx = stress.random_int(0, n - 1);
                    int val = stress.random_int(0, 100);
                    
                    if (op % 4 == 0) {
                        // Point update
                        ft.add(idx, val);
                        naive.add(idx, val);
                    } else if (op % 4 == 1) {
                        // Prefix sum query
                        if (ft.prefix_sum(idx) != naive.prefix_sum(idx)) return false;
                    } else if (op % 4 == 2) {
                        // Range sum query
                        auto [l, r] = stress.random_range(n);
                        if (ft.range_sum(l, r) != naive.range_sum(l, r)) return false;
                    } else {
                        // Bound queries (only if positive sums)
                        int target = stress.random_int(1, 1000);
                        if (ft.lower_bound(target) != naive.lower_bound(target)) return false;
                        if (ft.upper_bound(target) != naive.upper_bound(target)) return false;
                    }
                }
                return true;
            },
            [](const vector<int>& arr) -> bool { return true; },
            200, 20, 50
        );
    });
    
    runner.test("FenwickRangeAdd vs Naive", []() {
        StressTester stress;
        
        return stress.compare_simple<bool>(
            "FenwickRangeAdd vs Naive",
            [&](const vector<int>& arr) -> bool {
                int n = arr.size();
                FenwickRangeAdd<int> fr(n);
                NaiveFenwickRangeAdd<int> naive(n);
                
                for (int op = 0; op < 25; op++) {
                    if (op % 3 == 0) {
                        // Range update
                        auto [l, r] = stress.random_range(n);
                        int val = stress.random_int(-50, 50);
                        fr.add_range(l, r, val);
                        naive.add_range(l, r, val);
                    } else if (op % 3 == 1) {
                        // Prefix sum query
                        int idx = stress.random_int(0, n - 1);
                        if (fr.prefix_sum(idx) != naive.prefix_sum(idx)) return false;
                    } else {
                        // Range sum query
                        auto [l, r] = stress.random_range(n);
                        if (fr.range_sum(l, r) != naive.range_sum(l, r)) return false;
                    }
                }
                return true;
            },
            [](const vector<int>& arr) -> bool { return true; },
            200, 15, 50
        );
    });
    
    runner.test("FenwickRangeAP vs Naive", []() {
        StressTester stress;
        
        return stress.compare_simple<bool>(
            "FenwickRangeAP vs Naive",
            [&](const vector<int>& arr) -> bool {
                int n = arr.size();
                FenwickRangeAP<long long> fa(n);
                NaiveFenwickRangeAP<long long> naive(n);
                
                for (int op = 0; op < 20; op++) {
                    auto [l, r] = stress.random_range(n);
                    
                    if (op % 4 == 0) {
                        // Constant range add
                        int val = stress.random_int(-30, 30);
                        fa.add_range_constant(l, r, val);
                        naive.add_range_constant(l, r, val);
                    } else if (op % 4 == 1) {
                        // AP range add
                        fa.add_range_increasing_by_one(l, r);
                        naive.add_range_increasing_by_one(l, r);
                    } else if (op % 4 == 2) {
                        // Prefix sum query
                        int idx = stress.random_int(0, n - 1);
                        if (fa.prefix_sum(idx) != naive.prefix_sum(idx)) return false;
                    } else {
                        // Range sum query
                        if (fa.range_sum(l, r) != naive.range_sum(l, r)) return false;
                    }
                }
                return true;
            },
            [](const vector<int>& arr) -> bool { return true; },
            150, 12, 30
        );
    });
}

void test_performance(TestRunner& runner) {
    runner.set_module("Fenwick Trees - Performance");
    
    runner.test("Large array operations", []() {
        const int n = 100000;
        FenwickTree<long long> ft(n);
        FenwickRangeAdd<long long> fr(n);
        FenwickRangeAP<long long> fa(n);
        
        // Test that large operations complete quickly
        for (int i = 0; i < 1000; i++) {
            ft.add(i % n, 1);
            fr.add_range(i % (n/2), (i % (n/2)) + n/2 - 1, 1);
            fa.add_range_constant(i % (n/3), (i % (n/3)) + n/3 - 1, 1);
        }
        
        long long sum1 = ft.range_sum(0, n-1);
        long long sum2 = fr.range_sum(0, n-1);
        long long sum3 = fa.range_sum(0, n-1);
        
        // Just verify operations completed (sums will be positive)
        ASSERT_TRUE(sum1 > 0);
        ASSERT_TRUE(sum2 > 0);
        ASSERT_TRUE(sum3 > 0);
        
        return true;
    });
}

int main() {
    TestRunner runner;
    
    test_basic_fenwick_tree(runner);
    test_fenwick_range_add(runner);
    test_fenwick_range_ap(runner);
    stress_test_fenwick_trees(runner);
    test_performance(runner);
    
    runner.summary();
    return runner.get_exit_code();
}