#pragma once
#include <bits/stdc++.h>
using namespace std;

class TestRunner {
private:
    int tests_run;
    int tests_passed;
    string current_module;
    
public:
    TestRunner();
    
    void set_module(const string& module);
    void test(const string& test_name, function<bool()> test_func);
    void summary();
    int get_exit_code();
};

class StressTester {
private:
    mt19937 rng;
    unsigned int seed;
    
public:
    StressTester();
    
    int random_int(int min_val, int max_val);
    long long random_ll(long long min_val, long long max_val);
    vector<int> random_array(int size, int min_val, int max_val);
    pair<int, int> random_range(int n);
    string random_string(int length, char min_char = 'a', char max_char = 'z');
    
    vector<pair<int, int>> random_edges(int n, int m, bool allow_self_loops = false);
    
    void print_progress(int current, int total);
    
    template<typename TestData, typename FastAlgo, typename NaiveAlgo>
    bool compare_algorithms(const string& algo_name, 
                          FastAlgo fast_algo, 
                          NaiveAlgo naive_algo,
                          function<TestData()> data_generator,
                          int num_tests = 1000) {
        
        cout << "🔄 Stress testing " << algo_name 
                  << " (" << num_tests << " tests)...\n";
        
        for (int test = 0; test < num_tests; test++) {
            try {
                TestData test_data = data_generator();
                
                auto result_fast = fast_algo(test_data);
                auto result_naive = naive_algo(test_data);
                if (result_fast != result_naive) {
                    cout << "💥 FAILED on test " << test + 1 << "\n";
                    cout << "Fast result: " << result_fast << "\n";
                    cout << "Naive result: " << result_naive << "\n";
                    print_test_data(test_data);
                    return false;
                }
                
                print_progress(test, num_tests);
                
            } catch (const exception& e) {
                cout << "💥 Exception on test " << test + 1 << ": " << e.what() << "\n";
                return false;
            }
        }
        
        cout << "✅ All " << num_tests << " stress tests PASSED!\n";
        return true;
    }
    
    template<typename T>
    bool compare_simple(const string& algo_name,
                       function<T(const vector<int>&)> fast_algo,
                       function<T(const vector<int>&)> naive_algo,
                       int num_tests = 1000,
                       int max_size = 100,
                       int max_val = 1000) {
        
        auto generator = [&]() {
            int size = random_int(1, max_size);
            return random_array(size, 1, max_val); 
        };
        
        return compare_algorithms<vector<int>>(
            algo_name, fast_algo, naive_algo, generator, num_tests
        );
    }
    
private:
    template<typename TestData>
    void print_test_data(const TestData& data) {
        cout << "Test data: ";
        if constexpr (is_same_v<TestData, vector<int>>) {
            cout << "[";
            for (size_t i = 0; i < min(data.size(), size_t(20)); ++i) {
                if (i > 0) cout << ", ";
                cout << data[i];
            }
            if (data.size() > 20) cout << ", ...";
            cout << "] (size=" << data.size() << ")";
        } else if constexpr (is_same_v<TestData, string>) {
            cout << "\"" << data.substr(0, min(data.size(), size_t(50)));
            if (data.size() > 50) cout << "...";
            cout << "\" (length=" << data.size() << ")";
        }
        cout << "\n";
    }
};
#define ASSERT_EQ(actual, expected) \
    do { \
        auto exp_val = (expected); \
        auto act_val = (actual); \
        if (exp_val != act_val) { \
            cout << "❌ Assertion failed at line " << __LINE__ << ": " \
                     << "expected " << exp_val << ", got " << act_val << endl; \
            return false; \
        } \
    } while(0)

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            cout << "❌ Assertion failed at line " << __LINE__ << ": " \
                     << #condition << " is false" << endl; \
            return false; \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            cout << "❌ Assertion failed at line " << __LINE__ << ": " \
                     << #condition << " is true" << endl; \
            return false; \
        } \
    } while(0)

#define ASSERT_NEAR(expected, actual, tolerance) \
    do { \
        auto exp_val = (expected); \
        auto act_val = (actual); \
        auto tol_val = (tolerance); \
        if (abs(exp_val - act_val) > tol_val) { \
            cout << "❌ Assertion failed at line " << __LINE__ << ": " \
                     << "expected " << exp_val << ", got " << act_val \
                     << " (tolerance: " << tol_val << ")" << endl; \
            return false; \
        } \
    } while(0)