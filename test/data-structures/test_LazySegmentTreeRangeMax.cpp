#include "../test_runner.h"
#include "data-structures/LazySegmentTreeRangeMax.hpp"
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

template<typename T, typename F>
class NaiveRangeMax {
    vector<T> arr;
    T identity;
    F combine_func;
public:
    NaiveRangeMax(const vector<T>& in, T identity_val, const F& func) 
        : arr(in), identity(identity_val), combine_func(func) {}
    
    void range_add(int l, int r, T val) {
        for (int i = l; i <= r; i++) 
            arr[i] += val;
    }
    
    void range_set(int l, int r, T val) {
        for (int i = l; i <= r; i++) 
            arr[i] = val;
    }
    
    T range_query(int l, int r) {
        T result = identity;
        for (int i = l; i <= r; i++) 
            result = combine_func(result, arr[i]);
        return result;
    }
};

void test_basic_max_functionality(TestRunner& runner) {
    runner.set_module("LazySegmentTreeRangeMax - Basic Max Operations");
    
    runner.test("Construction and basic max queries", []() {
        vector<int> arr = {1, 3, 5, 7, 9};
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        
        ASSERT_EQ(st.range_query(0, 0), 1);
        ASSERT_EQ(st.range_query(1, 1), 3);
        ASSERT_EQ(st.range_query(0, 2), 5); 
        ASSERT_EQ(st.range_query(2, 4), 9); 
        ASSERT_EQ(st.range_query(0, 4), 9); 
        
        return true;
    });
    
    runner.test("Range addition updates with max", []() {
        vector<int> arr = {0, 0, 0, 0, 0};
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        
        st.range_add(1, 3, 5);
        ASSERT_EQ(st.range_query(0, 0), 0);
        ASSERT_EQ(st.range_query(1, 1), 5);
        ASSERT_EQ(st.range_query(2, 2), 5);
        ASSERT_EQ(st.range_query(3, 3), 5);
        ASSERT_EQ(st.range_query(4, 4), 0);
        ASSERT_EQ(st.range_query(1, 3), 5); 
        ASSERT_EQ(st.range_query(0, 4), 5); 
        
        return true;
    });
    
    runner.test("Range assignment updates with max", []() {
        vector<int> arr = {1, 2, 3, 4, 5};
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        
        st.range_set(1, 3, 10);
        ASSERT_EQ(st.range_query(0, 0), 1);
        ASSERT_EQ(st.range_query(1, 1), 10);
        ASSERT_EQ(st.range_query(2, 2), 10);
        ASSERT_EQ(st.range_query(3, 3), 10);
        ASSERT_EQ(st.range_query(4, 4), 5);
        ASSERT_EQ(st.range_query(1, 3), 10); 
        ASSERT_EQ(st.range_query(0, 4), 10); 
        
        return true;
    });
    
    runner.test("Mixed operations with max", []() {
        vector<int> arr = {1, 1, 1, 1, 1};
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        
        st.range_set(0, 4, 5);
        ASSERT_EQ(st.range_query(0, 4), 5);
        
        st.range_add(1, 3, 2);   
        ASSERT_EQ(st.range_query(0, 4), 7); 
        
        st.range_set(2, 2, 100);
        ASSERT_EQ(st.range_query(2, 2), 100);
        ASSERT_EQ(st.range_query(0, 4), 100); 
        
        return true;
    });
}

void test_basic_min_functionality(TestRunner& runner) {
    runner.set_module("LazySegmentTreeRangeMax - Basic Min Operations");
    
    runner.test("Construction and basic min queries", []() {
        vector<int> arr = {9, 7, 5, 3, 1};
        auto min_func = [](int a, int b) { return min(a, b); };
        LazyRangeMax<int, decltype(min_func)> st(arr, 1e9, min_func);
        
        ASSERT_EQ(st.range_query(0, 0), 9);
        ASSERT_EQ(st.range_query(1, 1), 7);
        ASSERT_EQ(st.range_query(0, 2), 5); 
        ASSERT_EQ(st.range_query(2, 4), 1); 
        ASSERT_EQ(st.range_query(0, 4), 1); 
        
        return true;
    });
    
    runner.test("Range operations with min", []() {
        vector<int> arr = {10, 10, 10, 10, 10};
        auto min_func = [](int a, int b) { return min(a, b); };
        LazyRangeMax<int, decltype(min_func)> st(arr, 1e9, min_func);
        
        st.range_add(1, 3, -3);
        ASSERT_EQ(st.range_query(0, 4), 7); 
        
        st.range_set(2, 2, 1);
        ASSERT_EQ(st.range_query(0, 4), 1); 
        
        return true;
    });
}

void test_edge_cases(TestRunner& runner) {
    runner.set_module("LazySegmentTreeRangeMax - Edge Cases");
    
    runner.test("Single element array with max", []() {
        vector<int> arr = {42};
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        
        ASSERT_EQ(st.range_query(0, 0), 42);
        
        st.range_add(0, 0, 10);
        ASSERT_EQ(st.range_query(0, 0), 52);
        
        st.range_set(0, 0, 100);
        ASSERT_EQ(st.range_query(0, 0), 100);
        
        return true;
    });
    
    runner.test("Large values (long long) with max", []() {
        vector<long long> arr = {1000000000LL, 2000000000LL, 3000000000LL};
        auto max_func = [](long long a, long long b) { return max(a, b); };
        LazyRangeMax<long long, decltype(max_func)> st(arr.size(), -1e18, max_func);
        st.build(arr);
        
        ASSERT_EQ(st.range_query(0, 2), 3000000000LL);
        
        st.range_add(0, 2, 1000000000LL);
        ASSERT_EQ(st.range_query(0, 2), 4000000000LL); 
        
        return true;
    });
    
    runner.test("Negative numbers with max", []() {
        vector<int> arr = {-5, -3, -1, -2, -4};
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        
        ASSERT_EQ(st.range_query(0, 4), -1); 
        ASSERT_EQ(st.range_query(0, 2), -1); 
        ASSERT_EQ(st.range_query(3, 4), -2); 
        
        return true;
    });
    
    runner.test("Zero array with max", []() {
        vector<int> arr(10, 0);
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        
        ASSERT_EQ(st.range_query(0, 9), 0);
        
        st.range_add(3, 7, 5);
        ASSERT_EQ(st.range_query(3, 7), 5);
        ASSERT_EQ(st.range_query(0, 9), 5); 
        
        return true;
    });
    
    runner.test("Identity value behavior", []() {
        vector<int> arr = {1, 2, 3};
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr.size(), -100, max_func);
        st.build(arr);
        
    
    
        ASSERT_EQ(st.range_query(0, 2), 3); 
        
        return true;
    });
}

void test_performance_characteristics(TestRunner& runner) {
    runner.set_module("LazySegmentTreeRangeMax - Performance");
    
    runner.test("Large array operations with max", []() {
        const int n = 10000;
        vector<int> arr(n, 1);
        auto max_func = [](int a, int b) { return max(a, b); };
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        
        st.range_set(0, n - 1, 5);
        ASSERT_EQ(st.range_query(0, n - 1), 5);
        
        st.range_add(1000, 8000, 3);
        ASSERT_EQ(st.range_query(1000, 8000), 8); 
        ASSERT_EQ(st.range_query(0, n - 1), 8);   
        
        return true;
    });
}

void stress_test_segment_tree_max(TestRunner& runner) {
    runner.set_module("LazySegmentTreeRangeMax - Stress Testing");
    
    runner.test("Compare max implementation with naive", []() {
        StressTester stress;
        
        return stress.compare_simple<bool>(
            "LazySegmentTreeRangeMax vs Naive",
            
            [&](const vector<int>& arr) -> bool {
                auto max_func = [](int a, int b) { return max(a, b); };
                LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
                NaiveRangeMax<int, decltype(max_func)> naive(arr, -1e9, max_func);
                
                for (int op = 0; op < 20; op++) {
                    auto [l, r] = stress.random_range(arr.size());
                    int val = stress.random_int(-50, 50);
                    
                    if (op % 3 == 0) {
                        int st_result = st.range_query(l, r);
                        int naive_result = naive.range_query(l, r);
                        if (st_result != naive_result) return false;
                        
                    } else if (op % 3 == 1) {
                        st.range_add(l, r, val);
                        naive.range_add(l, r, val);
                        
                    } else {
                        st.range_set(l, r, val);
                        naive.range_set(l, r, val);
                    }
                }
                
            
                for (int i = 0; i < (int)arr.size(); i++) {
                    if (st.range_query(i, i) != naive.range_query(i, i)) {
                        return false;
                    }
                }
                
                return true;
            },
            
            [](const vector<int>& arr) -> bool {
                return true;
            },
            
            100,   
            20,    
            50     
        );
    });
    
    runner.test("Compare min implementation with naive", []() {
        StressTester stress;
        
        return stress.compare_simple<bool>(
            "LazySegmentTreeRangeMin vs Naive",
            
            [&](const vector<int>& arr) -> bool {
                auto min_func = [](int a, int b) { return min(a, b); };
                LazyRangeMax<int, decltype(min_func)> st(arr, 1e9, min_func);
                NaiveRangeMax<int, decltype(min_func)> naive(arr, 1e9, min_func);
                
                for (int op = 0; op < 15; op++) {
                    auto [l, r] = stress.random_range(arr.size());
                    int val = stress.random_int(1, 100);
                    
                    if (op % 3 == 0) {
                        int st_result = st.range_query(l, r);
                        int naive_result = naive.range_query(l, r);
                        if (st_result != naive_result) return false;
                        
                    } else if (op % 3 == 1) {
                        st.range_add(l, r, val);
                        naive.range_add(l, r, val);
                        
                    } else {
                        st.range_set(l, r, val);
                        naive.range_set(l, r, val);
                    }
                }
                
                return true;
            },
            
            [](const vector<int>& arr) -> bool {
                return true;
            },
            
            100,   
            30,    
            100    
        );
    });
}

void test_custom_combine_functions(TestRunner& runner) {
    runner.set_module("LazySegmentTreeRangeMax - Custom Functions");
    
    runner.test("GCD combine function", []() {
        vector<int> arr = {12, 18, 24, 30};
        auto gcd_func = [](int a, int b) { 
            if (b == 0) return a;
            return __gcd(a, b); 
        };
        LazyRangeMax<int, decltype(gcd_func)> st(arr, 0, gcd_func);
        
        ASSERT_EQ(st.range_query(0, 1), 6);  
        ASSERT_EQ(st.range_query(0, 3), 6);  
        
    
        st.range_set(0, 3, 15);
        ASSERT_EQ(st.range_query(0, 3), 15);
        
        return true;
    });
    
    runner.test("GCD edge cases", []() {
        auto gcd_func = [](int a, int b) { 
            if (b == 0) return a;
            return __gcd(a, b); 
        };
        
    
        vector<int> single = {42};
        LazyRangeMax<int, decltype(gcd_func)> st_single(single, 0, gcd_func);
        ASSERT_EQ(st_single.range_query(0, 0), 42);
        
    
        vector<int> coprime = {7, 11, 13};
        LazyRangeMax<int, decltype(gcd_func)> st_coprime(coprime, 0, gcd_func);
        ASSERT_EQ(st_coprime.range_query(0, 2), 1); 
        
    
        vector<int> powers = {8, 16, 32, 64}; 
        LazyRangeMax<int, decltype(gcd_func)> st_powers(powers, 0, gcd_func);
        ASSERT_EQ(st_powers.range_query(0, 3), 8);  
        
        return true;
    });
    
    runner.test("GCD with mixed operations", []() {
        vector<int> arr = {24, 36, 48};
        auto gcd_func = [](int a, int b) { 
            if (b == 0) return a;
            return __gcd(a, b); 
        };
        LazyRangeMax<int, decltype(gcd_func)> st(arr, 0, gcd_func);
        
        ASSERT_EQ(st.range_query(0, 2), 12); 
        
    
        st.range_set(1, 1, 60); 
        ASSERT_EQ(st.range_query(0, 2), 12);
        
    
        st.range_set(0, 2, 12); 
        ASSERT_EQ(st.range_query(0, 2), 12);
        
        return true;
    });
    
    runner.test("Bitwise OR combine function", []() {
        vector<int> arr = {1, 2, 4, 8}; 
        auto or_func = [](int a, int b) { return a | b; };
        LazyRangeMax<int, decltype(or_func)> st(arr, 0, or_func);
        
        ASSERT_EQ(st.range_query(0, 1), 3);  
        ASSERT_EQ(st.range_query(0, 3), 15); 
        
        return true;
    });
}

void test_type_aliases(TestRunner& runner) {
    runner.set_module("LazySegmentTreeRangeMax - Type Aliases");
    
    runner.test("Type aliases work correctly", []() {
    
    
        
        vector<int> arr = {1, 5, 3, 8, 2};
        auto max_func = [](int a, int b) { return max(a, b); };
        
    
        LazyRangeMax<int, decltype(max_func)> st(arr, -1e9, max_func);
        ASSERT_EQ(st.range_query(0, 4), 8);
        
    
        vector<long long> ll_arr = {1000000000LL, 5000000000LL, 3000000000LL};
        auto ll_max_func = [](long long a, long long b) { return max(a, b); };
        LazyRangeMax<long long, decltype(ll_max_func)> ll_st(ll_arr, -1e18, ll_max_func);
        ASSERT_EQ(ll_st.range_query(0, 2), 5000000000LL);
        
        return true;
    });
}

int main() {
    TestRunner runner;
    
    test_basic_max_functionality(runner);
    test_basic_min_functionality(runner);
    test_edge_cases(runner);
    test_performance_characteristics(runner);
    test_custom_combine_functions(runner);
    test_type_aliases(runner);
    stress_test_segment_tree_max(runner);
    
    runner.summary();
    return runner.get_exit_code();
}