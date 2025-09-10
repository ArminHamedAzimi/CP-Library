
/**
 * Author: ArminHamedAzimi
 * Description: Offline Convex Hull Trick for maximum queries on lines y = kx + b.
 * Maintains upper hull breakpoints assuming lines are added in non-decreasing
 * slope order. Queries return the maximum y at a given x in O(log n).
 *
 * Features:
 * - add(k, b): add a line y = kx + b; slopes must be non-decreasing.
 *   If a line with the same slope exists, keeps the one with larger intercept.
 * - get(x): binary searches breakpoints to return max y at x.
 * - 128-bit-safe intersection comparisons via a rational `fraction` type.
 *
 * Requirements:
 * - Call `add` in non-decreasing slope order. For arbitrary order, sort lines by
 *   slope first and then add; ties resolved by higher intercept.
 * - For minimum queries, insert lines with negated slope/intercept and negate
 *   the returned value.
 *
 * Time: Build O(n) after slope-sorted insertion; Query O(log n)
 * Space: O(n)
 *
 * Usage:
 *  // Prepare lines and sort by slope (k) ascending
 *  vector<pair<long long,long long>> ls = {{1, 5}, {2, 3}, {4, -1}};
 *  sort(ls.begin(), ls.end());
 *
 *  offline_convex_hull cht;
 *  for (auto [k, b] : ls) cht.add(k, b);
 *
 *  long long x = 10;
 *  long long best = cht.get(x); // maximum y at x
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
// kx + b -> {k, b}
struct fraction {
    long long x, y;
    fraction(long long x = 0, long long y = 0): x(x), y(y) {}

    bool operator < (fraction p) {
        return (__int128_t)x * p.y < (__int128_t)p.x * y;
    }
    bool operator <= (fraction p) {
        return (__int128_t)x * p.y <= (__int128_t)p.x * y;
    }
};
struct Line {
    long long k, b;
    Line(long long k = 0, long long b = 0): k(k), b(b) {}
    long long get(long long x) {
        return k * x + b;
    }
};

fraction intersect(Line a, Line b) {
    return fraction(b.b - a.b, a.k - b.k);
}

struct offline_convex_hull {
    vector <fraction> inter;
    vector <Line> lines;
    const long long INF = 1e18 + 100;
    void add(long long k, long long b) {
        if (lines.size() > 0 && lines.back().k == k) {
            if (lines.back().b < b) {
                lines.pop_back();
                lines.push_back(Line(k, b));
            }
            return;
        }

        Line l(k, b);
        while (lines.size() > 1 && intersect(l, lines.back()) <= intersect(l, lines.end()[-2])) {
            lines.pop_back();
            inter.pop_back();
        }

        if (lines.size() > 0)
            inter.push_back(intersect(l, lines.back()));
        else
            inter.push_back(fraction(-INF, 1));
        lines.push_back(l);
    }
    long long get(long long x) {
        int ind = lower_bound(inter.begin(), inter.end(), fraction(x, 1)) - inter.begin() - 1;
        return lines[ind].get(x);
    }
};
