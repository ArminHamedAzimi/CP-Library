/**
 * Author: KACTL (https://github.com/kth-competitive-programming/kactl)
 * Source: https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h
 * Adapted by: CP-Library
 *
 * Description: Dynamic Convex Hull Trick using a multiset (LineContainer).
 * Maintains a set of lines y = kx + m and supports maximum queries at x.
 * For minimum queries, insert lines with negated slope/intercept and negate the result.
 *
 * Time: O(log n) amortized per add/query
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Line {
	mutable ll k, m, p;
	bool operator<(const Line& o) const { return k < o.k; }
	bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	static const ll inf = LLONG_MAX;
	ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b); }
	bool isect(iterator x, iterator y) {
		if (y == end()) return x->p = inf, 0;
		if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) {
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	ll query(ll x) {
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};