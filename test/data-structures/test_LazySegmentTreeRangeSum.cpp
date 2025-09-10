#include "../test_runner.h"
#include "data-structures/LazySegmentTreeRangeSum.hpp"
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class NaiveSegmentTree {
    vector <T> arr;
public:
    NaiveSegmentTree(const vector <T>& in) : arr(in) {}
    
    void range_add(int l, int r, T val) {
        for (int i = l; i <= r; i++) 
            arr[i] += val;
    }
    
    void range_set(int l, int r, T val) {
        for (int i = l; i <= r; i++) 
            arr[i] = val;
    }
    
    T range_sum(int l, int r) {
        T sum = 0;
        for (int i = l; i <= r; i++) 
            sum += arr[i];
        return sum;
    }
};

void test_basic_functionality(TestRunner& runner) {
    runner.set_module("Segment Tree - Basic Operations");
    
    runner.test("Construction and basic queries", []() {
        vector <int> arr = {1, 3, 5, 7, 9};
        LazyRangeSum <int> st(arr);
        
        ASSERT_EQ(st.range_sum(0, 0), 1);
        ASSERT_EQ(st.range_sum(1, 1), 3);
        ASSERT_EQ(st.range_sum(0, 2), 9);
        ASSERT_EQ(st.range_sum(2, 4), 21);
        ASSERT_EQ(st.range_sum(0, 4), 25);
        
        return true;
    });
    
    runner.test("Range addition updates", []() {
        vector <int> arr = {0, 0, 0, 0, 0};
        LazyRangeSum <int> st(arr);
        st.range_add(1, 3, 5);
        ASSERT_EQ(st.range_sum(0, 0), 0);
        ASSERT_EQ(st.range_sum(1, 1), 5);
        ASSERT_EQ(st.range_sum(2, 2), 5);
        ASSERT_EQ(st.range_sum(3, 3), 5);
        ASSERT_EQ(st.range_sum(4, 4), 0);
        ASSERT_EQ(st.range_sum(1, 3), 15);
        
        return true;
    });
    
    runner.test("Range assignment updates", []() {
        vector <int> arr = {1, 2, 3, 4, 5};
        LazyRangeSum <int> st(arr);
        st.range_set(1, 3, 10);
        ASSERT_EQ(st.range_sum(0, 0), 1);
        ASSERT_EQ(st.range_sum(1, 1), 10);
        ASSERT_EQ(st.range_sum(2, 2), 10);
        ASSERT_EQ(st.range_sum(3, 3), 10);
        ASSERT_EQ(st.range_sum(4, 4), 5);
        ASSERT_EQ(st.range_sum(1, 3), 30);
        
        return true;
    });
    
    runner.test("Mixed operations", []() {
        vector <int> arr = {1, 1, 1, 1, 1};
        LazyRangeSum <int> st(arr);
        
        st.range_set(0, 4, 5);
        ASSERT_EQ(st.range_sum(0, 4), 25);
        
        st.range_add(1, 3, 2);   
        ASSERT_EQ(st.range_sum(0, 4), 31); 
        
        st.range_set(2, 2, 100);
        ASSERT_EQ(st.range_sum(2, 2), 100);
        ASSERT_EQ(st.range_sum(0, 4), 124);
        
        return true;
    });
}

void test_edge_cases(TestRunner& runner) {
    runner.set_module("Segment Tree - Edge Cases");
    
    runner.test("Single element array", []() {
        vector <int> arr = {42};
        LazyRangeSum <int> st(arr);
        
        ASSERT_EQ(st.range_sum(0, 0), 42);
        
        st.range_add(0, 0, 10);
        ASSERT_EQ(st.range_sum(0, 0), 52);
        
        st.range_set(0, 0, 100);
        ASSERT_EQ(st.range_sum(0, 0), 100);
        
        return true;
    });
    
    runner.test("Large values (long long)", []() {
        vector <long long> arr = {1000000000LL, 2000000000LL, 3000000000LL};
        LazyRangeSum <long long> st(arr);
        
        ASSERT_EQ(st.range_sum(0, 2), 6000000000LL);
        
        st.range_add(0, 2, 1000000000LL);
        ASSERT_EQ(st.range_sum(0, 2), 9000000000LL);
        
        return true;
    });
    
    runner.test("Negative numbers", []() {
        vector <int> arr = {-5, -3, -1, 1, 3};
        LazyRangeSum <int> st(arr);
        
        ASSERT_EQ(st.range_sum(0, 4), -5);
        ASSERT_EQ(st.range_sum(0, 2), -9);
        ASSERT_EQ(st.range_sum(3, 4), 4);
        
        return true;
    });
    
    runner.test("Zero array", []() {
        vector <int> arr(10, 0);
        LazyRangeSum <int> st(arr);
        
        ASSERT_EQ(st.range_sum(0, 9), 0);
        
        st.range_add(3, 7, 5);
        ASSERT_EQ(st.range_sum(3, 7), 25);
        ASSERT_EQ(st.range_sum(0, 9), 25);
        
        return true;
    });
}

void test_performance_characteristics(TestRunner& runner) {
    runner.set_module("Segment Tree - Performance");
    
    runner.test("Large array operations", []() {
        const int n = 10000;
        vector <int> arr(n, 1);
        LazyRangeSum <int> st(arr);
        

        st.range_set(0, n - 1, 5);
        ASSERT_EQ(st.range_sum(0, n - 1), 5 * n);

        st.range_add(1000, 8000, 3);
        ASSERT_EQ(st.range_sum(1000, 8000), (8000-1000+1) * 8);
        
        return true;
    });
}

void stress_test_segment_tree(TestRunner& runner) {
    runner.set_module("Segment Tree - Stress Testing");
    
    runner.test("Compare with naive implementation", []() {
        StressTester stress;
        
        return stress.compare_simple<bool>(
            "Segment Tree vs Naive",
            
            [&](const vector <int>& arr) -> bool {
                LazyRangeSum <int> st(const_cast<vector <int>&>(arr));
                NaiveSegmentTree<int> naive(arr);
                
                for (int op = 0; op < 20; op++) {
                    auto [l, r] = stress.random_range(arr.size());
                    int val = stress.random_int(1, 100);
                    
                    if (op % 3 == 0) {
                        int st_result = st.range_sum(l, r);
                        int naive_result = naive.range_sum(l, r);
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
                    if (st.range_sum(i, i) != naive.range_sum(i, i)) {
                        return false;
                    }
                }
                
                return true;
            },
            
            [](const vector <int>& arr) -> bool {
                return true;
            },
            
            500,
            50,
            100
        );
    });
}

int main() {
    TestRunner runner;
    
    test_basic_functionality(runner);
    test_edge_cases(runner);
    test_performance_characteristics(runner);
    stress_test_segment_tree(runner);
    
    runner.summary();
    return runner.get_exit_code();
}