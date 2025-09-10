#include <bits/stdc++.h>
#include "data-structures/LazySegmentTreeRangeSum.hpp"
using namespace std;

void solve() {
    int n, q;
    cin >> n >> q;

    vector <int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    LongLazyRangeSum seg(a);
    for (int i = 0; i < q; i++) {
        int t;
        cin >> t;

        if (t == 1) {
            int l, r, val;
            cin >> l >> r >> val;
            l--, r--;
            seg.range_add(l, r, val);
        }
        else {
            int k;
            cin >> k;

            k--;
            cout << seg.range_sum(k, k) << "\n";
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int q = 1;
    // cin >> q;

    while (q--) {
        solve();
    }
}
