/**
 * Author: ArminHamedAzimi
 * Description: Sparse Table (Static RMQ) with generic idempotent combine function
 * Source: Adapted from KACTL RMQ with small changes
 *   https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/RMQ.h
 *
 * Features:
 * - O(1) query on immutable arrays after O(n log n) preprocessing.
 * - Generic combine function `F` (e.g., min, max, gcd) applied on overlapping blocks.
 * - Query helper `get(l, r)` for inclusive ranges [l, r].
 *
 * Requirements:
 * - The combine function `F(T, T) -> T` must be associative and idempotent
 *   (f(x, x) = x), which holds for min/max/gcd. This enables overlapping block merge.
 * - Input is 0-based indexed.
 *
 * Time: Build O(n log n), Query O(1)
 * Space: O(n log n)
 *
 * Usage:
 *  vector<int> a = {5, 2, 7, 3, 6};
 *  auto fmin = [](int x, int y) { return min(x, y); };
 *  RMQ<int, decltype(fmin)> st(a, fmin);
 *  int mn = st.get(1, 3); // min on [1,3] = 2
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;
template<typename T, typename F>
struct RMQ {
	vector<vector<T>> jmp;
    F func;
	RMQ(const vector<T>& V, const F& f): jmp(1, V), func(f) {
		for (int pw = 1, k = 1; pw * 2 <= int(V.size()); pw *= 2, ++k) {
				jmp.emplace_back(int(V.size()) - pw * 2 + 1);
			for (int j = 0; j < int(jmp[k].size()); j++)
				jmp[k][j] = func(jmp[k - 1][j], jmp[k - 1][j + pw]);
		}
	}
    T get(int l, int r) {
        return query(l, r + 1);
    }

    private:
	T query(int l, int r) {
		assert(l < r);
		int dep = 31 - __builtin_clz(r - l);
		return func(jmp[dep][l], jmp[dep][r - (1 << dep)]);
	}
};
