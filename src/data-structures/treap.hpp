/**
 * Treap (Cartesian tree) — randomized balanced BST.
 * This file implements an implicit treap (ordered by position) with subtree size.
 *
 * Operations provided here:
 * - merge(l, r): combine two treaps (all elements of l come before r)
 * - split(x, cnt): split first cnt elements into {left, right}
 * - pull(): recompute aggregates (currently only size)
 * - push(): placeholder for lazy propagation (extend as needed)
 *
 * Complexity: expected O(log n) per operation; space O(n).
 *
 * Usage example:
 *   node* root = nullptr;
 *   // insert at position pos
 *   auto [L, R] = split(root, pos);
 *   root = merge(merge(L, new node()), R);
 *   // erase at position pos
 *   auto [A, B] = split(root, pos);
 *   auto [M, C] = split(B, 1);
 *   // delete M;
 *   root = merge(A, C);
 */
#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
// mt19937 rng(10);
struct value {
    int sz;

    value(): sz(1) {}
};

value merge(value l, value r) {
    value res;
    res.sz = l.sz + r.sz;
    return res;
}

struct node {
    node *l, *r;
    int priority;
    value val;
    int s;

    node() {
        l = nullptr, r = nullptr;
        priority = rng();
        val = value();
    }

    void pull() {
        val = value();
        if (l != nullptr)
            val = merge(l->val, val);
        
        if (r != nullptr)
            val = merge(val, r->val);
    }

    void push() {
        
    }
};

node * merge(node *l, node *r) {
    if (l == nullptr)
        return r;
    if (r == nullptr)
        return l;
   
    l->push();
    r->push();

    if (l->priority < r->priority) {
        l->l = merge(l->r, r);
        l->pull();
        return l;
    }

    r->l = merge(l, r->l);
    r->pull();
    return r;
}

pair <node *, node *> split(node *x, int cnt) {
    if (cnt == 0)
        return {nullptr, x};

    x->push();
    
    int lc = 0;
    if (x->l != nullptr)
        lc += x->l->val.sz;

    if (cnt <= lc) {
        auto p1 = split(x->l, cnt);
        x->l = p1.second;
        x->pull();
        return {p1.first, x};
    }

    auto p1 = split(x->r, cnt - lc - 1);
    x->r = p1.first;
    x->pull();
    return {x, p1.second};
}
