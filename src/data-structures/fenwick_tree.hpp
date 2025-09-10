/**
 * Author: ArminHamedAzimi
 * Description: Fenwick (Binary Indexed) Trees for prefix/range sums
 *
 * Provides three variants:
 * - FenwickTree<T>: point add, prefix sum, range sum, lower_bound on prefix.
 * - FenwickRangeAdd<T>: range add, range sum (via two Fenwicks).
 * - FenwickRangeAP<T>: range add of an increasing-by-one arithmetic progression
 *   (i.e., add 1,2,3,... on [l,r]) and constant range add, range sum
 *   (via three Fenwicks using a quadratic prefix form).
 *
 * Time: O(log n) per update/query
 * Space: O(n)
 *
 * Usage:
 *  // 1) Basic point add + range sum
 *  FenwickTree<long long> ft(n);
 *  ft.add(3, 5);                    // a[3] += 5
 *  long long p = ft.prefix_sum(7);  // sum a[0..7]
 *  long long s = ft.range_sum(2, 6);
 *  int idx = ft.lower_bound(100);   // min i s.t. prefix_sum(i) >= 100; n if none
 *  int idx2 = ft.upper_bound(100);  // min i s.t. prefix_sum(i) > 100; n if none
 *
 *  // 2) Range add + range sum
 *  FenwickRangeAdd<long long> fr(n);
 *  fr.add_range(2, 5, 10);          // a[2..5] += 10
 *  long long rs = fr.range_sum(0, 8);
 *
 *  // 3) Range add arithmetic progression (1,2,3,...) on [l,r]
 *  FenwickRangeAP<long long> fa(n);
 *  fa.add_range_increasing_by_one(2, 4); // a[2]+=1, a[3]+=2, a[4]+=3
 *  fa.add_range_constant(0, 3, 5);       // optional: constant add via same structure
 *  long long tri = fa.range_sum(0, 4);
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick tree with 0-based public API and 1-based internal storage.
template <typename T = long long>
class FenwickTree {
private:
    int n;
    vector<T> bit;

public:
    explicit FenwickTree(int n_) : n(n_), bit(n_ + 1, T(0)) {
        assert(n_ >= 0);
    }

    // Add `val` to a[index]. 0 <= index < n
    void add(int index, T val) {
        assert(0 <= index && index < n);
        for (int i = index + 1; i <= n; i += i & -i) 
            bit[i] += val;
    }

    // Prefix sum: a[0] + ... + a[index]. If index < 0 returns 0.
    T prefix_sum(int index) const {
        if (index < 0) 
            return T(0);
        if (index >= n) 
            index = n - 1;
        T res = 0;
        for (int i = index + 1; i > 0; i -= i & -i)
            res += bit[i];
        return res;
    }

    // Sum on [l, r]. Returns 0 if l > r.
    T range_sum(int l, int r) const {
        if (l > r) 
            return T(0);
        assert(0 <= l && r < n);
        return prefix_sum(r) - prefix_sum(l - 1);
    }

    // Lower bound on prefix sums: smallest index i such that prefix_sum(i) >= target.
    // Returns n if no such index. Returns -1 if target <= 0.
    int lower_bound(T target) const {
        if (target <= T(0)) 
            return -1;
        int pos = 0;
        T sum = 0;
        
        int pw = 1;
        while (pw < n) 
            pw <<= 1;

        for (; pw; pw >>= 1) {
            int next = pos + pw;
            if (next <= n && sum + bit[next] < target) {
                sum += bit[next];
                pos = next;
            }
        }
        return (pos >= n? n : pos); // pos is the last with sum < target
    }

    // Upper bound on prefix sums: smallest index i such that prefix_sum(i) > target.
    // Returns n if no such index. Returns -1 if target < 0.
    int upper_bound(T target) const {
        if (target < T(0)) 
            return -1;
        int pos = 0;
        T sum = 0;

        int pw = 1;
        while (pw < n) 
            pw <<= 1;

        for (; pw; pw >>= 1) {
            int next = pos + pw;
            if (next <= n && sum + bit[next] <= target) {
                sum += bit[next];
                pos = next;
            }
        }
        return (pos >= n? n : pos);
    }
};

// Range add, range sum via two Fenwicks.
template <typename T = long long>
class FenwickRangeAdd {
private:
    int n;
    FenwickTree<T> b1, b2;

public:
    FenwickRangeAdd(int n_) : n(n_), b1(n_), b2(n_) {
        assert(n_ >= 0);
    }

    // Add val to all a[i] for i in [l, r].
    void add_range(int l, int r, T val) {
        assert(0 <= l && l <= r && r < n);
        b1.add(l, val);
        if (r + 1 < n)
            b1.add(r + 1, -val);
        b2.add(l, val * T(l));
        if (r + 1 < n)
            b2.add(r + 1, -val * T(r + 1));
    }

    // Sum a[0..x]
    T prefix_sum(int x) const {
        if (x < 0) 
            return T(0);
        T s1 = b1.prefix_sum(x);
        T s2 = b2.prefix_sum(x);
        return s1 * T(x + 1) - s2;
    }

    // Sum on [l, r]
    T range_sum(int l, int r) const {
        if (l > r) 
            return T(0);
        assert(0 <= l && r < n);
        return prefix_sum(r) - prefix_sum(l - 1);
    }
};

// Range add of an arithmetic progression 1,2,3,... on [l, r].
// Also supports constant range add using the same structure.
template <typename T = long long>
class FenwickRangeAP {
private:
    int n;
    FenwickTree<T> b0, b1, b2;

public:
    explicit FenwickRangeAP(int n_) : n(n_), b0(n_), b1(n_), b2(n_) {
        assert(n_ >= 0);
    }
    
    // Add val to all a[i] for i in [l, r].
    void add_range_constant(int l, int r, T val) {
        assert(0 <= l && l <= r && r < n);
        b1.add(l, 2 * val);
        if (r + 1 < n)
            b1.add(r + 1, -2 * val);
        b0.add(l, 2 * (T(-l + 1)) * val);
        if (r + 1 < n) 
            b0.add(r + 1, 2 * (T(r)) * val);
    }

    // Add an arithmetic progression 1,2,3,... on [l, r]
    // i.e., a[l]+=1, a[l+1]+=2, ..., a[r]+= (r-l+1)
    void add_range_increasing_by_one(int l, int r) {
        assert(0 <= l && l <= r && r < n);
        b2.add(l, 1);
        if (r + 1 < n) 
            b2.add(r + 1, -1);

        b1.add(l, T(-2 * l + 3));
        if (r + 1 < n) 
            b1.add(r + 1, -T(-2 * l + 3));

        T a = T(-l + 1);
        b0.add(l, a * (a + 1));
        if (r + 1 < n) {
            b0.add(r + 1, -a * (a + 1));
            T len = T(r - l + 1);
            b0.add(r + 1, len * (len + 1));
        }
    }

    // Sum a[0..x]
    T prefix_sum(long long x) const {
        if (x < 0)
            return T(0);
        if (x >= n)
            x = n - 1;
        T v2 = b2.prefix_sum(x);
        T v1 = b1.prefix_sum(x);
        T v0 = b0.prefix_sum(x);
        return (T(x) * T(x) * v2 + T(x) * v1 + v0) / T(2);
    }

    // Sum on [l, r]
    T range_sum(int l, int r) const {
        if (l > r) 
            return T(0);
        assert(0 <= l && r < n);
        return prefix_sum(r) - prefix_sum(l - 1);
    }
};
