/**
 * Author: ArminHamedAzimi
 * Description: Disjoint Set Union (Union-Find) with path compression and union by size.
 *
 * Features:
 * - Amortized near-constant time operations.
 * - `unite(u, v)` merges the components of u and v.
 * - `sameComponent(u, v)` checks if two nodes are in the same set.
 * - `parent(u)` returns the representative of u's set (with compression).
 * - `size(u)` returns the size of u's set.
 *
 * Time: Inverse Ackermann
 * Space: O(n)
 *
 * Usage:
 *  DSU dsu(n);
 *  dsu.unite(0, 1);
 *  bool ok = dsu.sameComponent(0, 1); // true
 *  int rep = dsu.parent(0);
 *  int sz  = dsu.size(1);
 */

#pragma once
#include <bits/stdc++.h>

using namespace std;

struct DSU {
    int n;
    vector <int> par;
    vector <int> sz;
    DSU() {}
    DSU(int n): n(n), sz(n, 1) {
        par.resize(n);
        iota(par.begin(), par.end(), 0);
    }
    int parent(int u) {
        if (par[u] == u)
            return u;
        return par[u] = parent(par[u]);
    }
    int size(int u) {
        return sz[parent(u)];
    }
    bool sameComponent(int u, int v) {
        return parent(u) == parent(v);
    }
    bool unite(int u, int v) {
        u = parent(u);
        v = parent(v);

        if (u == v)
            return false;
        if (sz[v] < sz[u])
            swap(u, v);
        
        par[u] = v;
        sz[v] += sz[u];
        return true;
    }
};
