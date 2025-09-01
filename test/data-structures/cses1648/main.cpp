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
        int t, l, r;
        cin >> t >> l >> r;
 
        if (t == 2) {
            l--, r--;
            cout << seg.range_sum(l, r) << "\n";
        }
        else {
            l--;
            seg.range_set(l, l, r);
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
