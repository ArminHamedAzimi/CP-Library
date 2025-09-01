/**
 * Author: ArminHamedAzimi
 * Description: Generic Lazy Propagation Segment Tree with Custom Combine Function
 * 
 * Features:
 * - Range queries with custom combine function (max, min, gcd, etc.) in O(log n)
 * - Range addition updates in O(log n)
 * - Range assignment updates in O(log n)
 * - Generic design supporting any associative operation
 * 
 * Time: O(log n) per operation
 * Space: O(n)
 * 
 * Template Parameters:
 *   T - Data type for elements (int, long long, etc.)
 *   F - Function type for combine operation (function<T(T,T)>)
 * 
 * Usage:
 *  // Range Maximum Queries
 *  vector<int> arr = {1, 3, 5, 7, 9};
 *  auto max_func = [](int a, int b) { return max(a, b); };
 *  LazyRangeMax<int, decltype(max_func)> st(arr.size(), INT_MIN, max_func);
 *  st.build(arr);
 *  
 *  cout << st.range_query(1, 3) << endl;  // 7 (max of 3,5,7)
 *  st.range_add(1, 3, 10);                // Add 10: {1, 13, 15, 17, 9}
 *  cout << st.range_query(1, 3) << endl;  // 17 (max of 13,15,17)
 *  
 *  // Using type aliases
 *  using MaxTree = LazyRangeMax<int, function<int(int,int)>>;
 *  MaxTree max_st(5, INT_MIN, [](int a, int b) { return max(a, b); });
 */

#pragma once
#include<bits/stdc++.h>
using namespace std;

template <typename T, typename F>
class LazyRangeMax {
private:
    struct LazyNode {
        T add_val;
        T set_val;
        bool has_set;
        
        LazyNode() : add_val(0), set_val(0), has_set(false) {}
        void apply_add(T val) {
            if (has_set)
                set_val += val;
            else
                add_val += val;
        }
        
        void apply_set(T val) {
            set_val = val;
            add_val = 0;
            has_set = true;
        }
        
        void clear() {
            add_val = 0;
            set_val = 0;
            has_set = false;
        }
        
        bool is_empty() const {
            return !has_set && add_val == 0;
        }
    };
    
    int n;
    T identity;
    vector <T> tree;
    vector <LazyNode> lazy;
    F combine_func;
    
    
    void build(const vector <T> &arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        } 
        int mid = (start + end) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree[node] = combine_func(tree[2 * node], tree[2 * node + 1]);
    }
    
    void apply_add(int node, int start, int end, T val) {
        tree[node] += val;
        lazy[node].apply_add(val);
    }

    void apply_set(int node, int start, int end, T val) {
        tree[node] = val;
        lazy[node].apply_set(val);
    }

    void push(int node, int start, int end) {
        if (lazy[node].is_empty())
            return;

        int mid = (start + end) / 2;
        if (lazy[node].has_set) {
            apply_set(2 * node, start, mid, lazy[node].set_val);
            apply_set(2 * node + 1, mid + 1, end, lazy[node].set_val);
        }
        else {
            apply_add(2 * node, start, mid, lazy[node].add_val);
            apply_add(2 * node + 1, mid + 1, end, lazy[node].add_val);
        }
        
        lazy[node].clear();
    }
    
    void update_range(int node, int start, int end, int l, int r, T val, bool is_set) {        
        if (start > r || end < l) 
            return;
        
        if (start >= l && end <= r) {
            if (is_set)
                apply_set(node, start, end, val);
            else 
                apply_add(node, start, end, val);
            return;
        }
        
        int mid = (start + end) / 2;
        push(node, start, end);
        update_range(2 * node, start, mid, l, r, val, is_set);
        update_range(2 * node + 1, mid + 1, end, l, r, val, is_set);        
        tree[node] = combine_func(tree[2 * node], tree[2 * node + 1]);
    }
    
    T query_range(int node, int start, int end, int l, int r) {
        if (start > r || end < l) 
            return identity;
        
        if (start >= l && end <= r)
            return tree[node];
        
        push(node, start, end);
        int mid = (start + end) / 2;
        return combine_func(query_range(2 * node, start, mid, l, r), query_range(2 * node + 1, mid + 1, end, l, r));
    }
    
public:
    // Constructor with size and default value
    LazyRangeMax(int size, T identity_val, const F& func) 
        : n(size), identity(identity_val), tree(4 * n, identity_val), lazy(4 * n), 
          combine_func(func) {
        assert(size > 0);
    }
    
    // Constructor with initial array
    template<typename U>
    LazyRangeMax(const vector <U> &arr, T identity_val, const F& func) 
        : n(arr.size()), identity(identity_val), combine_func(func) {
        tree.resize(4 * n);
        lazy.resize(4 * n);
        
        vector <T> converted_arr(arr.begin(), arr.end());
        build(converted_arr, 1, 0, n - 1);
    }
    
    // Build from array (can be called after default construction)
    template<typename U>
    void build(const vector <U> &arr) {
        assert(arr.size() == n);
        vector <T> converted_arr(arr.begin(), arr.end());
        build(converted_arr, 1, 0, n - 1);
    }
    
    // Add val to all elements in range [l, r]
    void range_add(int l, int r, T val) {
        assert(0 <= l && l <= r && r < n);
        update_range(1, 0, n - 1, l, r, val, false);
    }
    
    // Set all elements in range [l, r] to val
    void range_set(int l, int r, T val) {
        assert(0 <= l && l <= r && r < n);
        update_range(1, 0, n - 1, l, r, val, true);
    }
    
    // Get result of combine function on elements in range [l, r]
    T range_query(int l, int r) {
        assert(0 <= l && l <= r && r < n);
        return query_range(1, 0, n - 1, l, r);
    }
};

// Convenient type aliases for common use cases
template<typename T>
using LazyRangeMaxTree = LazyRangeMax<T, std::function<T(T, T)>>;

template<typename T>
using LazyRangeMinTree = LazyRangeMax<T, std::function<T(T, T)>>;

// Specific instantiations for max operations
using IntLazyRangeMax = LazyRangeMax<int, std::function<int(int, int)>>;
using LongLazyRangeMax = LazyRangeMax<long long, std::function<long long(long long, long long)>>;
using DoubleLazyRangeMax = LazyRangeMax<double, std::function<double(double, double)>>;