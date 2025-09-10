/**
 * Author: ArminHamedAzimi
 * Description: Li Chao Segment Tree (max) over integer domain [L, R].
 * Notes: This is a light class wrapper around a classic prewritten template,
 *        with improved naming, but the core structure and recursion are unchanged.
 *
 * Features:
 * - Add line y = a*x + b on [L, R]
 * - Point query for maximum value at x
 *
 * Time: O(log(R-L+1)) per add/query
 * Space: O(MAX_NODES) preallocated
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

class LiChaoTree {
public:
    static const long long INF = (long long)1e18 + 10000;
    static const int MAX_NODES = (int)1e7 + 100;

    struct Line {
        long long a, b; // y = a*x + b
        Line(long long a_ = 0, long long b_ = -INF) : a(a_), b(b_) {}
        inline long long value(long long x) const { 
            return a * x + b;
        }
    };

    struct Node {
        int left = 0, right = 0;
        Line line; // best line on this segment
    };

private:
    vector<Node> nodes;
    int node_count;
    int L, R;

    inline int new_node() {
        return node_count++;
    }

    void insert_line(int l, int r, int id, Line ln) {
        int mid = (l + r) / 2;
        if (ln.value(mid) > nodes[id].line.value(mid)) swap(ln, nodes[id].line);
        if (l == r) return;

        if (ln.value(l) > nodes[id].line.value(l)) {
            if (nodes[id].left == 0) 
                nodes[id].left = new_node();
            insert_line(l, mid, nodes[id].left, ln);
        } else {
            if (nodes[id].right == 0) 
                nodes[id].right = new_node();
            insert_line(mid + 1, r, nodes[id].right, ln);
        }
    }

    long long query_point(int l, int r, int id, long long x) {
        long long best = nodes[id].line.value(x);
        if (l == r) return best;
        int mid = (l + r) / 2;
        if (x <= mid) {
            if (nodes[id].left == 0) 
                nodes[id].left = new_node();
            return max(best, query_point(l, mid, nodes[id].left, x));
        } else {
            if (nodes[id].right == 0) 
                nodes[id].right = new_node();
            return max(best, query_point(mid + 1, r, nodes[id].right, x));
        }
    }

public:
    LiChaoTree(int l = 0, int r = 1000000000): nodes(MAX_NODES), node_count(1), L(l), R(r) {}

    void add_line(long long a, long long b) { 
        insert_line(L, R, 0, Line(a, b));
    }
    long long query(long long x) {
        return query_point(L, R, 0, x);
    }
};
