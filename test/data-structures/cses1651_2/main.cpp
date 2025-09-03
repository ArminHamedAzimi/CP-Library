#include <bits/stdc++.h>
#include "data-structures/fenwick_tree.hpp"
using namespace std;

void solve() {
    int n, q;
    cin >> n >> q;

    vector <int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    FenwickRangeAdd fen(n);
    for (int i = 0; i < n; i++)
        fen.add_range(i, i, a[i]);
    
    for (int i = 0; i < q; i++) {
        int t;
        cin >> t;

        if (t == 1) {
            int l, r, x;
            cin >> l >> r >> x;

            l--, r--;

            fen.add_range(l, r, x);
        }
        else {
            int k;
            cin >> k; k--;
            cout << fen.range_sum(k, k) << "\n";
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
