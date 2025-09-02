/**
 * Author: ArminHamedAzimi
 * Description: Lazy Propagation Segment Tree for Range Sum Queries
 * Features:
 * - Range sum queries in O(log n)
 * - Range addition updates in O(log n)  
 * - Range assignment updates in O(log n)
 * 
 * Time: O(log n) per operation
 * Space: O(n)
 * 
 * Usage:
 *  // Create from array
 *  vector <int> arr = {1, 3, 5, 7, 9};
 *  LazyRangeSum<int> st(arr);
 *  
 *  // Range operations
 *  cout << st.range_sum(1, 3) << endl;  // 15 (3+5+7)
 *  st.range_add(1, 3, 10);              // Add 10 to range [1,3]
 *  cout << st.range_sum(1, 3) << endl;  // 45 (13+15+17)
 *  st.range_set(0, 2, 5);               // Set range [0,2] to 5
 *  cout << st.range_sum(0, 4) << endl;  // 37 (5+5+5+17+9)
 * 
 *  // Point operations
 *  st.point_set(4, 100);
 *  cout << st.point_query(4) << endl;   // 100
 */

#pragma once
#include<bits/stdc++.h>
using namespace std;

template <typename T>
class LazyRangeSum {
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
    vector <T> tree;
    vector <LazyNode> lazy;
    
    void build(const vector <T> &arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            build(arr, (node << 1), start, mid);
            build(arr, (node << 1) | 1, mid + 1, end);
            tree[node] = tree[(node << 1)] + tree[(node << 1) | 1];
        }
    }
    
    void apply_add(int node, int start, int end, T val) {
        int len = end - start + 1;
        tree[node] += len * val;
        lazy[node].apply_add(val);
    }

    void apply_set(int node, int start, int end, T val) {
        int len = end - start + 1;
        tree[node] = len * val;
        lazy[node].apply_set(val);
    }

    void push(int node, int start, int end) {
        if (lazy[node].is_empty())
            return;

        int mid = (start + end) / 2;
        if (lazy[node].has_set) {
            apply_set((node << 1), start, mid, lazy[node].set_val);
            apply_set((node << 1) | 1, mid + 1, end, lazy[node].set_val);
        }
        else {
            apply_add((node << 1), start, mid, lazy[node].add_val);
            apply_add((node << 1) | 1, mid + 1, end, lazy[node].add_val);
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
        update_range((node << 1), start, mid, l, r, val, is_set);
        update_range((node << 1) | 1, mid + 1, end, l, r, val, is_set);        
        tree[node] = tree[(node << 1)] + tree[(node << 1) | 1];
    }
    
    T query_range(int node, int start, int end, int l, int r) {
        if (start > r || end < l) 
            return T(0);
        
        if (start >= l && end <= r)
            return tree[node];
        
        push(node, start, end);
        int mid = (start + end) / 2;
        return query_range((node << 1), start, mid, l, r) + query_range((node << 1) | 1, mid + 1, end, l, r);
    }
    
public:
    // Constructor with size and default value
    LazyRangeSum(int size, T default_val = T(0)) : n(size) {
        tree.resize(4 * n, T(0));
        lazy.resize(4 * n);
        
        if (default_val != T(0)) {
            vector <T> arr(n, default_val);
            build(arr, 1, 0, n - 1);
        }
    }
    
    // Constructor with initial array
    template<typename U>
    LazyRangeSum(const vector <U> &arr) : n(arr.size()) {
        tree.resize(4 * n);
        lazy.resize(4 * n);
        
        vector <T> converted_arr(arr.begin(), arr.end());
        build(converted_arr, 1, 0, n - 1);
    }
    
    // Build from array (can be called after default construction)
    template<typename U>
    void build(const vector <U> &arr) {
        assert(int(arr.size()) == n);
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
    
    // Get sum of elements in range [l, r]
    T range_sum(int l, int r) {
        assert(0 <= l && l <= r && r < n);
        return query_range(1, 0, n - 1, l, r);
    }
};

using IntLazyRangeSum = LazyRangeSum<int>;
using LongLazyRangeSum = LazyRangeSum<long long>;
using DoubleLazyRangeSum = LazyRangeSum<double>;
