#include <bits/stdc++.h>
#include "data-structures/LazySegmentTreeRangeMax.hpp"
using namespace std;

void solve() {
    int n, q;
    cin >> n >> q;
    
    vector <int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    const int INF = 1e9 + 1000;
    auto min_func = [&](int x, int y) -> int {
        return min(x, y);
    };
    LazyRangeMax <int, decltype(min_func)> seg(a, INF, min_func);
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;

        l--, r--;
        cout << seg.range_query(l, r) << "\n";
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