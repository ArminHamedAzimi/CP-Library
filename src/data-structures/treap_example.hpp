/**
 * Author: ArminHamedAzimi
 * Description: Implicit treap with lazy operations on a binary string.
 * This implementation was written as a solution to Codeforces Gym 102787 Problem Y.
 * Problem: https://codeforces.com/gym/102787/problem/Y
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
// mt19937 rng(10);
struct value {
    array <int, 2> pre;
    array <int, 2> suf;
    array <int, 2> mx, cnt;
    int rev;
    array <int, 2> st;
    int xr;
    int sz;

    value(int x = 0) {
        pre[x] = mx[x] = suf[x] = 1;
        pre[x ^ 1] = mx[x ^ 1] = suf[x ^ 1] = 0;
        rev = 0;
        cnt[x] = 1;
        cnt[x ^ 1] = 0;
        st[0] = st[1] = 0;
        xr = 0;
        sz = 1;
    }
};

value merge(value l, value r) {
    value res;
    res.sz = l.sz + r.sz;
    res.rev = 0;
    for (int i = 0; i < 2; i++) {
        res.cnt[i] = l.cnt[i] + r.cnt[i];
        res.st[i] = 0;
        res.suf[i] = r.suf[i];
        if (r.suf[i] == r.sz)
            res.suf[i] += l.suf[i];

        res.pre[i] = l.pre[i];
        if (l.pre[i] == l.sz)
            res.pre[i] += r.pre[i];

        res.mx[i] = max(l.mx[i], max(r.mx[i], r.pre[i] + l.suf[i]));
    }
    return res;
}   

struct node {
    array <node *, 2> c;
    int priority;
    value val;
    int s;

    node(int x) {
        c[0] = c[1] = nullptr;
        priority = rng();
        val = value(x);
        s = x;
    }

    void pull() {
        val = value(s);
        if (c[0] != nullptr)
            val = merge(c[0]->val, val);
        
        if (c[1] != nullptr)
            val = merge(val, c[1]->val);
    }

    void reverse() {
        swap(c[0], c[1]);
        for (int i = 0; i < 2; i++) {
            swap(val.pre[i], val.suf[i]);
        }
        val.rev ^= 1;
    }

    void set(int x) {
        val.suf[x] = val.sz;
        val.pre[x] = val.sz;
        val.mx[x] = val.sz;
        val.cnt[x] = val.sz;
        
        val.suf[x ^ 1] = 0;
        val.pre[x ^ 1] = 0;
        val.mx[x ^ 1] = 0;
        val.cnt[x ^ 1] = 0;

        s = x;

        val.st[x] = 1;
        val.st[x ^ 1] = 0;
    }

    void xr() {
        swap(val.pre[0], val.pre[1]);
        swap(val.suf[0], val.suf[1]);
        swap(val.mx[0], val.mx[1]);
        swap(val.cnt[0], val.cnt[1]);

        s ^= 1;

        if (val.st[0] || val.st[1])
            swap(val.st[0], val.st[1]);
        else
            val.xr ^= 1;
    }

    void push() {
        if (val.rev) {
            if (c[0] != nullptr)
                c[0]->reverse();
            if (c[1] != nullptr)
                c[1]->reverse();
        }

        if (val.xr) {
            if (c[0] != nullptr)
                c[0]->xr();
            if (c[1] != nullptr)
                c[1]->xr();
        }

        for (int i = 0; i < 2; i++) {
            if (val.st[i]) {
                if (c[0] != nullptr)
                    c[0]->set(i);
                if (c[1] != nullptr)
                    c[1]->set(i);
            }
        }

        val.rev = 0;
        val.st[0] = val.st[1] = 0;
        val.xr = 0;
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
        l->c[1] = merge(l->c[1], r);
        l->pull();
        return l;
    }

    r->c[0] = merge(l, r->c[0]);
    r->pull();
    return r;
}

pair <node *, node *> split(node *x, int cnt) {
    if (cnt == 0)
        return {nullptr, x};

    x->push();
    
    int lc = 0;
    if (x->c[0] != nullptr)
        lc += x->c[0]->val.sz;

    if (cnt <= lc) {
        auto p1 = split(x->c[0], cnt);
        x->c[0] = p1.second;
        x->pull();
        return {p1.first, x};
    }

    auto p1 = split(x->c[1], cnt - lc - 1);
    x->c[1] = p1.first;
    x->pull();
    return {x, p1.second};
}

void solve() {
    int n, q;
    cin >> n >> q;

    string s;
    cin >> s;

    node * root = nullptr;
    for (char c: s)
        root = merge(root, new node(c - '0'));

    // cout << "XD" << endl;

    for (int i = 0; i < q; i++) {
        // cout << "check - " << i << endl;
        int t, l, r;
        cin >> t >> l >> r;

        if (t == 1) {
            auto p1 = split(root, r);
            auto p2 = split(p1.first, l - 1);
            p2.second->xr();
            root = merge(p2.first, merge(p2.second, p1.second));
        }
        else if (t == 2) {
            auto p1 = split(root, r);
            auto p2 = split(p1.first, l - 1);
            p2.second->reverse();
            root = merge(p2.first, merge(p2.second, p1.second));
        }
        else {
            auto p1 = split(root, r);
            auto p2 = split(p1.first, l - 1);

            int cnt0 = p2.second->val.cnt[0];
            auto p3 = split(p2.second, cnt0);
            if (p3.first != nullptr)
                p3.first->set(0);
            if (p3.second != nullptr)
                p3.second->set(1);

            root = merge(merge(p2.first, merge(p3.first, p3.second)), p1.second);
        }


        // cout << endl;
        // cout << "root:\n";
        // for (int i = 0; i < 2; i++) {
        //     cout << "suf " << i << " -> " << root->val.suf[i] << "\n";
        //     cout << "pre " << i << " -> " << root->val.pre[i] << "\n";
        //     cout << "cnt " << i << " -> " << root->val.cnt[i] << "\n";
        //     cout << "mx " << i << " -> " << root->val.mx[i] << "\n";
        // }
        // cout << endl;

        cout << max(root->val.mx[0], root->val.mx[1]) << "\n";
    }
}
